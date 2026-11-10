# USAGE — 설치된 하네스로 일하기

하네스가 채택된 **product repo** 에서 변경 하나를 SDLC 루프에 태우는 방법.
단계별 배경·측정 지표는 [`sdlc/README.md`](sdlc/README.md) 와 `sdlc/stage-*.md`,
최신 하네스로 갱신하는 법은 하네스 저장소(agent-harness)의 `UPDATE.md` 를 본다.

## 한눈에

변경 1개 = `intent/<기능-슬러그>/` 폴더 1개. 그 안에 `intent.md` → `spec.md` → `plan.md`.
브랜치도 같은 순서로 흐르고, 트렁크로 가는 사람 승인은 두 번(intent·spec)이다.

| 단계 | 브랜치 | 산출물 | 스킬 | 초안 / 승인 |
|---|---|---|---|---|
| 1 Plan | `intent/<슬러그>` | `intent/<슬러그>/intent.md` | `deep-interview` (모호할 때) | 에이전트 / **PO** (PR 머지) |
| 2 Design | `spec/<슬러그>` | `intent/<슬러그>/spec.md` | `write-spec` | 에이전트 / **PO** (PR 머지) |
| 3 Build | `feature/<슬러그>` | `intent/<슬러그>/plan.md` + 코드 | `plan-brief` (plan mode) | 에이전트 / **개발자** (코드 리뷰) |
| 4–6 | — | 테스트·리뷰·회고 | (설계만) | — |

도구는 **GitHub(repo·PR) · Slack(@claude) · Claude** 뿐. 외부 트래커·위키를 쓰지 않는다.
에이전트는 자기 변경을 승인하지 않는다.

## 한 사이클 (예: `ship-tracking`)

### 1. Plan

```
git switch -c intent/ship-tracking
```

요청이 흐릿하면 `deep-interview` 로 목표·범위·제약·완료 기준을 먼저 정리한다.
정리되면 `.harness/templates/intent.md` 형식(공식 5필드: Problem / Proposed outcome /
Affected users and systems / Constraints / Open questions)으로:

```
intent/ship-tracking/intent.md
```

`intent.md` 가 PRD·스프린트 문서를 대체한다. PR 을 열고 **PO 가 트렁크로 머지 = 승인**.
PR 이 없는 저장소(SVN 등)는 PO 가 승인하면 `Status: approved (승인자, 날짜)` 로 남긴다(`sdlc/README.md` 「브랜치·승인 규약」).

### 2. Design

```
git switch -c spec/ship-tracking
```

`write-spec` 스킬. 입력은 머지된 `intent/ship-tracking/intent.md`.
제약으로 로드: `.harness/principles/` + 회사/프로젝트 정책 스킬(있으면).
`.harness/templates/spec.md` 형식으로 `intent/ship-tracking/spec.md` 작성 —
요구사항 / 설계 / **flag된 우려사항**(정책·보안·설계원칙 위반 가능성). flag 는 머지 전 해소.
PR → **PO 머지**.

### 3. Build

```
git switch -c feature/ship-tracking
```

`plan-brief` 스킬을 **plan mode(read-only)** 로. 입력은 머지된 `spec.md` + 제품 `CLAUDE.md`
(빌드/테스트 명령·관례·"반복하는 실수" 로그). `.harness/templates/plan.md` 형식으로
`intent/ship-tracking/plan.md` — 영향 파일 / 작업 순서(각 단계 → 검증) / 테스트 / 리스크.
**담당 개발자**가 계획 승인 → plan mode 해제 → 구현 → `plan.md` + 코드를 함께 커밋 →
구현 PR → 리뷰·머지.

### 4–6 (아직 설계만)

- **Test**: 단일 검증 명령(`make test` 등)을 `CLAUDE.md` 에 정상 출력과 함께 고정.
  버그는 재현 테스트 먼저. 세션은 통과시킨 뒤에만 완료 보고.
- **Deploy**: `/code-review`(버그) · `layered-review`(리뷰 레이어 전체) +
  `.harness/templates/REVIEW.md`(3패스·심각도·자기승인 금지). 레이어 기준은 `sdlc/review-layers.md`.
- **Maintain**: 장애는 `감지/진단/수정/교훈` 포맷 회고 → 반복 실수는 `CLAUDE.md` 로그로.

## 스킬

`.claude/skills/` 의 스킬은 설명(description)에 맞으면 자동 트리거되고, `/deep-interview` 처럼 직접
부를 수도 있다. Hermes 는 `.hermes/skills/` 를 프로필 설정이 있을 때만 읽는다. 프로젝트의 `.hermes/skills/` 는
`.git` 이 있는 root 가 `skills.trusted_project_dirs` 에 있을 때만 로드된다. 하네스 원본
(`<하네스>/.hermes/skills`)을 `skills.external_dirs` 에 지정하면 SVN 작업본을 포함해 어디서든 로드된다.
같은 이름이면 프로젝트 > 프로필 로컬 > external 순으로 우선한다.

| 스킬 | 쓸 때 | 안 쓸 때 |
|---|---|---|
| `deep-interview` | 목표·범위·제약이 흐릿 | 요청이 이미 구체적 / 오타·설정 소변경 |
| `write-spec` | 승인된 `intent.md` 로 Stage 2 진입 | `intent.md` 없음·미승인 / 요구사항 모호(→ `deep-interview` 먼저) |
| `plan-brief` | 승인된 `spec.md` 로 Stage 3 착수 | `spec.md` 없음·미승인 / 한두 줄 수정 |
| `layered-review` | 변경을 리뷰 레이어 전체로 점검 | 버그만 빠르게 볼 때(→ `/code-review`) |
| `tmux-collaboration` | TMUX 패인으로 다른 에이전트와 협업 | TMUX 협업이 아닌 일반 작업 |

이 문서의 `/code-review` 는 Claude Code 내장 명령(현재 diff·PR 대상)이다. code-review 플러그인을 설치했다면
그쪽은 `/code-review:code-review` 로 따로 부르는 별개 스킬이다.

`harness-diet`·`harness-legacy-scan` 은 하네스 자체 관리용이라 product repo 에 배포되지 않는다.

## Advisor / Worker

- **Advisor**: 사용자와 대화하는 주 에이전트. 요구 분석·분해·검증 기준 수립, Worker 결과
  직접 검증(diff·테스트·빌드), 최종 보고.
- **Worker**: Advisor 브리프 범위만 구현·조사. 막히면 추측 말고 `결정 요청` 반환.
- 승인 성격이 갈린다: **PO 모자**(intent·spec — 제품·정책) / **개발자 모자**(plan — 기술·코드베이스).
- 상세: `AGENTS.md`.

## hook

`.claude/hooks/pre-commit-secret-scan.sh` 는 git 작업본에서 staged 파일의 비밀정보를 스캔해 커밋을 막는다.
**기본 미배선** — `.claude/hooks/README.md` 절차로 git 네이티브 pre-commit 또는 Claude Code
PreToolUse 에 건다. SVN 작업본에서는 hook 이 동작하지 않으니 `python3 .claude/hooks/secret-scan.py <경로>`
로 직접 검사한다.

## 하네스 갱신

```
cd /path/to/agent-harness && git pull
cd /path/to/product-repo && /path/to/agent-harness/update.sh          # SVN 작업본이면 --svn
```

`.harness/templates`·`.harness/sdlc`·`.harness/USAGE.md`·`.claude/hooks` 와
`.claude/skills`·`.hermes/skills` 의 스킬(하네스 전용 `harness-diet`·`harness-legacy-scan` 제외)은
하네스가 정본이라 덮어쓴다. 제품이 채운 `CLAUDE.md` 와 커스터마이즈한 `AGENTS.md` 는 병합한다
(`update.sh` 는 이 둘을 건드리지 않는다). 상세: 하네스 저장소의 `UPDATE.md`.

## 유지보수자 노트

- `AGENTS.md`·`.harness/` 문서 파일명은 ASCII로 시작한다. 한글로 시작하면 `@파일명` import 가
  전개되지 않는다.
