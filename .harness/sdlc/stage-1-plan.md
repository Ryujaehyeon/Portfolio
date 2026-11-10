# Stage 1: Plan — 운영 지침

## 목표

문제와 의도를 머신 판독 가능한 `intent.md` 로 캡처한다. `intent.md` 가 **PRD·스프린트
문서를 대체**한다 — 아직 해결책이 아니라 "무엇이 왜 필요한가"만 담는다.

## 어떻게 하는가

1. `intent/<기능-슬러그>` 브랜치를 만든다.
2. 요청이 모호하면 `deep-interview` 스킬로 목표·범위·제약·완료 기준·열린 질문을 정리한다.
3. 정리된 내용을 `.harness/templates/intent.md` 형식(공식 5필드)으로 옮긴다:
   Problem / Proposed outcome / Affected users and systems / Constraints / Open questions.
4. `<product>/intent/<기능-슬러그>/intent.md` 로 저장하고 intent PR 을 연다.
5. **PO 가 검토하고 트렁크(MAIN/develop)로 머지한다. 머지가 곧 승인이다.**
   PR 이 없는 저장소(SVN 등)는 PO 가 승인하면 `Status: approved (…)` 로 남긴다 — `README.md` 「브랜치·승인 규약」.

## 초안 / 승인

- 초안: 에이전트(Advisor/Worker).
- 승인: **PO**(제품·정책 판단). 없으면 사용자가 그 역할을 한다. Claude 는 승인하지 않는다.
- git 이력이 작성자·시각·개정 이력을 남긴다 (거버넌스 계층 1).

## 측정 지표

- Build 착수 전에 `intent.md` 가 존재하는 작업의 비율.
- Design/Build 중에 "Open questions" 로 되돌아온 횟수 (적을수록 Plan 이 충실).

## 다음 단계

머지된 `intent.md` → [stage-2-design.md](stage-2-design.md) (`write-spec` 스킬).
