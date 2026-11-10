# Stage 2: Design — 운영 지침

## 목표

승인된 `intent.md` 를 요구사항·설계 명세 `spec.md` 로 만든다. **회사/프로젝트 정책이
설계에 들어간다** — 설계 원칙·보안·컴플라이언스·브랜드를 제약으로 적용하고, 위반
가능성은 리뷰가 아니라 이 단계에서 flag 한다.

## 어떻게 하는가

1. `spec/<기능-슬러그>` 브랜치를 만든다.
2. `write-spec` 스킬을 쓴다. 입력은 PO 가 머지한 `intent/<기능-슬러그>/intent.md`.
3. 제약으로 로드:
   - `.harness/principles/solid-summary.md`, `.harness/principles/programming-principles-summary.md`
   - 회사/프로젝트 정책 — 대상 프로젝트의 정책 스킬(브랜드·보안·컴플라이언스·API 설계 등)이 있으면 그것도.
4. `.harness/templates/spec.md` 형식으로 작성: 요구사항 / 설계 / flag된 우려사항.
   - 요구사항은 충족 여부를 판정할 수 있는 문장으로.
   - 설계 판단(경계 분리, 추상화 도입)은 원칙 근거를 한 줄로 밝힌다. YAGNI 적용.
   - flag된 우려사항: 「무엇이 / 어느 기준에 / 해소안 또는 담당자」.
5. flag된 항목은 담당자가 해소하거나 사용자가 명시적으로 수용 결정을 한다.
6. `<product>/intent/<기능-슬러그>/spec.md` 로 저장하고 spec PR 을 연다.
7. **PO 가 트렁크로 머지한다. 충돌은 사람이 해소한다.**
   PR 이 없는 저장소는 PO 승인 후 `Status: approved (…)` 로 남긴다 — `README.md` 「브랜치·승인 규약」.

## 초안 / 승인

- 초안: 에이전트. 승인: **PO**. 리스크가 큰 변경은 테크 리드가 함께 검토한다.

## 측정 지표

- flag된 우려사항 중 머지 전에 해소된 비율(수용 결정 포함, 미해결 0 이어야).
- Build 단계에서 발견된 정책 위반 수 (Design 에서 걸러졌어야 하는 것).

## 다음 단계

머지된 `spec.md` → [stage-3-build.md](stage-3-build.md) (`plan-brief` 스킬).
