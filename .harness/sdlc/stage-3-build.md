# Stage 3: Build — 운영 지침

## 목표

승인된 `spec.md` 를 구현 계획 `plan.md` 로 바꾸고, 개발자 승인 후 계획대로 구현한다.
계획 수립은 read-only(plan mode)로만 한다. `plan.md` 는 별도 트렁크 게이트가 아니라
`feature/<슬러그>` 브랜치에서 코드와 함께 개발자 리뷰로 흡수된다.

## 어떻게 하는가

1. `feature/<기능-슬러그>` 브랜치를 만든다.
2. `plan-brief` 스킬을 쓴다. 입력은 PO 가 머지한 `intent/<기능-슬러그>/spec.md`.
3. plan mode(read-only)에서 조사:
   - 제품 `CLAUDE.md` 를 읽는다. 이 파일이 **제도적 지식 파일**이다 —
     빌드/테스트/린트 명령과 각 정상 출력, 코드 관례, 아키텍처 패턴,
     "Claude 가 반복하는 실수" 로그. 골격은 `.harness/templates/product-CLAUDE.md`.
   - 관련 코드를 읽어 실제 영향 범위를 확인한다.
4. `.harness/templates/plan.md` 형식으로 작성: 영향받는 파일 / 작업 순서 / 테스트 / 리스크.
   - 각 작업 단계 = 무엇을 한다 → 어떻게 검증한다.
   - 버그 수정이면 재현 테스트를 먼저.
5. **담당 개발자**(PO 아님 — 기술·코드베이스 판단)가 계획을 승인한다.
6. `<product>/intent/<기능-슬러그>/plan.md` 를 코드와 함께 `feature/<슬러그>` 브랜치에 커밋 →
   구현 → 구현 PR → 리뷰·머지.
   - 커밋 메시지는 `type(<슬러그>): 제목` (Conventional Commits). 상세 규약은 제품 `CLAUDE.md` "커밋 규약".

## 이 하네스의 가드레일

- **Skills = 정책 as code**: `.claude/skills/`·`.hermes/skills/` (하네스가 정본 — product 에는 update 스크립트로 배포).
  설계 원칙은 `.harness/principles/`.
- **Hooks = 결정론적 가드레일**: `.claude/hooks/pre-commit-secret-scan.sh` 가 staged 파일의
  비밀정보를 스캔해 커밋을 차단한다. 배선 방법은 `.claude/hooks/README.md` (기본 미배선).
- **병렬 세션 / worktree 격리**: 여러 세션을 동시에 돌릴 때는
  `.harness/collaboration/tmux-collaboration.md` 를 따른다. 소스·빌드 출력·변경 데이터가
  서로 격리됐는지 먼저 확인한다.

## 초안 / 승인

- 초안: 에이전트. 계획 승인: **담당 개발자**. 리스크가 큰 작업은 테크 리드/아키텍트가 함께.

## 측정 지표

- 구현이 `plan.md` 의 "영향받는 파일" 범위를 벗어난 횟수 (범위 크리프 지표).
- `CLAUDE.md` "반복하는 실수" 로그에 같은 항목이 2회 이상 재등장하는지.

## 다음 단계

구현된 변경 → [stage-4-test.md](stage-4-test.md).
