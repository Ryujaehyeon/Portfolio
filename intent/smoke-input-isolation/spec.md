# Spec: smoke-input-isolation

Author: Ryu Jahyeon
Status: draft

<!-- 입력: 승인된 intent.md (PR #45). 경로는 2D-Portfolio/ 기준. -->

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | `Headers/Function.h` 에 입력 읽기 함수 두 개 — `GameAsyncKeyState(int)`: 스모크면 0, 아니면 `GetAsyncKeyState`. `GameCursorPos(HWND, POINT*)`: 스모크면 그 창 클라이언트 영역 가운데의 화면 좌표, 아니면 `GetCursorPos` | 코드 검토 |
| R2 | Client 코드에서 `GetAsyncKeyState`·`GetCursorPos` 를 직접 부르는 곳이 위 함수 밖에 없다 | `git grep` |
| R3 | `CDevice::SetKey` 가 스모크면 키보드·마우스 상태를 0 으로 둔다(장치를 읽지 않음) | 코드 검토 |
| R4 | 일반 실행은 그대로 — 스모크가 아니면 모든 함수가 원래 API 와 같은 값을 돌려준다 | 코드 검토 + 일반 실행 |
| R5 | 사용자가 평소처럼 PC 를 쓰는 동안 x64 Debug·Win32 Release Client 스모크 각 50회 PASS(분리 전에는 같은 조건에서 수 % 실패) | 반복 실행 |
| R6 | 4개 구성 클린 빌드 경고 0·오류 0, 툴 스모크·일반 실행 | CLAUDE.md |
| R7 | CLAUDE.md 「테스트」에 「스모크는 실제 입력을 받지 않는다(커서 고정)」, 「실행」에 일반 실행은 비활성 창에서도 입력을 받는다는 주의 | 문서 검토 |

## 설계

- **한 곳에서 가른다** — 입력 원천은 세 가지(전역 키 상태, DirectInput 장치, 커서 좌표)뿐이다. 각각 읽는 자리를 함수 하나로 모아 스모크 여부를 거기서만 판단한다(DRY — 호출부 9곳에 `IsSmokeMode()` 를 흩뿌리지 않는다).
  DirectInput 은 `CDevice::SetKey` 한 곳에서만 읽으므로 거기서 판단한다.
- `Function.h` 는 `g_hWnd` 선언(`Defines.h`)보다 먼저 포함되므로 `GameCursorPos` 는 창 핸들을 인자로 받는다.
- 커서 고정 위치는 창 가운데 — 플레이어가 화면 가운데에 있으므로 방향이 정해지고(각도 계산이 0 벡터가 되지 않게), 메뉴 버튼 위가 아니라 스모크 자동 클릭 경로와 겹치지 않는다.
- 스모크 자동 진행(메뉴 「시작」, 스킬 시전, 몬스터 처치)은 지금처럼 `IsSmokeMode()` 분기로 동작한다 — 실제 입력에 기대지 않는다.

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| 스모크가 「입력이 있는 경로」를 더 검증하지 못함 | 검증 범위 | 지금도 의도된 입력 경로는 스모크 분기로 흉내 낸다. 우연한 실제 입력에 기대는 검증은 재현할 수 없어 가치가 없다 |
| 일반 실행에서 비활성 창 입력 문제가 남음 | 사용성 | 범위 밖(intent) — `input-unify` |
