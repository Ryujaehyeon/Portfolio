> **상태: 설계만 · 미구현.** 아래는 대략적인 안이며 아직 운영하지 않는다.

# Stage 4: Test — 대략적인 안

## 목표

세션이 사람 리뷰 전에 스스로 검증한다. 단일 검증 명령으로 회귀를 잡고, 설정 변경에는
지속 eval 을 돌려 에이전트 구성 자체의 퇴행을 감지한다.

## 지금 가능한 최소 버전 (인프라 없이)

- 대상 repo 에 단일 검증 명령을 관례화한다 (`make test`, `npm test` 등 하나).
- 그 정상 출력을 대상 프로젝트 `CLAUDE.md` 에 그대로 기록한다 (무엇이 "통과"인지 고정).
- "버그는 실패 테스트 먼저" 를 지킨다 — `AGENTS.md` 의
  **4. Goal-Driven Execution** ("버그 수정" → "재현 테스트 작성 후 통과시키기").
- 세션은 이 명령을 통과시킨 뒤에만 완료 보고를 한다.

## 실제 구축이 필요한 부분 (미룸)

- 최근 작업 20~50개를 회귀 eval 스위트로 고정.
- `CLAUDE.md` / 스킬 / hook 이 바뀌면 eval 자동 실행.
- 운영 중 장애가 난 케이스를 영구 eval 로 편입.
- CI 체크 런, 스크린샷 diff, OpenTelemetry 내보내기.

## 도입 트리거

하위 repo 에 CI 파이프라인이 존재하고, 에이전트가 작성한 변경이 주 단위로 병합되기 시작할 때.

## 플레이북

[The AI-Native SDLC Playbook](https://claude.com/blog/the-ai-native-sdlc-playbook) — Stage 4: Test.
