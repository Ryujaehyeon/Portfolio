> **상태: 설계만 · 미구현.** 아래는 대략적인 안이며 아직 운영하지 않는다.

# Stage 6: Maintain — 대략적인 안

## 목표

결정론적 모니터링이 control-band 이탈을 감지하면 Claude 를 자동 호출한다.
1σ 로그만 / 2σ 읽기전용 진단 / 3σ 행동(PR 또는 사전 승인 런북).
진단 결과는 `intent.md` 로 되먹여 Stage 1 루프를 다시 시작한다.

## 지금 가능한 최소 버전 (인프라 없이)

- `reflections/` 회고를 "장애 기록" 포맷으로 정착시킨다:
  **감지 / 진단 / 수정 / 교훈**.
- 반복되는 실수는 회고에만 두지 말고 대상 프로젝트 `CLAUDE.md` 의
  "반복하는 실수" 로그로 올린다 (2회 반복 시).

## 실제 구축이 필요한 부분 (미룸)

- 결정론적 감지 스크립트: 지표별 1σ/2σ/3σ 밴드 판정.
- 예약 보안 스캔: `security-review` 스킬을 스케줄로 실행.
- Claude Tag 챗옵스: 인시던트 채널에서 호출.
- incident → 영구 eval 편입.

## 도입 트리거

운영 지표를 내보내는 배포된 서비스가 존재할 때.

## 플레이북

[The AI-Native SDLC Playbook](https://claude.com/blog/the-ai-native-sdlc-playbook) — Stage 6: Maintain.
