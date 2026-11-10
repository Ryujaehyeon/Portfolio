---
name: write-spec
description: |
  승인된 intent.md 를 입력받아 설계 원칙과 프로젝트 정책을 제약으로 적용해 spec.md 를 작성한다.
  사용자가 write-spec, 설계 명세, spec 작성을 요청하거나 Stage 2(Design) 로 넘어갈 때 사용한다.
  intent.md 가 없거나 아직 승인되지 않았거나 요청이 모호하면 사용하지 않는다. 요구사항이 흐릿하면 deep-interview 를 먼저 쓴다.
argument-hint: "<intent.md 경로>"
---

# Write Spec (Stage 2: Design)

승인된 `intent.md` 를 요구사항·설계 명세(`spec.md`)로 바꾼다. 정책 위반 가능성은 엔지니어가 보기 전에 flag 한다.

## 선행 조건

- `intent/<기능-슬러그>/intent.md` 가 존재하고 PO 승인이 끝났다 — git 은 intent PR 트렁크 머지, PR 이 없는
  저장소는 `Status: approved (…)` (`.harness/sdlc/README.md` 「브랜치·승인 규약」).
- 없거나 미승인이면 멈추고 사용자에게 알린다. 추측으로 진행하지 않는다.

## 절차

1. `intent/<기능-슬러그>/intent.md` 를 읽고 문제·제안 결과·제약·미해결 질문을 파악한다.
2. 제약으로 로드한다:
   - `.harness/principles/solid-summary.md`, `.harness/principles/programming-principles-summary.md` (설계 판단 기준)
   - 회사/프로젝트 정책 — 대상 프로젝트의 정책 스킬(브랜드·보안·컴플라이언스·API 설계 등)이 있으면 그것도. 정책은 리뷰가 아니라 여기 명세 단계에서 설계에 반영한다.
3. `.harness/templates/spec.md` 형식으로 작성한다:
   - **요구사항**: intent 의 "제안하는 결과"를 검증 가능한 항목으로 분해.
   - **설계**: 모듈 경계·데이터 흐름·주요 인터페이스. 경계를 나누거나 추상화를 도입한 판단은 원칙 근거를 한 줄로 밝힌다. YAGNI — 지금 필요 없는 유연성은 넣지 않는다.
   - **flag된 우려사항**: 로드한 제약 중 위반 가능성이 있는 항목마다 「무엇이 / 어느 기준에 / 해소안 또는 담당자」.
4. 미해결 질문 중 설계에 필요한 것은 사용자에게 한 번에 하나씩 묻는다.

## 종료 기준

- 모든 요구사항이 판정 가능한 문장이다.
- flag된 우려사항이 비어 있거나, 각 항목에 해소안/수용 결정이 붙어 있다.
- 결과를 `intent/<기능-슬러그>/spec.md` 로 저장하고, flag 를 해소한 뒤 승인받도록 안내한다 — git 은
  `spec/<기능-슬러그>` 브랜치 PR → PO 머지, PR 이 없는 저장소는 사용자 승인 후 `Status: approved (…)`.

## 사용하지 말아야 할 때

- `intent.md` 가 없거나 미승인.
- 한두 줄 수정처럼 명세가 불필요한 작업.
- 요구사항 자체가 모호할 때 — `deep-interview` 로 먼저 정리한다.
