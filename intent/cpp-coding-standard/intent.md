# Intent: C++ 코딩 표준 도입

Author: Ryu Jahyeon
Status: approved (사용자, 2026-11-10)

## Problem
CLAUDE.md 에는 이 코드베이스의 **기존 관례**(명명·파일 구조)만 있고, 고치거나 새로 쓰는 코드의 **품질 기준**이 없다.
레거시 정리 인텐트(경고 정리, `const`, 문자열 비교 등)마다 기준을 새로 말해야 하고, 리뷰 때 근거로 댈 문서가 없다.

## Proposed outcome
- 개발자가 쓰던 C++ 코딩 표준이 저장소 안에 있고, CLAUDE.md 가 **어디까지 적용하는지** 정한다.
- 기존 관례와 표준이 충돌할 때 무엇을 따를지 한 줄로 판정된다.

## Affected users and systems
사용자: 이 repo 개발자, 에이전트(리뷰·작성 기준)
시스템: `.harness/standards/`(하네스 배포), `CLAUDE.md`. 코드는 바꾸지 않는다.

## Constraints
반드시 지킬 것: 기존 코드의 일괄 개명·재구조화를 유발하지 않는다.
범위 밖: 표준에 맞춘 코드 수정(각 레거시 인텐트에서), 컴파일 언어 표준(`/std`) 변경.

## 검토한 후보 (2026-11-10)
- `C++11 Coding Standards Summary.md` — **채택**. 일반 C++ 표준, 프로젝트 규칙 우선을 스스로 명시.
- `C-C++ Safety Coding Standard.md` — **제외**. MISRA C:2012·C++:2008 + 방위사업청 무기체계 코딩규칙 기반 안전필수 표준.
  동적 할당(§3.1.10.1)·재귀(§3.1.8.2)·예외(§3.1.9.22) 금지 등 게임 구조 자체와 충돌하고, 214KB 로 컨텍스트 비용이 크다.

## Open questions
- C++14 이후용 표준: 원본 폴더(`OneDrive/Agents/standards`)에는 없다. 필요하면 별도 인텐트로(아래 spec 「flag」).
