# Plan: smoke-stabilize

Author: Ryu Jahyeon
Status: approved (사용자 지시 — 「스모크 안정화부터 진행」, 2026-11-12)

<!-- 입력: spec.md. 경로는 2D-Portfolio/ 기준. -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Client/Obj.cpp` | 이름 받는 생성자에 `m_dwKey(0)`(R1) |
| `Client/Player.cpp` | 「도달하지 못했을 때」 분기에 스모크 시전(R2) |
| `docs/FIX-GUIDE.md` | 다음 순서에서 `warning-cleanup` 삭제(R5) |

## 작업 순서

1. R1·R2 수정 → x64 Release 빌드, 스모크 30회 연속. 커밋 `fix(smoke-stabilize): m_dwKey 초기화, 스모크 시전을 우클릭 두 자리 모두에서`
2. 클린 4개 구성 빌드·스모크(x64 Release 외 각 5회)·일반 실행 → 문서 → PR → 머지.

## 테스트

- 수정 전 기준: 진단 로그 실행에서 x64 Release 30회 중 3회 실패(끊김형 2, 선택 해제형 1), 그 전 측정 13회 중 4회.
- 수정 후: x64 Release 30회 연속 PASS.

## 리스크

- 세 번째 원인이 있으면 30회 안에 드러난다 → 멈추고 진단 로그로 다시 조사.
