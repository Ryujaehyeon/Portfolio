# Intent: 소스 없는 Debug.dll 의존 제거

Author: Ryu Jahyeon
Status: draft

## Problem
Client 가 소스 없는 2016년 바이너리 `Debug.lib`/`Debug.dll` 에 링크돼 있다. 근거: [legacy-audit/review.md](../legacy-audit/review.md) 8번.

- **배포 차단**: `Debug.dll` 이 VS2010 디버그 CRT `MSVCR100D.dll` 에 링크돼 있다. 디버그 CRT 는 재배포 불가이고
  VS2010 이 없는 PC 에 없으므로, 그런 PC 에서는 **Release 빌드조차 시작되지 않는다.**
- **x64 차단**: PE32(x86) 전용이라 x64 링크 불가(LNK4272) → [x64-migration](../x64-migration/intent.md) 의 선행 조건.
- **Release 에도 딸려 감**: `Client/Client.cpp:8` `#pragma comment(lib, "Debug.lib")` 이 조건 없이 링크된다.
- **사실상 하는 일이 없음**: 쓰는 곳은 `Client/MenuD2.cpp:84` `DebugMsg(Info)` 1곳. DLL 이 따로 관리하는 콘솔에 쓰는데
  그 콘솔을 만드는 `CreateConsole` 호출처가 0 이다(추정 — 「plan」 1단계에서 확인).

한편 같은 기능(콘솔 로그)은 이미 repo 안의 헤더 전용 `Headers/Debug.hpp` 가 한다 — `Client.cpp:58` `CREATE_DEBUG_CONSOLE`,
`Player.cpp:98` `DebugLog`. 이것은 `#ifdef _DEBUG` 로 감싸여 Release 에서 빈 매크로가 된다(`Debug.hpp:132-140`).

## Proposed outcome
- Client·MFC_000 이 `Debug.lib`/`Debug.dll`/`Debug.h` 없이 빌드·실행된다.
- 디버그 로그는 `Debug.hpp` 하나로 통일되고, 메뉴 장면의 장면 번호 로그가 Debug 콘솔에 **실제로** 찍힌다.
- Release exe 의 의존 DLL 에 `Debug.dll`·`MSVCR100D.dll` 이 없다.

## Affected users and systems
사용자: 이 repo 개발자, 배포본을 실행하는 사람
시스템: `Client/Client.cpp`, `Client/MenuD2.cpp`, `Headers/Include.h`, `Headers/Debug.h`, `Client/Client.vcxproj`(Post-Build),
`MyProgram.sln`(솔루션 항목), `Client/Debug.lib`, `Client/Debug.dll`, `Client/Debug/Debug.dll`

## Constraints
반드시 지킬 것: 디버깅 기능(콘솔 생성·`DebugLog`·`DEBUG_ENTRY_EXIT`)은 그대로. Release 에 디버그 코드가 남지 않을 것.
범위 밖: `Debug.hpp` 자체의 개선(경고 C4267·`itoa` 등 → `warning-cleanup`), 다른 로그 체계 도입.

## Open questions
없음. (DLL 재작성 대신 `Debug.hpp` 를 쓰기로 2026-11-10 대화에서 결정.)
