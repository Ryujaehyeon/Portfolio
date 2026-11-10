# Plan: debug-dll-removal

Author: Ryu Jahyeon
Status: draft

<!-- 구현은 개발자(본인)가 직접 한다. 경로는 2D-Portfolio/ 기준. 소스는 UTF-8 BOM 유지(.editorconfig). -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Client/MenuD2.cpp:82-84` | `wsprintf` + `DebugMsg` 3줄 → `DebugLog` 1줄 (아래 2단계) |
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
   - Win32 Debug 빌드 → VS 에서 F5(작업 디렉터리 기본값 `$(ProjectDir)`) → 메뉴 화면까지 진입.
   - 디버그 콘솔 창에 `Scene : ` 줄이 나오는지, VS 출력 창에 나오는지 본다.
   - 기대: 둘 다 안 나옴(= DLL 호출이 무효). 만약 나온다면 어디에 나왔는지 PR 에 기록 — 2단계 이후 같은 곳/콘솔에 나오면 동등.
2. **`MenuD2.cpp` 교체**
   ```cpp
   // 전 (:82-84)
   TCHAR Info[128];
   wsprintf(Info, L"Scene : %d", iScene);
   DebugMsg(Info);
   // 후
   if(iScene > SCENEID_NONPASS)
       DebugLog(L"Scene : %d", iScene);
   ```
   - `iScene > SCENEID_NONPASS` 조건: 지금은 메뉴에서 **매 프레임** 보낸다. 콘솔에 찍히게 되면 다른 로그(`Player.cpp:98`)를 밀어내므로
     장면이 바뀔 때만 찍는다. 바로 위 `:80` 의 `InitScene` 조건과 같다. 매 프레임 보고 싶으면 조건을 빼도 된다.
   - `wsprintf` 두 줄을 지우는 것이 중요 — 매크로 밖이라 Release 에서도 매 프레임 실행되던 코드다.
   → 확인: Debug 실행 시 장면 전환 순간 콘솔에 `Scene : <번호>` 출력.
3. **링크·헤더 제거** — `Client.cpp:8` pragma, `Include.h:2`, `Headers/Debug.h`(`git rm`), `MyProgram.sln:11`.
   → 확인: `git grep -n -E "Debug\.h\b|Debug\.lib|DebugMsg|CreateConsole|DestroyConsole"` 결과가 문서(`intent/`)뿐.
4. **Post-Build 수정** — `Client.vcxproj` 의 복사 줄·메시지. x64 작업을 이미 했다면 `$(FmodDll)` 줄만 남는다.
5. **바이너리 삭제** — `git rm Client/Debug.lib Client/Debug.dll Client/Debug/Debug.dll`
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
- Debug: 콘솔 창이 뜨고(`CREATE_DEBUG_CONSOLE`), 스테이지에서 마우스·좌표 로그(`Player.cpp:98`)가 갱신되고,
  장면 전환 때 `Scene : <번호>` 가 찍힌다. 로고 → 메뉴 → 스테이지 정상.
- Release: 콘솔 창이 **뜨지 않고** 게임 정상.
- 선택(배포 검증): VS2010 이 없는 PC 나 Windows Sandbox 에 `Exe\Win32\Release\Client\` + `Resource/`·`Client/` 데이터를 복사해 실행 →
  `MSVCR100D.dll` 오류 없이 시작. (VC++ 2015-2022+ 재배포 패키지는 필요)

**x64 링크 확인**(x64 구성이 이미 있으면): `-p:Platform=x64` 빌드에서 LNK4272·`DebugMsgw` LNK2019 가 사라짐.

## 리스크

- **`DebugMsg` 가 사실은 어딘가에 출력하고 있었을 경우**: 1단계에서 확인. 출력이 있었다면 그 용도를 PR 에 적고, `DebugLog` 로 같은 정보가 나오는지 본다.
- **`Debug.hpp` 의 콘솔은 Debug 전용**: Release 에서 로그가 필요해지면 별도 설계(파일 로그 등)가 필요 — 범위 밖.
- **복원**: 바이너리는 git 이력에 남는다(`git show <커밋>^:2D-Portfolio/Client/Debug.dll`). 다만 소스가 없어 고칠 수 없다는 사실은 그대로.
