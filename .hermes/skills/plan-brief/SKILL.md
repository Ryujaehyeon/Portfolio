---
name: plan-brief
description: |
  승인된 spec.md 와 대상 프로젝트 CLAUDE.md 를 read-only 로 조사해(Claude Code 는 plan mode) 구현 계획(plan.md)을 만든다.
  사용자가 plan-brief, 구현 계획, Stage 3(Build) 착수를 요청할 때 사용한다.
  spec.md 가 없거나 미승인이면, 또는 한두 줄 수정처럼 계획이 불필요한 작업에는 사용하지 않는다.
argument-hint: "<spec.md 경로>"
---

# Plan Brief (Stage 3: Build)

승인된 `spec.md` 를 실행 가능한 구현 계획으로 바꾼다. 조사는 read-only 로만 하고, 승인 전에는 코드를 고치지 않는다.

## 선행 조건

- `intent/<기능-슬러그>/spec.md` 가 존재하고 PO 승인이 끝났다(git 은 spec PR 트렁크 머지, PR 이 없는 저장소는
  `Status: approved (…)` — `.harness/sdlc/README.md` 「브랜치·승인 규약」). 없으면 멈추고 알린다.
- read-only 로 진행한다(Claude Code 는 plan mode 로 진입). 이 단계에서 파일을 수정하지 않는다.

## 절차

1. `intent/<기능-슬러그>/spec.md` 의 요구사항·설계·flag된 우려사항을 읽는다.
2. 대상 프로젝트 `CLAUDE.md` 를 읽는다 — 빌드/테스트/린트 명령, 코드 관례, "반복하는 실수" 로그. 이것이 제도적 지식 파일이다.
3. 관련 코드를 read-only 로 조사해 실제 영향 범위를 확인한다.
4. `.harness/templates/plan.md` 형식으로 작성한다:
   - **영향받는 파일**: 만들/고칠/지울 파일과 각 파일에서 할 일 한 줄.
   - **작업 순서**: 번호 단계. 각 단계 = 무엇을 한다 → 어떻게 검증한다.
   - **테스트**: `CLAUDE.md` 에 적힌 검증 명령과 기대 출력. 버그 수정이면 재현 테스트를 먼저.
   - **리스크**: 계획이 틀릴 수 있는 지점과 대응.
5. 병렬 세션·worktree 격리가 필요하면 `.harness/collaboration/tmux-collaboration.md` 를 따른다.

## 종료 기준

- 계획을 담당 개발자에게 제시하고 승인을 받는다 (PO 아님 — 기술·코드베이스 판단).
- 승인된 `plan.md` 를 `intent/<기능-슬러그>/plan.md` 로 저장한다. git 은 `feature/<기능-슬러그>` 브랜치에서 코드와 함께
  커밋하고, PR 이 없는 저장소는 `Status: approved (…)` 로 승인을 남긴다.
- 그다음에만 read-only 를 풀고(Claude Code 는 plan mode 해제) 구현에 착수한다.

## 사용하지 말아야 할 때

- `spec.md` 가 없거나 미승인.
- 한두 줄 수정, 오타 수정처럼 계획 오버헤드가 더 큰 작업.
