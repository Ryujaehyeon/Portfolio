# Plan: debug-dll-removal

Author: Ryu Jahyeon
Status: approved (사용자, 2026-11-10) · 1차 구현 커밋 `45682f8` · B안 구현 완료(2026-11-10)

<!-- 구현은 개발자(본인)가 직접 한다. 경로는 2D-Portfolio/ 기준. 소스는 UTF-8 BOM 유지(.editorconfig). -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Client/MenuD2.cpp:82-84` | `wsprintf` + `DebugMsg` 3줄 삭제 — 장면 로그는 두지 않는다 (아래 2단계) |
| `Client/SceneMgr.cpp:14` `InitScene` | 맨 앞에 `DebugOut(L"Scene : %d", ID);` (2단계, B안) |
| `Headers/Debug.hpp:130`·`:140` 부근 | `DebugOut` 매크로 추가 — `_DEBUG` 블록과 `#else` 블록 각각 (2단계, B안) |
| `Client/Client.cpp:8` | `#pragma comment(lib, "Debug.lib")` 삭제 |
| `Headers/Include.h:2` | `#include "Debug.h"` 삭제 (`Debug.hpp` include 는 `:3` 에 그대로) |
| `Headers/Debug.h` | 파일 삭제 — `DebugMsgw/a`·`CreateConsole`·`DestroyConsole` 선언과 `EXPORT`/`EXTERN` 매크로. 다른 사용처 0 |
| `MyProgram.sln:11` | 솔루션 항목 `Headers\Debug.h = Headers\Debug.h` 줄 삭제 |
| `Client/Client.vcxproj` | Post-Build `Command` 의 `copy ... Debug.dll ...` 줄 삭제, `Message` 에서 `Debug.dll` 문구 삭제 |
| `Client/Debug.lib`, `Client/Debug.dll`, `Client/Debug/Debug.dll` | `git rm` |
| `CLAUDE.md` | 빌드 섹션의 「Client 런타임 DLL(`Debug.dll`, `fmodexL.dll`)」 → `fmodexL.dll` 만. 「건드리면 안 되는 부분」의 DLL 목록 갱신 |

참조 조사(2026-11-10): `Debug.h` 를 include 하는 곳은 `Include.h:2` 하나. `DebugMsg`·`CreateConsole`·`DestroyConsole`·`EXPORT`·`EXTERN`
사용처는 `MenuD2.cpp:84` 하나뿐. `Include.h` 는 MFC_000 도 include 하므로 MFC_000 의 C4005(`EXPORT` 재정의) 경고도 함께 사라진다.

## 작업 순서

0. 브랜치: `git switch master && git pull && git switch -c feature/debug-dll-removal`
   - 작업 트리에 x64 작업(sln·vcxproj 미커밋 변경)이 있으면 먼저 커밋해 두거나 별도 브랜치로 옮긴다 — 두 작업이 같은 파일을 건드린다.
1. **현재 동작 기록(변경 전)** — `DebugMsg` 가 정말 아무것도 안 하는지 확인.
   - **결과(2026-11-10, 완료)**: 출력 없음 확인. Win32 Debug 를 `Client/` 에서 실행 → 1초 후 메뉴(`Ball.cpp:40`) → 8초 시점에
     32비트 PowerShell 로 게임 콘솔에 `AttachConsole` 해 화면 버퍼(120×9001)를 읽음 → **비어 있음, `Scene` 0줄**.
     실행 중 새로 생긴 파일은 `Client/fmod.log`(FMOD 로깅판, gitignore)뿐. `Debug.dll` 에 로그 파일명 문자열·`OutputDebugString` import 없음.
     → 교체 후 `Scene : <번호>` 가 콘솔에 나오면 기능이 **늘어난** 것(회귀 아님).
   - Win32 Debug 빌드 → VS 에서 F5(작업 디렉터리 기본값 `$(ProjectDir)`) → 메뉴 화면까지 진입.
   - 디버그 콘솔 창에 `Scene : ` 줄이 나오는지, VS 출력 창에 나오는지 본다.
   - 기대: 둘 다 안 나옴(= DLL 호출이 무효). 만약 나온다면 어디에 나왔는지 PR 에 기록 — 2단계 이후 같은 곳/콘솔에 나오면 동등.
2. **장면 로그 — B안(VS 출력 창, `InitScene` 한 곳)** (2026-11-10 변경. 1차 구현은 `MenuD2` 안 `DebugLog` 였다)
   - **2-a. `Headers/Debug.hpp` 에 `DebugOut` 추가**
     ```cpp
     // _DEBUG 블록 — :130 `#endif`(_UNICODE 분기 끝) 다음 줄
     #define DebugOut(fmt, ...) do { wchar_t dbgOutBuf_[256]; \
         _snwprintf_s(dbgOutBuf_, _TRUNCATE, fmt, __VA_ARGS__); \
         OutputDebugStringW(dbgOutBuf_); OutputDebugStringW(L"\n"); } while (0)

     // #else 블록 — :140 `#define DebugLog(str, ...)` 다음 줄
     #define DebugOut(fmt, ...)
     ```
     - `_snwprintf_s(…, _TRUNCATE, …)`: 256자를 넘으면 잘라낸다. `swprintf_s` 는 넘치면 Debug CRT 가 프로그램을 멈춘다.
     - `do { … } while (0)`: `if (x) DebugOut(...); else …` 에서도 한 문장처럼 동작하게.
     - `<stdio.h>`·`<Windows.h>` 는 `Debug.hpp` 가 이미 include 한다.
   - **2-b. `Client/SceneMgr.cpp` `InitScene` 맨 앞**: `DebugOut(L"Scene : %d", ID);`
   - **2-c. `Client/MenuD2.cpp`**: 장면 로그 코드 없음 — `wsprintf` 두 줄·`DebugMsg`(원래)와 `DebugLog` 블록(1차 구현) 모두 삭제.
   - **진행 상황(2026-11-10)**: 2-a·2-b·2-c 완료.
   → 확인: VS 에서 F5 → 출력 창(「디버그」)에 `Scene : 0`, `Scene : 1` 이 바로 찍히고, 「시작」을 누르면 `Scene : 2`.
     콘솔 창에는 장면 로그가 없고 스테이지에서 좌표 로그만 갱신된다.
     `git grep -n "Scene : " -- 2D-Portfolio` → `SceneMgr.cpp` 1건.
3. **링크·헤더 제거** — `Client.cpp:8` pragma, `Include.h:2`, `Headers/Debug.h`(`git rm`), `MyProgram.sln:11`.
   → 확인: `git grep -n -E "Debug\.h\b|Debug\.lib|DebugMsg|CreateConsole|DestroyConsole"` 결과가 문서(`intent/`)뿐.
4. **Post-Build 수정** — `Client.vcxproj` 의 복사 줄·메시지. x64 작업을 이미 했다면 `$(FmodDll)` 줄만 남는다.
5. **바이너리 삭제** — `git rm Client/Debug.lib Client/Debug.dll` (`Client/Debug/Debug.dll` 은 repo-cleanup `7c94bb0` 에서 삭제됨)
   - `Client/Debug/Debug.dll` 은 [repo-cleanup](../repo-cleanup/plan.md) 에도 있다. 먼저 머지되는 쪽이 지우고, 나중 쪽은 그 줄을 뺀다.
6. **CLAUDE.md 갱신** (위 표).
7. **커밋** — `refactor(debug-dll-removal): 소스 없는 Debug.dll 의존 제거, 디버그 로그를 Debug.hpp 로 통일` → PR.

## 테스트

```
rm -rf 2D-Portfolio/Exe 2D-Portfolio/Temp
MSBuild.exe 2D-Portfolio/MyProgram.sln -t:restore -p:RestorePackagesConfig=true -nologo -v:m
MSBuild.exe 2D-Portfolio/MyProgram.sln -p:Configuration=Debug   -p:Platform=Win32 -m -nologo -v:m
MSBuild.exe 2D-Portfolio/MyProgram.sln -p:Configuration=Release -p:Platform=Win32 -m -nologo -v:m
```
기대: 종료 코드 0. 경고는 C4005 1건 감소(MFC_000). 링크 경고·오류 없음.

**의존 DLL 확인** (VS 개발자 명령 프롬프트):
```
dumpbin /dependents 2D-Portfolio\Exe\Win32\Release\Client\Client.exe
dumpbin /dependents 2D-Portfolio\Exe\Win32\Debug\Client\Client.exe
```
기대: 목록에 `Debug.dll` 없음. `MSVCR100D.dll` 은 원래 `Debug.dll` 을 거쳐서만 들어오므로 체인에서도 사라짐.
`Exe\...\Client\` 폴더에 `Debug.dll` 이 복사되지 않음.

**실행** (작업 디렉터리 `2D-Portfolio/Client/`):
- Debug(VS F5): 콘솔 창이 뜨고(`CREATE_DEBUG_CONSOLE`), 스테이지에서 마우스·좌표 로그(`Player.cpp:98`)가 갱신된다.
  **VS 출력 창**에 `Scene : 0` → `Scene : 1` → (「시작」) `Scene : 2`. 로고 → 메뉴 → 스테이지 정상.
  VS 없이 확인하려면 Sysinternals DebugView 로 `OutputDebugString` 을 본다.
- Release: 콘솔 창이 **뜨지 않고** 게임 정상.
- 선택(배포 검증): VS2010 이 없는 PC 나 Windows Sandbox 에 `Exe\Win32\Release\Client\` + `Resource/`·`Client/` 데이터를 복사해 실행 →
  `MSVCR100D.dll` 오류 없이 시작. (VC++ 2015-2022+ 재배포 패키지는 필요)

**x64 링크 확인**(x64 구성이 이미 있으면): `-p:Platform=x64` 빌드에서 LNK4272·`DebugMsgw` LNK2019 가 사라짐.

## 검증 결과 (2026-11-10, 1차 구현 `45682f8` 기준 — R4·R5·R9·R10 은 B안 반영 후 재검증)

| ID | 결과 |
|---|---|
| R1 참조 0 | ✅ `git grep` 결과 `intent/`·`docs/` 밖 0건 |
| R2 파일 제거 | ✅ `Debug.lib`·`Debug.dll`·`Headers/Debug.h` 삭제 (`Client/Debug/Debug.dll` 은 repo-cleanup 에서) |
| R3 의존 DLL | ✅ `dumpbin /dependents` Debug·Release 모두 `Debug.dll`·`MSVCR100D.dll` 없음 (CRT 는 `VCRUNTIME140(D)`·`ucrtbase(d)`) |
| R4 Debug 콘솔·로그 | ✅ B안: `DBWIN_BUFFER` 리스너(DebugView 와 같은 방식)로 Debug `Client.exe` 를 8초 실행 → `Scene : 0`, `Scene : 1` 수신, 창 응답. 「시작」 클릭 후 `Scene : 2` 는 사용자가 VS 에서 확인(2026-11-10) |
| R5 Release 무콘솔 | ✅ `AttachConsole` 실패(콘솔 없음), 창 응답 |
| R6 C4005 제거 | ✅ 0건, 고유 경고 283 → 282 |
| R7 빌드·실행 | ✅ `Exe/`·`Temp/` 삭제 후 Debug·Release 종료 코드 0, 두 구성 창 응답·`fmodexL.dll` 로드 |
| R8 x64 링크 | — x64 구성은 아직 WIP 브랜치 (x64-migration 에서 확인) |
| R9 `DebugOut` | ✅ `Headers/Debug.hpp` `_DEBUG` 블록(`_snwprintf_s` + `_TRUNCATE` → `OutputDebugStringW`, `do { } while (0)`)과 `#else` 빈 매크로. Debug 수신은 R4, Release 는 같은 리스너로 6초 실행해 출력 0줄·창(`Client`) 응답 |
| R10 로그 한 곳 | ✅ `git grep -n "Scene : "` → `Client/SceneMgr.cpp:16` 1건 |
| 경고 (B안 후) | ✅ Debug `/W3` Rebuild 282건(기준과 같음), Release 빌드 종료 코드 0·오류 0 |

**R4 관찰 불가 이유**: 로그는 `iScene > SCENEID_NONPASS` 일 때만 찍히는데, 메뉴에서 그 경우는 「시작」으로 스테이지에 들어가는 순간뿐이다.
스테이지에서는 `CPlayer::Progress()` 가 매 프레임 `DebugLogClear`(`Player.cpp:97`)로 콘솔을 지우므로 `Scene : 2` 는 다음 프레임에 사라진다.
변경 전에는 아무 출력도 없었으므로 회귀는 아니다. 장면 전환 로그를 실제로 보려면 콘솔 대신 VS 출력 창(`OutputDebugString`) 쪽으로
보내는 매크로가 필요하다 → **사용자 결정: B안**(2단계에 반영). 로고→메뉴(1)가 안 찍히던 것도 로그 위치를 `InitScene` 으로 옮겨 해결.

## 리스크

- **`DebugMsg` 가 사실은 어딘가에 출력하고 있었을 경우**: 1단계에서 확인. 출력이 있었다면 그 용도를 PR 에 적고, `DebugLog` 로 같은 정보가 나오는지 본다.
- **`Debug.hpp` 의 콘솔은 Debug 전용**: Release 에서 로그가 필요해지면 별도 설계(파일 로그 등)가 필요 — 범위 밖.
- **복원**: 바이너리는 git 이력에 남는다(`git show <커밋>^:2D-Portfolio/Client/Debug.dll`). 다만 소스가 없어 고칠 수 없다는 사실은 그대로.
