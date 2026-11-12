# Spec: input-unify

Author: Ryu Jahyeon
Status: draft

<!-- 입력: intent.md. 경로는 2D-Portfolio/ 기준. -->

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | `Player.cpp` 의 마우스 버튼 판정 4곳이 `m_dwKey & KEY_LBUTTON`/`KEY_RBUTTON` 을 쓴다(`rgbButtons[0]` = 왼쪽, `[1]` = 오른쪽). `CInputDevice::SetKey` 호출은 지운다 | 코드 검토 |
| R2 | `CInputDevice`(`InputDevice.h/.cpp`)를 지운다. `MainGame` 의 초기화·해제 호출, `Include.h` 의 포함, vcxproj·filters 항목도 지운다 | `git grep CInputDevice` 결과 없음 |
| R3 | Client 가 DirectInput 에 의존하지 않는다 — `stdafx.h` 의 `<dinput.h>`·`DIRECTINPUT_VERSION`, 링크의 `dinput8.lib` 를 지운다. `dxguid.lib` 는 DirectInput GUID 용이라 함께 지운다(링크가 실패하면 남긴다) | `git grep -i dinput` 결과 없음, 빌드 |
| R4 | `GameAsyncKeyState` 는 호출한 스레드에 활성 창이 없으면(`GetActiveWindow() == nullptr`) 0 을 돌려준다 — 게임 창이 비활성일 때 키·버튼이 안 눌린 것으로 보인다 | 코드 검토 + 수동 확인 |
| R5 | 창이 활성일 때의 동작은 그대로다 — 왼쪽 클릭 이동, 오른쪽 클릭 시전, 메뉴 버튼, 단축키 | 수동 확인(사용자) |
| R6 | 4개 구성 클린 빌드 경고 0·오류 0. Client 스모크(x64 Debug·Win32 Release 각 20회, 나머지 5회), 툴 스모크, 일반 실행 | CLAUDE.md |
| R7 | 문서 — CLAUDE.md 「실행」의 비활성 창 주의를 고친다. 싱글턴 목록에서 `CInputDevice` 를 뺀다. ARCHITECTURE.md 의 입력 설명도 고친다 | 문서 검토 |

## 설계

- **`GetAsyncKeyState` 쪽으로 통일한다.**
  - 키보드는 이미 전부 이 경로다. 마우스 버튼도 `CKeyMgr` 가 이미 읽고 있다 — 바꾸는 것은 `Player` 의 판정 4곳뿐이다(KISS·DRY).
  - DirectInput `rgbButtons` 와 `GetAsyncKeyState(VK_*BUTTON)` 은 둘 다 **물리** 버튼 상태다. 왼손잡이 설정(버튼 바꾸기)에서도 지금과 같다.
- **판정 시점이 같다.** 지금 `Player::CheckKey` 는 `SetKey()`(DirectInput 스냅숏) 직후에 `m_dwKey = CKeyMgr::GetKey()` 를 받는다. `CKeyMgr::CheckKey` 는 `CMainGame::Progress` 에서 프레임마다 장면보다 먼저 불린다. 그래서 두 값 모두 「이번 프레임의 버튼 상태」다.
- **비활성 판정은 `GameAsyncKeyState` 한 곳에서 한다.** 입력 읽기가 모두 이 함수를 거치므로 호출부를 고치지 않는다.
  - `GetActiveWindow()` 는 호출 스레드의 활성 창이다. 다른 프로세스 창이 포그라운드면 `nullptr` 이다.
  - Client 는 창 하나·UI 스레드 하나라 「게임 창이 활성인가」와 같다. `Function.h` 는 `g_hWnd` 선언보다 먼저 포함되므로 창 핸들을 비교하지 않는다.
- **스모크에는 영향이 없다.** `GameAsyncKeyState` 는 스모크에서 이미 비활성 판정보다 먼저 0 을 돌려준다. 자동 진행은 `IsSmokeMode()` 분기로 동작한다.

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| 비활성 창을 클릭해 활성화하는 그 클릭이 게임 입력으로 들어감 | 사용성 | 지금도 같다. 클릭 순간 창이 활성화되므로 그 뒤 프레임에는 눌림으로 읽힌다. 바꾸지 않는다 |
| 창 활성 여부·조작감은 자동 판정 불가 | 검증 | R4 는 코드 검토, R5 는 사용자 수동 확인. 스모크는 입력을 받지 않으므로 회귀만 본다 |
