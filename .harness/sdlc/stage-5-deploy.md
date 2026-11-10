> **상태: 설계만 · 미구현.** 아래는 대략적인 안이며 아직 운영하지 않는다.

# Stage 5: Deploy — 대략적인 안

## 목표

Claude 가 들어오는 PR 을 리뷰하고(버그·로직 / 보안·취약점 / spec·plan 대비 컴플라이언스)
리뷰 코멘트에 대응한다. 배포 자율성은 dev → staging → prod 로 계층적으로만 넓힌다.
에이전트는 자기 코드를 승인할 수 없다.

## 지금 가능한 최소 버전 (인프라 없이)

- `/code-review` 사용을 관례화한다 (변경 diff 또는 PR 대상). 레이어 전체 점검은 `layered-review` 스킬로 한다.
- 리뷰 레이어와 각 레이어의 판별 질문은 [review-layers.md](review-layers.md) 를 따른다.
- `.harness/templates/REVIEW.md` 를 프로젝트에 채택한다 — 3개 리뷰 패스,
  심각도 기준(Important / Nit / Pre-existing), 제외 대상(생성 파일, CI 강제 항목), 자기 승인 금지.
- 리뷰 결과는 심각도를 붙여 보고하고, Important 는 병합 전에 해소한다.

## 실제 구축이 필요한 부분 (미룸)

- 승인 게이트 hook: 운영 배포 차단, 변경 티켓 요구, 지정 릴리스 매니저 승인.
- 계층적 자율성(dev→staging→prod)과 환경별 자격증명 스코프.
- MCP 배포 도구 연동.
- managed settings 로 접근 제어 강제.
- 롤백 리허설.

## 도입 트리거

GitHub PR 흐름을 상시 사용하고 협업자가 늘어날 때.

## 플레이북

[The AI-Native SDLC Playbook](https://claude.com/blog/the-ai-native-sdlc-playbook) — Stage 5: Deploy.
