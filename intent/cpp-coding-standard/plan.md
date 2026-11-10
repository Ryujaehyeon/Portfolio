# Plan: cpp-coding-standard

Author: Ryu Jahyeon
Status: approved (사용자, 2026-11-10) · 구현 완료

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `.harness/standards/cpp11-coding-standard.md` (신규) | 하네스 `update.sh` 로 받음(원본 `C:\Users\LOQ\OneDrive\Agents\standards\C++11 Coding Standards Summary.md` 와 동일) |
| `.claude/skills/coding-standard/`·`.hermes/skills/coding-standard/` (신규) | 하네스 `update.sh` 로 받음 — `/coding-standard` 보고서 스킬 |
| `CLAUDE.md` 「코드 관례」 아래 `### C++ 코딩 표준` | 템플릿 형식: 표준 경로, 언어 표준(C++14), 검사 제외 경로, 적용 범위 표, 우선순위 |
| `docs/FIX-GUIDE.md` 「함정 모음」 | 새 코드 작성 시 표준 적용 범위 한 줄 |

## 작업 순서

1. 하네스 반영(`update.sh --no-stage`) → 표준이 원본과 `cmp` 동일 확인.
2. CLAUDE.md 적용 범위 작성 → 인용 항목 번호를 원문과 대조.
3. FIX-GUIDE 한 줄.
4. `/coding-standard 2D-Portfolio/Client/SceneMgr.cpp` 1회 실행해 보고서 양식 확인(보고서는 커밋하지 않음).
5. 커밋 `docs(cpp-coding-standard): C++11 코딩 표준 도입과 적용 범위` → PR.

## 테스트

문서 변경만 있으므로 빌드 영향 없음. `git diff --stat origin/master` 에 `2D-Portfolio/` 경로가 없어야 한다.

## 리스크

- PR #9(debug-dll-removal)도 CLAUDE.md 「코드 관례」 끝과 FIX-GUIDE 를 고친다. 템플릿 형식을 따르느라 `### C++ 코딩 표준` 이
  「코드 관례」 목록 뒤(PR #9 추가 줄과 같은 자리)에 들어가 나중에 머지되는 쪽에서 충돌이 난다 — 양쪽 모두 유지
  (PR #9 의 목록 줄 → 이 절 순)로 해결. FIX-GUIDE 는 표 **맨 끝**이라 겹치지 않는다.
