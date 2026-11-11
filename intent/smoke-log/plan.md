# Plan: smoke-log

Author: Ryu Jahyeon
Status: approved (사용자, 2026-11-10) · 구현·검증 완료(2026-11-11)

<!-- 입력: intent/smoke-log/spec.md (PR #16 머지로 승인, 2026-11-10). 경로는 2D-Portfolio/ 기준.
     승인되면 feature/smoke-log 브랜치에서 이 내용을 intent/smoke-log/plan.md 로 저장해 코드와 함께 커밋한다. -->

## Context

검증할 때마다 사람이 실행해 눈·귀로 확인해야 하는 문제를 없앤다. `Client.exe -smoke` 는 입력 없이 로고→메뉴→스테이지→스킬까지
진행하고 `smoke.log` 에 확인 결과를 남긴 뒤 종료 코드로 성패를 알린다. `MFC_000.exe -smoke <경로>` 는 `Map.dat` 를 읽어 타일 수를 확인한다.
보류 중인 `x64-migration` R4~R6 을 이것으로 자동 판정하는 것이 직접 목적.

## spec 대비 구체화 (조사 결과 반영 — spec.md 도 같은 PR 에서 갱신)

- **몬스터 수는 `== 5` 가 아니라 `>= 5`**: `CStage::Progress`(`Stage.cpp:1748`)가 매 프레임 `GetMonsterCnt() < 10` 이면 Skeleton 을 보충하고
  보스(`SUMMONER`)도 1마리 만든다. 초기 5마리 이후 수가 계속 변한다 → `GetMonsterCnt() >= 5`.
- **「모든 확인 통과」 판정 = 필수 항목이 PASS 로 한 번 이상 기록됨 + FAIL 0**: 단계가 아예 실행되지 않으면 FAIL 줄도 안 남으므로,
  `SmokeEnd(필수 접두어 목록)` 이 PASS 메시지 중 접두어가 맞는 것이 없으면 `FAIL missing <접두어>` 를 남긴다.
- **스킬은 한 번 호출이 아니라 「생성될 때까지 매 프레임」**: `CPlayer::SkillActive`(`Player.cpp:808-855`)는 `m_tFrame.fStart <= 0` 일 때만
  객체를 만든다. 스테이지 2초 뒤부터 `BoneSpear` 레벨 1 로 두고 생성 성공까지 매 프레임 호출, 성공하면 멈춘다.

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Headers/SmokeLog.h` (새, UTF-8 BOM) | `SmokeBegin/IsSmokeMode/SmokeLog/SmokeCheck/SmokeEnd`. 상태는 함수 안 `static` 구조체 하나. Win32 `CreateFile`/`WriteFile` + `WideCharToMultiByte` 로 UTF-8(BOM 없음), 줄마다 기록(버퍼 없음). 경과 시간 `GetTickCount64` |
| `Headers/Function.h` | `#include "SmokeLog.h"`. `ErrMsgBox`: 스모크면 `SmokeCheck(false, L"err %s", szMsg)` 하고 `MessageBox` 생략 |
| `MyProgram.sln` | Headers 솔루션 폴더 항목에 `Headers\SmokeLog.h` 추가(표시용) |
| `Client/Client.cpp` | `lpCmdLine` 에 `-smoke` 면 `SmokeBegin(L"smoke.log")`. 루프에서 `SmokeTick(iScene)`(같은 파일 정적 함수)이 true 면 break. 반환값: 스모크면 `SmokeEnd(필수 목록)`, 아니면 기존 `msg.wParam` |
| `Client/SceneMgr.cpp` `InitScene` | 결과를 `SmokeCheck(SUCCEEDED(hr), L"scene %d init", ID)` — 반환 경로를 한 변수로 모아 기록 |
| `Client/MenuButton.cpp` | 스모크면 「시작」 버튼이 함수 안 `static` 누적 시간 1초 뒤 `SCENEID_STAGE` 반환(마우스 무시) |
| `Client/GameSound.hpp` | `LoadSound` 결과 `SmokeCheck(ok, L"sound load %s")`, `Play` 끝에 `SmokeCheck(L"sound play %s")`, `_errcheck` 는 스모크면 `MessageBox` 대신 `SmokeCheck(false, L"fmod error %d")`. 서식 정리 안 함 |
| `Client/StageBackground.cpp` `InitTileFromFile` | 읽은 수로 `SmokeCheck(n == 800, L"map tiles=%d", n)` |
| `Client/Player.cpp` | `Progress` 에 스모크 분기: 스테이지 2초 뒤부터 스킬 생성 전까지 `BoneSpear` 레벨 1 + `SkillActive(BONESPEAR)`. `SkillActive` 의 BoneSpear `AddObject` 결과를 `SmokeCheck(..., L"skill BoneSpear")` |
| `MFC_000/Page2.cpp`·`Page2.h` | 「타일 불러오기」 읽기 루프를 자유 함수 `int ReadTileFile(const TCHAR* _pPath, vector<TILE*>& _rTiles)`(열기 실패 -1)로 추출, 버튼은 그 함수를 호출 |
| `MFC_000/MFC_000.cpp` | `InitInstance` 에서 `CWinAppEx::InitInstance()` 직후 `__argc >= 3 && -smoke` 면 `ReadTileFile(__wargv[2])` → `SmokeCheck(n == 800)` → 타일 해제 → 종료 코드 저장 → `return FALSE`. `ExitInstance` 는 스모크면 저장한 코드 반환 |
| `CLAUDE.md` | 「테스트」 줄: 스모크 명령·기대 출력·`smoke.log` 위치 |
| `intent/smoke-log/spec.md` | 위 「spec 대비 구체화」 3건 반영 |
| `intent/smoke-log/plan.md` | 이 계획 + 검증 결과 |

필수 목록(Client): `scene 0 init`, `scene 1 init`, `scene 2 init`, `sound load Wilderness`, `sound play Wilderness`,
`sound load Tristram`, `sound play Tristram`, `map tiles`, `monsters`, `skill BoneSpear`. (툴): `map tiles`.

## 작업 순서

0. `master`(`ea11455`)에서 `feature/smoke-log`. 기준선: Win32 Debug·Release 경고 198(이번 세션 측정값).
1. **`SmokeLog.h` + `Function.h`·`ErrMsgBox` 분기** → 빌드 오류 0·경고 198. `-smoke` 없이 8초 실행 → `smoke.log` 없음(R4 일부).
2. **Client 진입·종료**(`Client.cpp` + `SmokeTick` + `SceneMgr`) → `-smoke` 실행: 로고→메뉴에서 멈춰 30초 타임아웃,
   로그에 `scene 0/1 init PASS` + `FAIL timeout`, 종료 코드 ≠ 0. (진행 골격 확인)
3. **메뉴 건너뛰기**(`MenuButton.cpp`) → `scene 2 init PASS` 까지, 스테이지 진입 4초 뒤 정상 종료.
4. **확인 지점**(`GameSound.hpp`·`StageBackground.cpp`·`Player.cpp`, `SmokeTick` 의 몬스터) → `RESULT PASS`, 종료 코드 0, 약 7초.
5. **툴**(`Page2` 추출 + `MFC_000.cpp`) → `MFC_000.exe -smoke ../Client/Map.dat` 종료 코드 0·`map tiles=800 PASS`,
   `-smoke 없는경로` → ≠ 0. 툴을 그냥 실행해 창이 뜨는지(회귀).
6. **실패 경로(R3)**: 로컬에서 `Stage.cpp:406` 경로를 잠시 틀리게 → `-smoke` → 종료 코드 ≠ 0, `FAIL err Player Stand D 로드 실패`,
   메시지박스 없이 30초 안 종료 → 되돌림(커밋 안 함, `git diff` 확인).
7. **전 구성 검증**: Win32 Debug·Release Rebuild(`-m:1 -nodeReuse:false`) 경고 198, 두 구성에서 2·5단계 명령 반복(R6).
8. **문서**: CLAUDE.md 「테스트」, spec 갱신, plan.md 검증 결과 → 커밋 `feat(smoke-log): -smoke 모드와 smoke.log 로 자동 스모크 검증` → PR → 머지 → 브랜치 삭제(단계별).

## 테스트

```
# 빌드 (Win32 두 구성, 경고 판정용)
MSBuild.exe 2D-Portfolio/MyProgram.sln -t:Rebuild -p:Configuration=<Debug|Release> -p:Platform=Win32 -m:1 -nodeReuse:false -nologo -v:m
# Client 스모크 (작업 디렉터리 Client/)
cd 2D-Portfolio/Client && timeout 60 ../Exe/Win32/<구성>/Client/Client.exe -smoke; echo $?   # 기대 0
grep -c PASS smoke.log; grep FAIL smoke.log; tail -1 smoke.log                             # FAIL 없음, "RESULT PASS"
# 툴 스모크 (작업 디렉터리 MFC_000/)
cd 2D-Portfolio/MFC_000 && ../Exe/Win32/<구성>/MFC_000/MFC_000.exe -smoke ../Client/Map.dat; echo $?   # 기대 0
# 일반 실행 불변
rm -f smoke.log; (8초 실행) ; test ! -e smoke.log
```
WSL 에서 Windows exe 의 종료 코드는 그대로 전달된다. 실행 확인에는 이번 세션의 창 목록 스크립트도 쓴다(`system err` 없음).

| spec | 확인 단계 |
|---|---|
| R1 자동 진행·종료 코드 | 4, 7 |
| R2 로그 PASS 항목 | 4 |
| R3 실패 시 FAIL·무정지 | 2(타임아웃), 6 |
| R4 일반 실행 불변 | 1, 7 |
| R5 툴 스모크 | 5 |
| R6 Win32 두 구성 | 7 |
| R7 새 경고 0 | 1, 7 |
| R8 CLAUDE.md | 8 |

## 리스크

- **WinMain 루프를 break 로 빠져나올 때 정리 순서**: 기존 `SCENEID_END` 종료와 같은 경로(`Setter->Destroy`·`Sound->Close`)를 타게 한다. 크래시하면 2단계에서 드러남.
- **`MenuButton`·`Player` 의 함수 안 `static` 타이머**: 장면을 다시 만들면 초기화되지 않는다 — 스모크는 한 번만 지나가므로 수용.
  일반 실행 경로에서는 접근하지 않는다.
- **`SkillActive` 가 계속 실패**(프레임 조건): 4초 안에 생성이 없으면 `FAIL missing skill BoneSpear` — 원인 조사 후 호출 조건 조정.
- **`GameSound.hpp` 의 `_errcheck` 가 `private`·한 줄 압축 서식**: 분기만 끼워 넣고 서식은 그대로.
- **MFC `InitInstance` FALSE → `ExitInstance` 반환값이 종료 코드가 되는지**: 5단계에서 `echo $?` 로 확인. 안 되면 `ExitProcess(code)`.
- **`__wargv` 를 쓰려면 유니코드 CRT 진입점**: MFC_000 은 Unicode 라 `__wargv` 사용 가능. 아니면 `CommandLineToArgvW(GetCommandLineW())`.
- **스모크 중 FMOD 실패가 `_errcheck` 밖(`Play` 가 반환값 없음)**: `Play` 는 마지막 `setPaused` 결과로 판단.

## 구현 중 변경 (spec.md 는 승인본 그대로 두고 여기에 기록)

| 무엇 | 이유 | 결정 |
|---|---|---|
| 스킬 시전을 `Progress` 앞부분이 아니라 우클릭 시전과 같은 자리(목표 도달 → `STAND` 설정 뒤)에서 | 앞에서 부르면 같은 프레임에 `STAND` 로 덮여 `CAST` 프레임이 시작되지 않는다 | 구체화 |
| **범위 확장**: 종료 크래시 2건 수정 — `CPlayer` 생성자 2개·`CSceneObj` 생성자에서 `m_pSkillPrototype = nullptr` | 스테이지 해제 시 초기화 안 된 포인터를 `SAFE_DELETE` 해 접근 위반(`C0000005`). 스모크와 무관한 기존 버그로, 스테이지에서 게임을 끝내면 항상 발생(cdb 스택으로 확인: `CPlayer::Release` → `Player.cpp:737`, `CStage::Release` → `Stage.cpp:1786`) | 사용자 결정 — 이 2건만, 더 나오면 `objmgr-lifetime` |
| **알려진 실패**: Release 에서 `missing skill BoneSpear` | `CObj::FrameMove`(`Obj.cpp:59-75`)가 Debug 는 `fStart = 0` 리셋, Release 는 `fStart -= fMax` → 생성 조건 `fStart <= 0`(`Player.cpp` `SkillActive`)이 Release 에서 거의 성립하지 않는다. **실제 게임에서도 Release 스킬이 나가지 않는 기존 버그** | 사용자 결정 — 별도 인텐트(`ub-fixes` 에 포함). 이 인텐트에서는 고치지 않음 |
| `Function.h`·`SceneObj.cpp` 에 UTF-8 BOM 추가 | 한글 주석이 들어가 BOM 이 필요(CLAUDE.md 인코딩 규칙). `StageBackground.cpp`·`GameSound.hpp` 는 ASCII 만 추가해 그대로 | 관례 |

## 검증 결과 (2026-11-11)

| spec | 결과 |
|---|---|
| R1 자동 진행·종료 코드 | ✅ Debug: `Client.exe -smoke` 3회 모두 종료 코드 0, 약 8.6초 |
| R2 로그 PASS 항목 | ✅ Debug: `scene 0/1/2 init`, `sound load/play Wilderness·Tristram`, `map tiles=800`, `monsters=10`, `skill BoneSpear`, `RESULT PASS (fail=0)` |
| R3 실패 시 FAIL·무정지 | ✅ 진행 골격 단계에서 메뉴 정지 → `FAIL timeout (30 s)` + `missing` 목록, 종료 코드 1. `Stage.cpp:406` 경로 일시 변경 → `FAIL err Player Stand D 로드 실패`·`FAIL scene 2 init`, 메시지박스 없이 7초에 종료 코드 1(변경은 되돌림) |
| R4 일반 실행 불변 | ✅ Debug·Release 8초 실행 창 응답(`system err` 없음), `smoke.log` 생기지 않음. 툴 일반 실행도 창 뜸·로그 없음 |
| R5 툴 스모크 | ✅ Debug·Release `MFC_000.exe -smoke ../Client/Map.dat` → 0·`map tiles=800`, 없는 경로 → 1·`map tiles=-1` |
| R6 Win32 두 구성 | ⚠️ Debug 전부 통과. Release 는 `missing skill BoneSpear` 1건 실패 — 위 「알려진 실패」(기존 버그를 스모크가 잡은 것), 나머지 항목·툴은 통과 |
| R7 새 경고 0 | ✅ Win32 Debug·Release Rebuild(`-m:1 -nodeReuse:false`) 모두 198, 코드별 수 불변 |
| R8 CLAUDE.md | ✅ 「테스트」 줄 갱신 |
