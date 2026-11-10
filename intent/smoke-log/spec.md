# Spec: smoke-log

Author: Ryu Jahyeon
Status: draft

<!-- 입력: intent/smoke-log/intent.md (PR #15 머지로 승인, 2026-11-10). 경로는 2D-Portfolio/ 기준. -->

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | `Client/` 를 작업 디렉터리로 `Client.exe -smoke` 를 실행하면 입력 없이 로고 → 메뉴 → 스테이지 → 스킬 1회까지 진행하고 **30초 안에 스스로 종료**하며, 모든 확인이 통과하면 종료 코드 0 | `timeout 60 ../Exe/<플랫폼>/<구성>/Client/Client.exe -smoke; echo $?` → `0` |
| R2 | 작업 디렉터리의 `smoke.log` 에 아래 확인 줄이 모두 `PASS` 로 남고 마지막 줄이 `RESULT PASS` 다: 장면 0·1·2 초기화, 배경음 `Wilderness`·`Tristram` 로드·재생, `Map.dat` 타일 800개, 몬스터 5마리, 스킬(`BoneSpear`) 객체 생성 | `grep` 으로 각 줄 확인 |
| R3 | 확인 하나라도 실패하면 그 이유가 `FAIL` 줄로 남고 종료 코드가 0 이 아니다. 스모크 모드에서는 `ERR_MSG`·FMOD 오류가 **메시지박스를 띄우지 않고** 로그에 남아 실행이 멈추지 않는다 | 로컬에서 리소스 경로 하나를 일시적으로 틀리게 해 실행 → 종료 코드 ≠ 0, `FAIL` 줄에 리소스 이름, 30초 안에 종료(되돌림, 커밋 안 함) |
| R4 | `-smoke` 없이 실행하면 동작이 지금과 같고 `smoke.log` 가 생기지 않는다 | 기존 8초 스모크 + 실행 전후 `smoke.log` 없음 |
| R5 | `MFC_000/` 를 작업 디렉터리로 `MFC_000.exe -smoke <Map.dat 경로>` 를 실행하면 대화상자·창 없이 파일을 읽어 타일 수를 `smoke.log` 에 남기고 종료한다(800개면 0, 열기 실패·0개면 ≠ 0) | `../Client/Map.dat` → 0 + `tiles=800`, 없는 경로 → ≠ 0 |
| R6 | Win32 Debug·Release 에서 R1~R5 를 만족한다(x64 는 이 기능을 받은 뒤 `x64-migration` 에서 같은 명령으로 확인) | 두 구성 실행 |
| R7 | 새 경고 0 — Win32 Debug·Release 경고 수가 198 그대로 | 빌드 출력 집계(`-m:1 -nodeReuse:false`) |
| R8 | CLAUDE.md 「빌드 / 테스트 / 린트」의 「테스트」 줄이 스모크 명령·기대 출력·`smoke.log` 위치로 바뀐다 | 문서 검토 |

## 설계

### 로그 모듈 — `Headers/SmokeLog.h` (새 파일, 헤더 전용, Client·MFC_000 공용)

```cpp
bool  SmokeBegin(const TCHAR* _pLogPath);   // 스모크 모드 켜기 + 로그 파일 생성(덮어쓰기)
bool  IsSmokeMode();
void  SmokeLog(const TCHAR* _pFormat, ...); // 경과 ms + 한 줄 (정보)
void  SmokeCheck(bool _bPass, const TCHAR* _pFormat, ...); // PASS/FAIL 한 줄, 실패를 누적
int   SmokeEnd();                           // RESULT 줄 기록·파일 닫기, 종료 코드 반환(0 = 전부 PASS)
```
- 상태(켜짐 여부·파일·실패 수·시작 시각)는 함수 안 `static` 하나에 둔다 — 헤더 전용이라 두 프로젝트에 링크 설정 없이 들어가고,
  전역 변수 정의 중복(ODR) 문제도 없다(KISS).
- **공용 헤더에 두는 이유**: 툴(R5)도 같은 기록 형식을 쓴다(DRY). `Include.h` 첫 줄 `Debug.hpp` 다음에 include.
- 파일은 UTF-8(BOM 없음)로 쓴다 — WSL·`grep` 에서 바로 읽힌다. 한 줄마다 즉시 flush(크래시해도 마지막 줄까지 남게).
- `_DEBUG` 조건 없이 컴파일된다(Release 지원, intent 결정). 스모크가 꺼져 있으면 모든 함수가 즉시 반환한다.
- 줄 형식: `[  1234 ms] PASS scene 2 init` / `FAIL ...` / `INFO ...` / 마지막 `RESULT PASS|FAIL (fail=N)`.

### Client — 진행과 확인 지점

| 위치 | 스모크 모드일 때만 하는 일 |
|---|---|
| `Client.cpp` `_tWinMain` | `lpCmdLine` 에 `-smoke` 가 있으면 `SmokeBegin(L"smoke.log")`. 루프에서 `SmokeTick()` 결과가 「끝」이면 빠져나와 기존 정리 후 `SmokeEnd()` 값을 반환. 30초 넘으면 `FAIL timeout` |
| `SceneMgr.cpp` `InitScene` | `SmokeCheck(SUCCEEDED(hr), L"scene %d init", ID)` — 모든 전환이 지나는 한 곳(`DebugOut` 옆) |
| `MenuButton.cpp` | 「시작」 버튼이 메뉴에 들어온 뒤 1초가 지나면 마우스 없이 `SCENEID_STAGE` 반환 |
| `GameSound.hpp` `LoadSound`·`Play`·`_errcheck` | 로드·재생 결과를 `SmokeCheck`. `_errcheck` 는 메시지박스 대신 `FAIL fmod` |
| `StageBackground.cpp` `InitTileFromFile` | 읽은 타일 수로 `SmokeCheck(n == 800, L"map tiles=%d", n)` |
| `Stage.cpp` 진입 후 | 몬스터 리스트 크기로 `SmokeCheck(n == 5, L"monsters=%d", n)` |
| `Player.cpp` | 스테이지 진입 2초 뒤 한 번: `BoneSpear` 레벨을 1로 두고 `SkillActive(BONESPEAR)` → `AddObject` 결과를 `SmokeCheck` |
| `Headers/Function.h` `ErrMsgBox` | `IsSmokeMode()` 면 `MessageBox` 대신 `SmokeCheck(false, L"err %s", msg)` |

- **진행 시간표**(`SmokeTick` 이 경과 시간으로 판단): 로고 1초(기존) → 메뉴 1초 → 스테이지 진입 → 2초 뒤 스킬 → 2초 뒤 종료. 정상이면 약 7초.
  고정 프레임이 아니라 시간 기준 — 느린 PC 에서도 같은 순서.
- **스킬은 함수 직접 호출**(키 흉내 기각) — 입력 장치·포커스와 무관하게 결정적. 시작 시 스킬 레벨이 0 이라
  스모크 모드에서만 `BoneSpear` 레벨을 1 로 둔다. 확인 대상은 「스킬 객체가 생성되는가」.
- **분기는 확인 지점에 한 줄씩** — 진행 로직은 `SmokeTick`(Client 전용, `Client.cpp` 안 정적 함수)에 모으고, 각 파일에는
  `if (IsSmokeMode())` 한두 줄만 둔다(SRP: 기록은 SmokeLog, 진행은 WinMain 루프, 확인은 해당 지점).
- 기대값(타일 800, 몬스터 5)은 현재 데이터·코드 기준 상수로 확인 지점에 둔다. 데이터가 바뀌면 그때 함께 고친다(YAGNI — 설정 파일 없음).

### MFC_000 — 타일 파일만 읽고 종료

- `MFC_000.cpp` `InitInstance` 맨 앞에서 `__wargv` 에 `-smoke <경로>` 가 있으면: `SmokeBegin(L"smoke.log")` →
  `Page2.cpp` 의 읽기 루프를 뽑아낸 `ReadTileFile(경로, vector<TILE*>&)` 로 읽기 → `SmokeCheck(n == 800, ...)` → 타일 해제 →
  `SmokeEnd()` 값을 종료 코드로 남기고 `FALSE` 반환(창·문서 생성 전). `ParseCommandLine` 에 넘기지 않아 경로가 「문서 열기」로 오해되지 않는다.
- `CPage2` 「타일 불러오기」 버튼도 같은 `ReadTileFile` 을 쓴다 — 버튼과 스모크가 같은 코드를 검증한다(DRY).

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| 확인 지점이 9개 파일에 흩어진다 | SRP·결합도 | 수용. 각 지점은 `IsSmokeMode()` 한두 줄이고 로직은 `SmokeLog.h`·`SmokeTick` 에 모은다. 확인 대상이 원래 그 파일들에 있다 |
| 스모크 모드에서 스킬 레벨을 바꾼다(게임 상태 개입) | 「`-smoke` 없으면 동작 동일」 | 스모크 모드에서만. R4 로 일반 실행 불변 확인 |
| `GameSound.hpp` 는 스타일이 다른 외부 유래 코드(한 줄 압축) | 기존 관례 | 최소 변경 — `_errcheck`·`LoadSound`·`Play` 에 분기만 추가, 서식 정리 안 함 |
| 로그 「재생 성공」은 실제 소리가 들림을 보증하지 않는다. 화면 표시도 마찬가지 | 검증 범위 | 수용(intent 범위 밖). 지각 확인은 필요할 때만 사람이 |
| 기대값 하드코딩(타일 800·몬스터 5) | 유연성 | 수용(YAGNI). 데이터·코드가 바뀌면 같은 커밋에서 갱신 |
| `ErrMsgBox`(공용)가 `SmokeLog.h` 에 의존 → include 순서 | 의존 방향 | `Function.h` 가 `SmokeLog.h` 를 include. `SmokeLog.h` 는 다른 프로젝트 헤더에 의존하지 않는다 |
| MFC `InitInstance` 에서 `FALSE` 반환 시 종료 코드 전달 | 동작 확실성 | plan 1단계에서 확인. 안 되면 `ExitProcess(code)` 로 즉시 종료 |
| 30초 타임아웃·시간표가 느린 환경에서 빠듯할 수 있다 | 안정성 | 정상 약 7초 대비 4배 여유. 실패 시 로그에 어느 단계까지 갔는지 남는다 |
| x64 검증은 이 인텐트 범위 밖 | 범위 | `x64-migration` 브랜치가 master 를 받은 뒤 같은 명령으로 R4~R6 판정 |
