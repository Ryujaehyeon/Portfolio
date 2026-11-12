# Intent: 컴파일러 경고 0, 경고를 오류로

Author: Ryu Jahyeon
Status: draft

## Problem
빌드마다 경고가 수백 줄 나와 **새 경고가 묻힌다.** CLAUDE.md 는 「기존 경고 기준 수(Win32 196, x64 268·206)보다 늘면 안 된다」고 적어 두고 매번 세어 비교한다.
근거: [legacy-audit/review.md](../legacy-audit/review.md) 11번(P3), 2026-11-12 `master`(`cc3f483`) 클린 빌드(`/W3`)에서 위치 기준 중복 제거. 경로는 `2D-Portfolio/` 기준.

- **Win32 196건**(4개 구성 공통 종류): C4244(암묵적 축소 변환) 177, C4838(중괄호 초기화의 축소 변환) 18, C4018(signed/unsigned 비교) 1.
  - **122건(62%)이 한 원인** — 애니메이션 프레임 수 상수 18개(`Headers/Constant.h`, `PLAYER_*`·`SKELETON_*`·`SUMMONER_*`)가 정수 값인데 `const float` 로 선언돼,
    텍스처 개수(`int`)로 `InsertTexture` 에 넘길 때마다(`Client/Stage.cpp`) 경고가 난다.
  - 나머지 74건: `UIObj.cpp` 34(그리기 `RECT` 를 `float` 로 초기화 C4838 16 등), `MFC_000View.cpp` 11, `Monster.cpp` 10, `Player.cpp` 5, 그 밖 8개 파일 14.
- **x64 추가**: C4267(`size_t` → `int`/`DWORD`) 13곳 — `vector::size()`·`strlen` 결과를 좁은 정수에 담는다(`Debug.hpp` 2곳은 번역 단위마다 반복돼 Debug 집계를 부풀린다).
- 코딩 표준 I-56(「모든 컴파일러 경고는 반드시 고친다」)을 지키지 못하고 있다.

## Proposed outcome
- 두 프로젝트·4개 구성 빌드 경고 0.
- 이후 경고가 생기면 빌드가 실패한다(경고를 오류로 — `/WX`, 사용자 결정).
- 게임·툴 동작은 그대로다 — 축소 변환은 지금 컴파일러가 하던 것과 같은 결과가 되게 **명시**한다(값이 바뀌는 「고침」은 하지 않는다).
- CLAUDE.md 「빌드」의 경고 기준 수가 「경고 0, `/WX`」로 바뀐다.

## Affected users and systems
사용자: 이 repo 개발자(새 경고가 바로 드러남). 게임·툴 사용자에게 보이는 변화 없음
시스템: `Headers/Constant.h`·`Debug.hpp`, Client·MFC_000 의 경고가 난 파일들, 두 vcxproj, CLAUDE.md·`docs/FIX-GUIDE.md`

## Constraints
반드시 지킬 것:
- 동작 불변 — 4개 구성 스모크. 변환 결과 값이 지금과 같아야 한다(예: `double` 식은 `float` 로 바꾸지 말고 결과를 `static_cast<float>` — 중간 정밀도가 바뀌지 않게).
- 경고를 `#pragma warning(disable)`·`/wd` 로 끄지 않는다 — 코드를 고친다. 불가피하면 이유를 남긴다.
- 기존 명명·구조 관례 유지.

범위 밖:
- 경고 수준 올리기(`/W4`) — 사용자 결정으로 `/W3` 유지.
- 서드파티(`Client/fmod/`, NuGet D3DX) 헤더의 경고 — 지금은 없다.

## Open questions
없음 — 사용자 결정(2026-11-12): 경고 0 뒤 `/WX` 켜기, `/W3` 유지. 프레임 수 상수는 정수(`constexpr int`)로 바꾼다(시험 빌드에서 122건이 사라지고 새 경고 없음 확인).
