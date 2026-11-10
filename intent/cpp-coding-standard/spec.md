# Spec: cpp-coding-standard

Author: Ryu Jahyeon
Status: approved (사용자, 2026-11-10)

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | 표준 원문이 `.harness/standards/cpp11-coding-standard.md` 에 **내용 그대로** 있다 | 원본과 `cmp` 동일 |
| R2 | CLAUDE.md 「C++ 코딩 표준」 절(하네스 템플릿 형식)이 적용 범위를 정한다: 기존 명명·구조 관례 유지 / 고치는 코드·새 코드엔 품질 규칙 / 「미정」 항목은 규칙 아님 | 문서 검토 |
| R3 | 충돌 시 우선순위(CLAUDE.md → 표준)가 명시된다 | 문서 검토 |
| R4 | 인용한 항목 번호가 원문 번호와 일치한다 | 원문 대조 |
| R5 | 코드·프로젝트 파일 변경 없음 | `git diff --stat` 에 `2D-Portfolio/` 없음 |

## 설계

- **위치는 `.harness/standards/`** — 하네스가 표준과 `coding-standard` 스킬을 함께 배포한다(agent-harness `d18e263`).
  처음엔 `.harness/principles/` 로 잡았으나 `update.sh` 가 principles 를 갱신하지 않아 하네스가 경로를 바꿨다.
  AGENTS.md 처럼 `@` 로 매 세션 로드하지 않고 CLAUDE.md 에서 링크만 한다(17KB, 필요할 때 읽음).
- **CLAUDE.md 는 템플릿(`.harness/templates/product-CLAUDE.md`) 「C++ 코딩 표준」 절 형식** — `/coding-standard` 스킬이
  이 절에서 언어 표준·검사 제외 경로·적용 범위를 읽는다.
- **원문은 수정하지 않고 복사** — 출처 문서와 동기화가 쉽도록. 이 repo 에 맞춘 해석은 전부 CLAUDE.md 쪽에(관심사 분리).
- **기존 관례 유지가 기본** — 표준을 소급 적용하면 `C` 접두사·`m_` 헝가리안 등 수천 줄 개명이 생기고 이력·리뷰가 망가진다.
  대신 품질 규칙(경고·`const`·`nullptr`·`override`·중괄호 등)은 레거시 인텐트와 방향이 같아 고치는 범위에서 적용한다.
- **새 파일도 이름은 주변 관례** — 한 코드베이스에 두 명명 체계가 섞이는 것을 피한다(PIE: 읽는 사람 기준).

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| 표준은 C++11 기준, 프로젝트는 MSVC 기본 **C++14** 로 컴파일(`/std` 미지정) | 일관성 | 수용. C++11 규칙은 C++14 에서 모두 유효. C++14 이후 기능(`std::make_unique`, `auto` 반환, C++17 `[[nodiscard]]` 등) 규칙은 이 표준에 없음 |
| 더 최신 표준(C++17/20) 문서가 없음 | 완전성 | 원본 폴더에 없음. 필요 시 공개 지침(ISO C++ Core Guidelines 등)을 별도 인텐트로 검토 — `/std` 상향과 함께 판단 |
| I-16(getter/setter 강제)·I-40(`New`/`Delete` 래퍼) 등은 현 구조와 크게 다름 | 적용 가능성 | 「기존 관례 유지」 범위로 처리. 해당 리팩터링이 필요해지면 별도 인텐트 |
| 원본 문서가 OneDrive 에서 바뀌면 사본과 어긋남 | 동기화 | 수용. 바뀌면 다시 복사하고 diff 를 커밋 |
