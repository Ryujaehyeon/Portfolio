# templates

SDLC 각 단계에서 채워 쓰는 아티팩트 골격. `<...>` 자리를 실제 값으로 채워서 복사해 쓴다.
언제·어떻게 쓰는지 자세한 절차는 [`../USAGE.md`](../USAGE.md)와 [`../../intent/README.md`](../../intent/README.md)의 SSOT 표를 본다.

| 파일 | 언제 | 결과물 위치 |
|---|---|---|
| `intent.md` | Stage 1(Plan) — 요구사항이 정리되면 | `intent/<기능-슬러그>/intent.md` |
| `spec.md` | Stage 2(Design) — 승인된 intent.md 를 설계로 옮길 때 | `intent/<기능-슬러그>/spec.md` |
| `plan.md` | Stage 3(Build) — 승인된 spec.md 로 구현 계획을 세울 때 | `intent/<기능-슬러그>/plan.md` |
| `REVIEW.md` | Stage 5(Deploy) — PR 리뷰 기준을 정할 때 | product repo 루트 |
| `product-CLAUDE.md` | 하네스를 새 product repo 에 채택할 때 | product repo 루트의 `CLAUDE.md` |
