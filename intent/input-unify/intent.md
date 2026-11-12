# Intent: 입력 읽기를 한 경로로 통합

Author: Ryu Jahyeon
Status: draft

## Problem
Client 가 입력을 **두 경로**로 읽는다(오딧 [10번](../legacy-audit/review.md)). 경로는 `2D-Portfolio/` 기준.

- 키보드·마우스 버튼은 `CKeyMgr::CheckKey`(`KeyMgr.cpp`)와 `CMenuButton` 이 `GameAsyncKeyState`(= `GetAsyncKeyState` 래핑)로 읽는다.
  이미 `VK_LBUTTON`·`VK_RBUTTON` 도 `KEY_LBUTTON`·`KEY_RBUTTON` 비트로 읽고 있다.
- 플레이어 이동·스킬 시전의 마우스 버튼만 DirectInput(`CInputDevice`, [shared-render-lib](../shared-render-lib/intent.md) 에서 Engine 밖으로 옮겨 둔 것)으로 읽는다 — `Player.cpp` 4곳.
  같은 왼쪽·오른쪽 버튼을 두 API 로 따로 읽는 셈이다. DirectInput 키보드 상태(`GetKeyBoardState`)는 아무도 읽지 않는다.
- DirectInput 은 키보드·마우스 용도로 MS 가 권장하지 않는다. 그런데도 Client 는 이 경로 때문에 `dinput8.lib`·`dxguid.lib` 를 링크한다.
- 두 경로 모두 **게임 창이 비활성이어도 입력을 받는다**.
  - DirectInput 은 `DISCL_BACKGROUND` 로 열려 있다.
  - `GetAsyncKeyState` 는 시스템 전역 상태다.
  - 결과: 게임을 띄워 둔 채 다른 창에서 클릭하거나 키를 누르면 게임 캐릭터가 움직인다(CLAUDE.md 「실행」 주의, [smoke-input-isolation](../smoke-input-isolation/intent.md) 에서 넘김).

## Proposed outcome
- 입력은 `GameAsyncKeyState`·`GameCursorPos` 한 경로로만 읽는다. DirectInput(`CInputDevice`)과 그 링크 의존성이 Client 에서 사라진다.
- 일반 실행에서 게임 창이 비활성일 때는 키·마우스 버튼을 「안 눌림」으로 본다. 창이 활성일 때의 조작감은 그대로다.

## Affected users and systems
사용자:
- 게임 사용자: 다른 창 작업이 게임에 새지 않는다.
- 이 repo 개발자: 입력 경로가 하나로 줄어든다.

시스템: `Headers/Function.h`·`Include.h`, `Client/Player.cpp`·`MainGame.cpp`·`stdafx.h`·`InputDevice.h/.cpp`(삭제)·`Client.vcxproj(.filters)`, CLAUDE.md·ARCHITECTURE.md

## Constraints
반드시 지킬 것:
- 창이 활성일 때 입력 동작은 그대로다(같은 버튼 → 같은 동작).
- 스모크 동작과 결과는 그대로다.
- 경고 0. 4개 구성 빌드.

범위 밖:
- `CKeyMgr` 의 키 비트 구조·키 반복 처리 개선, 윈도 메시지 기반 입력으로 전환.
- 커서 좌표(`GameCursorPos`)의 비활성 처리 — 버튼이 안 눌리면 좌표만으로는 게임이 움직이지 않는다.
- 리소스 경로 → `resource-path`.

## Open questions
없음 — 오딧 권장(「한쪽으로 통일」)을 따른다. 이미 래핑돼 있는 `GetAsyncKeyState` 쪽으로 통일한다(spec 설계).
