# AI-Native SDLC (6단계)

제품 repo 에서 AI 에이전트가 참여하는 개발 수명주기를 6단계로 운영한다.
원문: [The AI-Native SDLC Playbook](https://claude.com/blog/the-ai-native-sdlc-playbook).

각 단계는 **머신 판독 가능한 아티팩트**를 하나 남기고, 사람 판단은 매 단계를 처음부터
시작하는 대신 **게이트(승인 지점)**에 모인다.

## 도구

GitHub(repo·PR) · Slack(@claude 태그·알림) · Claude(에이전트) 셋뿐이다.
요구·설계·계획·코드·리뷰가 전부 제품 repo 하나에 산다 (SSOT). 외부 트래커·위키를 쓰지 않는다.

## 아티팩트 연쇄

변경 1개 = `intent/<기능-슬러그>/` 폴더 1개. 초안은 전부 에이전트가 쓴다.

| 단계 | 아티팩트 | 내용 | 위치 | 초안 / 승인 |
|---|---|---|---|---|
| 1 Plan | `intent.md` | 문제 / 제안 결과 / 영향받는 사용자·시스템 / 제약 / 미해결 질문 | `<product>/intent/<기능-슬러그>/intent.md` | 에이전트 / **PO** |
| 2 Design | `spec.md` | 요구사항 / 설계 / flag된 우려사항 | `<product>/intent/<기능-슬러그>/spec.md` | 에이전트 / **PO** |
| 3 Build | `plan.md` + 코드 diff | 영향 파일 / 작업 순서 / 테스트 / 리스크 | `<product>/intent/<기능-슬러그>/plan.md` | 에이전트 / **개발자** |
| 4 Test | 테스트 출력 / CI eval | 통과 결과, 빌드 로그, 회귀 eval | 세션 기록 / PR 체크 | — |
| 5 Deploy | 리뷰 결과 PR | 3개 리뷰 패스 결과, 승인 결정 | PR 히스토리 | 에이전트 / 사람 코드 오너 |
| 6 Maintain | 모니터링발 `intent.md` / 장애 기록 | control-band 이탈 진단, 제안 수정 | `reflections/` / 새 `intent/<슬러그>/` | — |

Stage 6 의 산출물은 다시 Stage 1 의 `intent.md` 로 되먹임되어 루프를 돈다.

## 슬러그 규약

한 변경 = 슬러그 하나. 이 문자열이 폴더(`intent/<슬러그>/`)·브랜치·커밋 scope·PR 제목에
전부 같은 형태로 박혀 조인 키가 된다.

- 형태 `<공통어간>-<구분어>`, 소문자 + 하이픈. 읽어서 뭔지 알아야 한다 — ❌ `a-1` `phase2`,
  ✅ `can-codec-cannelloni` `procedure-runner`.
- 관련된 조각은 어간을 공유해 형제 관계를 드러낸다 (`can-codec-slcan` / `can-codec-cannelloni`).
- 슬러그 하나 = `intent.md`·`spec.md`·`plan.md` 한 벌. spec 이 둘 필요하면 슬러그를 쪼갠다.

## 브랜치·승인 규약

- 브랜치: `intent/<슬러그>` → `spec/<슬러그>` → `feature/<슬러그>`. 트렁크는 MAIN(또는 develop).
- 트렁크 게이트는 사람이 두 번: **intent PR → PO 머지**, **spec PR → PO 머지**.
- `plan.md` 는 별도 트렁크 게이트가 아니라 `feature/<슬러그>` 브랜치에서 코드와 함께
  **담당 개발자 리뷰**로 흡수된다 (PO 아님 — 기술·코드베이스 판단).
- **PR 이 없는 저장소(SVN 등)**: 브랜치 규약은 적용하지 않는다. 사용자가 명시적으로 승인하면 그 산출물
  첫머리의 `Status:` 줄을 `approved (<승인자>, <YYYY-MM-DD>)` 로 바꾸고, 이것을 PR 머지 대신 승인으로 본다.
  에이전트는 사용자의 명시적 승인 없이 이 값을 바꾸지 않는다.
- 에이전트는 자기 변경을 승인할 수 없다.

## 구현 상태 매트릭스

| 단계 | 상태 | 수단 |
|---|---|---|
| 1 Plan | 운영 | `deep-interview` 스킬 → `.harness/templates/intent.md` |
| 2 Design | 운영 | `write-spec` 스킬 → `.harness/templates/spec.md` |
| 3 Build | 운영 | `plan-brief` 스킬 → `.harness/templates/plan.md`, `.claude/hooks/pre-commit-secret-scan.sh` |
| 4 Test | 설계만 | `.harness/sdlc/stage-4-test.md` (대략적인 안) |
| 5 Deploy | 설계만 | `.harness/sdlc/stage-5-deploy.md` + `.harness/sdlc/review-layers.md` + `.harness/templates/REVIEW.md` + `layered-review` 스킬 |
| 6 Maintain | 설계만 | `.harness/sdlc/stage-6-maintain.md` (대략적인 안) |

## 거버넌스 5계층 현황

| 계층 | 플레이북 정의 | 현황 |
|---|---|---|
| 1. Artifact Versioning | 모든 산출물을 git 에 커밋(작성자·시각·이력) | 부분 — 템플릿·위치·브랜치 규약은 정했으나 제품 repo 가 실제 커밋해야 완성 |
| 2. Policy-as-Code | Skills = 정책, Hooks = 결정론적 가드레일 | Skills ✅ / Hooks 스크립트만(`.claude/hooks/`, 미배선) |
| 3. Human Approval Gates | 브랜치 보호, 릴리스 승인, 코드 오너 사인오프 | 부분 — intent·spec PR 머지 게이트는 규정, 자동 강제는 없음 |
| 4. Sandbox Isolation | OS 수준 파일/네트워크 제한, 환경별 자격증명 | 부분 — 권한·plan mode 관례만 |
| 5. Audit Trail | 세션 기록, PR 히스토리, OpenTelemetry, 커밋된 아티팩트 | ❌ — managed settings/텔레메트리 미도입 |

## 도입 순서

1. 제품 repo 한 곳에 하네스를 채택([agent-harness README 「새 프로젝트에 설치하기」](https://github.com/Ryujaehyeon/agent-harness#새-프로젝트에-설치하기))하고 Stage 1~3 을 한 사이클 돌려 본다 (`intent → spec → plan → 구현`).
2. `.harness/templates/product-CLAUDE.md` 기반 `CLAUDE.md` 를 채운다.
3. 검증 명령이 안정되면 Stage 4 의 "지금 가능한 최소 버전"을 적용한다.
4. GitHub PR 흐름을 상시 쓰게 되면 Stage 5, 배포된 서비스가 생기면 Stage 6 을 실제 구축한다.

## 단계별 문서

- [stage-1-plan.md](stage-1-plan.md) · [stage-2-design.md](stage-2-design.md) · [stage-3-build.md](stage-3-build.md) — 운영 지침
- [stage-4-test.md](stage-4-test.md) · [stage-5-deploy.md](stage-5-deploy.md) · [stage-6-maintain.md](stage-6-maintain.md) — 대략적인 안
