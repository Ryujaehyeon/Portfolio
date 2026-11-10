# Intent: 레거시 코드 오딧

Author: Ryu Jahyeon
Status: approved (사용자, 2026-11-10 대화 승인)

## Problem
2016년 코드베이스에 쓰지 않는 코드·파일, 커밋된 빌드 산출물, 우연히 동작하는 코드(문자열 포인터 비교,
반환값 누락 등)가 섞여 있다. 무엇을 걷어내고 무엇을 고쳐야 하는지 근거 있는 목록이 없어서
수정 순서와 범위를 정할 수 없다.

## Proposed outcome
- `review.md` 에 `file:line` 근거가 붙은 항목 목록과 우선순위가 있다.
- 각 묶음이 후속 인텐트 슬러그로 쪼개져, 하나씩 `intent/<슬러그>/` 로 착수할 수 있다.

## Affected users and systems
사용자: 이 repo 개발자
시스템: `2D-Portfolio/` 전체(Client, MFC_000, Headers), repo 에 커밋된 바이너리

## Constraints
반드시 지킬 것: 이 인텐트는 **조사·문서만** 한다. 코드는 바꾸지 않는다.
범위 밖: 수정 자체(후속 인텐트), DirectX 버전 전환(별도 인텐트로 남김).

## Open questions
없음 — 착수 순서는 `review.md` 「제안 순서」를 사용자가 확정한다.
