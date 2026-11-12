# Intent: 문자열 포인터를 const 로, 표준 준수 모드 켜기

Author: Ryu Jahyeon
Status: draft

## Problem
문자열 리터럴(`const wchar_t[]`)을 non-const `TCHAR*` 에 담는다. 표준에서는 C++11 부터 허용되지 않는 변환인데, MSVC 의 비표준 확장 덕에 컴파일된다.
그래서 표준 준수 모드(ConformanceMode, `/permissive-`)를 켤 수 없다 — CLAUDE.md 「빌드」에 「켜면 대량 오류 — 별도 작업」으로 남아 있다.
근거: [legacy-audit/review.md](../legacy-audit/review.md) 13번, 2026-11-12 `master`(`87d9afc`)에서 `_CL_=/permissive-` 시험 빌드(Win32 Debug)로 측정. 경로는 `2D-Portfolio/` 기준.

- **오류 282건**(위치 기준 중복 제거, Client·툴 합계):
  - const 문제 266건 — 리터럴 → `TCHAR*` 대입·초기화(C2440 226: 이름 상수 `Headers/Constant.h` 38개 × 두 프로젝트 76, 대입 117, 지역 초기화 33),
    그 때문에 맞는 생성자를 못 찾음(C2665 38, `StageObjProto.cpp`·`SkillProto.cpp`·`MenuObjProto.cpp` 의 원형 생성), 인자 변환(C2664 2: `Debug.hpp` 의 `__writew`, `VelueToPercentage`).
  - 임시 값의 주소(C2102 16) — `PtInRect(&RealRect(), …)` 처럼 함수가 돌려준 `RECT` 의 주소를 받는다(Client 10, 툴 6). const 와 무관하지만 `/permissive-` 가 거부한다.
- `TCHAR*` 로 받는 멤버·인자(`m_pObjName`·`m_pObjKey`·`m_pStateKey`·`m_pMotion`, 생성자·`SetMotion`·`FontOutput` 등)가 실제로는 문자열을 고치지 않는다 — 타입이 의도를 말하지 못한다.
- 비표준 확장에 기대는 코드는 컴파일러가 바뀌면 깨진다. 표준 준수 모드를 켜 두면 새 코드가 다시 기대지 못한다.

## Proposed outcome
- 문자열을 읽기만 하는 포인터(이름 상수·멤버·인자·지역)가 `const TCHAR*` 다. 임시 값의 주소를 받지 않는다.
- 두 프로젝트(Client·MFC_000)의 4개 구성 모두 `ConformanceMode=true`(`/permissive-`)로 오류 0, 경고가 늘지 않는다.
- 게임·툴 동작은 그대로다(4개 구성 스모크 PASS).
- CLAUDE.md 「빌드」의 「ConformanceMode 는 꺼져 있다」가 「켜져 있다」로 바뀐다.

## Affected users and systems
사용자: 이 repo 개발자(타입이 의도를 말하고, 비표준 확장에 기대는 코드를 컴파일러가 막는다). 게임·툴 사용자에게 보이는 변화 없음
시스템: `Headers/Constant.h`·`Debug.hpp`, Client 의 `CObj` 계열 헤더·소스(오류가 난 20여 개 파일), `MFC_000/MFC_000View.cpp`·`MiniView.cpp`, 두 vcxproj, CLAUDE.md

## Constraints
반드시 지킬 것:
- 동작 불변 — 4개 구성 빌드·스모크. 경고가 늘지 않는다.
- 기존 명명·구조 관례 유지(CLAUDE.md 「C++ 코딩 표준」) — 타입에 `const` 를 붙이고 임시 값을 지역 변수로 받는 것만 한다. 이름·구조는 바꾸지 않는다.
- `.dat` 구조체(`Headers/Struct.h`)는 건드리지 않는다.
- `const_cast` 로 오류를 덮지 않는다 — 불가피한 곳이 나오면 이유를 남긴다.

범위 밖:
- 문자열을 `wstring`·`enum` 으로 바꾸는 구조 변경. 남은 경고 정리(→ `warning-cleanup`).
- `/permissive-` 외의 표준 옵션(`/std:c++17` 등) 변경.

## Open questions
없음 — 두 프로젝트·4개 구성 모두에 켠다(CLAUDE.md 가 이미 「별도 작업으로 다룬다」고 예고한 일).
