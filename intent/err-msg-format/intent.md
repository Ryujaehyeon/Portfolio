# Intent: ERR_MSG 가 포맷 인자를 버리는 문제 수정

Author: Ryu Jahyeon
Status: draft

## Problem
리소스 로드 실패 메시지박스가 **무엇이 실패했는지 알려 주지 못한다.** 근거: [legacy-audit/review.md](../legacy-audit/review.md) 4번.

- `Headers/Macro.h:22` `#define ERR_MSG(hwnd, sz) MessageBox(hwnd, sz, L"system err", MB_OK)` — 인자 2개짜리 매크로다.
- 그런데 `Client/Stage.cpp` 의 84곳이 `ERR_MSG(g_hWnd, L"%d Run L 로드 실패", Monster[i]);` 처럼 **세 번째 인자**를 넘긴다.
  매크로가 그 인자를 버려(C4002) 메시지박스에 `%d`·`%s` 가 글자 그대로 찍힌다.
- 넘기는 인자는 모두 `TCHAR*` 문자열이다(`Charter[i]` 41곳, `Monster[i]`·`Monster[0]` 33곳, `BONESPEAR`·`FIREWALL`·`BLIZZARD` 10곳).
  그런데 포맷은 82곳이 `%d`, 2곳만 `%s` 다 — 인자를 살리기만 하면 문자열 주소가 숫자로 찍힌다.
- 이 84건이 Debug|Win32 빌드 경고 282건 중 C4002 84건 전부다(2026-11-10 Rebuild 기준).

## Proposed outcome
- 포맷 인자를 넘긴 `ERR_MSG` 호출이 실패한 리소스 이름(예: `Skeleton Run L 로드 실패`)을 메시지박스에 보여 준다.
- C4002 경고가 0 이 된다(Debug|Win32 경고 282 → 198).
- 포맷 인자 없이 부르는 나머지 `ERR_MSG` 호출(약 130곳, Client·MFC_000)은 고치지 않아도 지금과 같은 메시지를 보여 준다.

## Affected users and systems
사용자: 이 repo 개발자(리소스 누락·경로 문제를 메시지박스로 진단), 게임을 실행하는 사람
시스템: `Headers/Macro.h`(Client·MFC_000 공용), `Client/Stage.cpp`(포맷 인자를 넘기는 84곳 전부)

## Constraints
반드시 지킬 것: 메시지박스 제목(`system err`)·버튼·호출 시점은 그대로. 실패 시 흐름(`return E_FAIL` 등) 불변.
Release 에서도 동작해야 한다(`ERR_MSG` 는 디버그 전용이 아니다). 기존 2인자 호출부는 수정 없이 컴파일·동작.
범위 밖: 다른 경고(C4244·C4838·C4715·C4018 → `ub-fixes`·`warning-cleanup`), `TCHAR*` 의 `const` 화(→ `const-tchar`),
`Stage.cpp` 의 리소스 로드 반복 구조 개선, 메시지 문구 다듬기.

## Open questions
- 매크로를 가변 인자로 바꿀지, 가변 인자 함수(인라인)로 바꿀지 — spec 에서 정한다.
- 조립 버퍼 크기와 넘침 처리(`DebugOut` 처럼 `_TRUNCATE` 로 잘라낼지).
