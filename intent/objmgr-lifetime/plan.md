# Plan: objmgr-lifetime

Author: Ryu Jahyeon
Status: approved (사용자 위임 — 「이대로 + 머지까지」, 2026-11-12)

<!-- 입력: 승인된 spec.md (PR #28). 경로는 2D-Portfolio/ 기준. 줄 번호는 master `27fd9a3` 기준. -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Client/ObjMgr.cpp` | `Progress` 첫머리에서 `IterRelease()` 한 번, 루프 안 호출·`bErased`·`erase` 경로 삭제(R1·R4). `IterRelease` 재작성: 수집 → 대상 끊기 → delete + `remove_if`(R2·R3), 스모크 로그(R5) |
| `Client/Monster.cpp` | `Progress` 에 스모크 분기 — 스테이지 1.5초 뒤 한 마리 체력 0(R5) |
| `Client/Client.cpp` | 스모크 필수 항목에 `removed monster`(R5) |
| `CLAUDE.md`, `docs/ARCHITECTURE.md` | 스모크가 확인하는 것, 「7. 객체 생성과 소멸」(R9) |

## 작업 순서

브랜치 `feature/objmgr-lifetime`.

1. **정리 위치·방식** — `ObjMgr.cpp` 변경. → Win32 Debug 빌드 + 스모크 PASS. 커밋 `fix(objmgr-lifetime): 죽은 객체를 프레임 시작에 한 번 정리`
2. **스모크 검증 경로** — `Monster.cpp`·`Client.cpp`·`IterRelease` 의 스모크 로그. → Win32 Debug·Release 스모크에서 `smoke kill`·`removed monster` 확인.
   커밋 `test(objmgr-lifetime): 스모크가 몬스터 정리 경로를 검증`
3. **전체 검증**(ub-fixes 와 같은 절차: 클린 빌드 4개 구성, 스모크 4개 구성, PowerShell 일반 실행 8초) → **문서** → PR → 머지 → 브랜치 삭제.

## 테스트

- R1: `git grep -n "IterRelease" -- 2D-Portfolio/Client` → `ObjMgr.h` 선언, `ObjMgr.cpp` 정의, `Progress` 첫머리 호출 1곳.
- R5·R6: `smoke.log` 에 `INFO smoke kill <이름>`, `PASS removed monster <이름>`, `RESULT PASS (fail=0)` — 4개 구성.
- R7: 일반 실행 8초 뒤 살아 있음, `smoke.log` 없음.
- R8: 경고 Win32 196, x64 268·206.

## 리스크

- 정리 시점이 바뀌어 죽은 몬스터의 경험치를 두 번 주거나 못 줄 수 있음 → spec 의 판정 시점 분석. 스모크는 경험치를 보지 않으므로 코드 검토로 확인.
- 스모크 kill 이 스테이지 4초 안에 정리까지 가지 않음 → `missing removed monster` 로 드러남. 그때 시각 조정.
