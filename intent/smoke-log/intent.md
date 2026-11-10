# Intent: 자동 검증용 스모크 모드와 로그

Author: Ryu Jahyeon
Status: draft

## Problem
변경을 검증할 때마다 **사람이 직접 실행해 눈·귀로 확인**해야 한다. CLAUDE.md 도 「테스트: 없음 — 실행해서 눈으로 확인」이다.

- 에이전트가 자동으로 할 수 있는 것은 "몇 초 살아 있고 오류 메시지박스가 없다"까지다. 메뉴 「시작」은
  `MenuButton.cpp:45-52` 가 `GetAsyncKeyState(VK_LBUTTON)` 로 실제 마우스 상태를 읽어, 메시지로는 누를 수 없다.
  그래서 스테이지·배경음·맵 타일·몬스터·스킬은 매번 사람이 확인한다
  (예: `x64-migration` R4~R6, `debug-dll-removal` R4, `err-msg-format` R6).
- 성공해도 흔적이 남지 않는 곳이 있다. `GameSound.hpp` 는 FMOD 실패만 메시지박스로 알리고,
  `StageBackground.cpp:21-41` 는 `Map.dat` 열기에 실패해도 조용히 타일 0개로 진행한다.
- 실패 시에는 `ERR_MSG`·FMOD 오류가 **모달 메시지박스**를 띄워 실행이 멈춘다 — 무인 실행에서는 끝나지 않는다.
- 맵 툴(MFC_000)은 「타일 불러오기」 파일 대화상자(`Page2.cpp:55` 등)를 거쳐야 `Map.dat` 를 연다.

## Proposed outcome
- `Client.exe -smoke` 로 실행하면 사람 입력 없이 로고 → 메뉴 → 스테이지 진입 → 스킬 1회 사용까지 진행하고 스스로 종료한다.
- 그동안 확인할 사건(장면 전환, 배경음 로드·재생 결과, `Map.dat` 타일 수, 몬스터 생성, 스킬 사용, 오류)을
  작업 디렉터리의 로그 파일(`smoke.log`)에 남기고, 모두 통과하면 종료 코드 0, 아니면 0 이 아닌 값으로 끝난다.
- 스모크 모드에서는 오류 메시지박스 대신 로그에 남겨, 무인 실행이 멈추지 않는다.
- Debug·Release, Win32·x64 모두에서 같은 방식으로 동작한다.
- 맵 툴도 `MFC_000.exe -smoke <경로>` 로 대화상자 없이 `Map.dat` 를 읽고 타일 수를 남긴 뒤 종료한다.
- 그 결과, 보류 중인 `x64-migration` R4~R6 을 사람 확인 없이 판정할 수 있다.

## Affected users and systems
사용자: 이 repo 개발자, 검증을 맡는 에이전트(Claude 등)
시스템: `Client/Client.cpp`(명령줄·종료 코드), `Client/MenuD2.cpp`·`MenuButton.cpp`(메뉴 건너뛰기), `Client/Stage.cpp`·
`StageBackground.cpp`·`Player.cpp`·`ObjMgr.cpp`(사건 기록), `Client/GameSound.hpp`(소리 결과), `Headers/`(로그·`ErrMsgBox` 분기),
`MFC_000/`(툴 스모크), CLAUDE.md 「빌드 / 테스트 / 린트」, `.gitignore`(`*.log` 로 이미 제외)

## Constraints
반드시 지킬 것: `-smoke` 없이 실행하면 동작이 지금과 같다(로그 파일도 안 생긴다). `.dat`·`Resource/` 는 건드리지 않는다.
Debug 콘솔·`DebugOut` 등 기존 디버그 기능은 그대로. 새 코드는 CLAUDE.md 「C++ 코딩 표준」 품질 규칙을 따른다.
범위 밖: 소리가 실제로 들리는지·화면에 제대로 그려지는지의 지각적 확인(로그는 "로드·재생 성공"까지),
Stage2·Stage3 진행, 일반 게임 로그 체계, 단위 테스트 프레임워크 도입, 발견된 버그 수정(각 인텐트로).

## Open questions
- 스모크 진행 순서·대기 시간과 각 단계의 통과 조건(예: 스테이지 진입 후 몇 프레임, 몬스터 몇 마리 이상) — spec 에서.
- 스킬 1회 사용을 어떤 경로로 일으킬지(키 입력 흉내 vs 플레이어 함수 직접 호출) — spec 에서.
- 로그 기록 위치: 공용 헤더(`Headers/`)에 둘지 Client 전용으로 둘지 — 툴과 공유 여부에 따라 spec 에서.
