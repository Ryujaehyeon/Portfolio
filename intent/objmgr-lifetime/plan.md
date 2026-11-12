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

## 구현 중 변경

- 스모크의 「어느 몬스터가 지워졌나」를 포인터로 대조하지 않고, 스모크 모드에서 지워지는 몬스터마다 `PASS removed monster <이름>` 을 남긴다 —
  `Monster.cpp`·`ObjMgr.cpp` 사이에 상태를 공유하지 않아도 되고, 죽인 몬스터가 그 안에 포함되므로 R5 판정에 충분하다.
- 리스트 정리는 `remove_if` 대신 `list::remove(nullptr)`(같은 효과). `std::find` 를 쓰려고 `ObjMgr.cpp` 에 `<algorithm>` 을 직접 포함했다(PCH 는 그대로).
- `docs/ARCHITECTURE.md` 의 프레임 흐름(4절)·장면 전환 설명·「알려진 문제」 표가 ub-fixes 의 변경(장면 교체 위치, `ColCircle` 호출)을 반영하지 않고 있어 함께 고쳤다.

## 검증 결과 (2026-11-12)

`Exe`·`Temp` 삭제 → restore → 4개 구성 빌드(`-m:1 -nodeReuse:false`) → 4개 구성 스모크 → PowerShell 로 8초 일반 실행.

| spec | 결과 |
|---|---|
| R1 정리 호출 1곳 | ✅ `IterRelease` 는 `ObjMgr.h` 선언, `ObjMgr.cpp` 정의, `Progress` 첫머리 호출뿐 |
| R2·R3·R4 정리 방식 | ✅ 코드 검토 — 수집 → 대상 끊기 → delete → `remove(nullptr)`, 순회 중 `erase` 경로 없음 |
| R5·R6 스모크 | ✅ 4개 구성 모두 `INFO smoke kill Skeleton` → `PASS removed monster Skeleton`(죽인 뒤 약 0.8초) → `RESULT PASS (fail=0)`, 종료 코드 0. 툴 4개 구성 `tiles=800` |
| R7 일반 실행 | ✅ Win32 Debug·Release Client·툴 8초 뒤 살아 있음, `system err` 없음, `smoke.log` 없음, 남은 프로세스 0 |
| R8 경고 | ✅ Win32 196, x64 Debug 268·Release 206 — 변화 없음 |
| R9 문서 | ✅ CLAUDE.md 「테스트」, `docs/ARCHITECTURE.md` 4·7·11절 |

- 경험치 지급 횟수·죽음 애니메이션의 마지막 한 프레임은 눈으로 확인하지 않았다(spec 의 판정 시점 분석으로 대신).
