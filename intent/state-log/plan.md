# Plan: state-log

Author: Ryu Jahyeon
Status: approved (PR #58)

<!-- 입력: spec.md. 경로는 2D-Portfolio/ 기준. -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Client/StateRecorder.h/.cpp`(새, UTF-8 BOM) | R1·R3~R7 |
| `Client/ObjMgr.h` | `GetObjectMap() const` |
| `Client/MainGame.cpp` | `Record()`(장면 갱신 직후), `DestroyInstance`(Release) |
| `Client/SceneMgr.cpp` | `InitScene` 에서 `OnScene(ID)` |
| `Client/Client.cpp` | `-log` → `Begin(L"state.log")` |
| `Headers/Include.h` | client 블록에 `StateRecorder.h` |
| `Client/Client.vcxproj(.filters)` | 새 파일 |
| `.gitignore`, `CLAUDE.md` | R9 |

## 작업 순서

1. 기록기·호출 → Win32 Debug 빌드 → `-smoke -log` 로 로그 확인, `-smoke` 단독 PASS → 커밋 `feat(state-log): -log 로 객체 상태 변화를 state.log 에 기록`
2. 4개 구성 빌드 → 문서 → PR → 머지.

## 테스트

- R8: `cd Client && ../Exe/Win32/Debug/Client/Client.exe -smoke -log; echo $?` → 0, `grep -c '^\[' state.log`, `grep 'scene 2' state.log`.

## 검증 결과 (2026-11-12)

| spec | 결과 |
|---|---|
| R1~R3·R7 | ✅ `CStateRecorder`(공개 접근자만 사용, friend 없음), `CObjMgr::GetObjectMap() const`, `CMainGame::Progress` 장면 갱신 직후 `Record()` |
| R4~R6 | ✅ Win32 Debug `-smoke -log`: 107줄 — `scene 0/1/2`, 생성 38, 삭제 1(처치한 Skeleton), 모션·HP·대상·충돌·이동 변화. 예: 스테이지 첫 프레임에 Player #11 과 Skeleton #6 이 겹쳐 생성돼 바로 충돌·HP 500 → 485 |
| R8 | ✅ 4개 구성 경고 0·오류 0. Win32 Release `-smoke -log` PASS(149줄). `-log` 없는 x64 Debug 스모크 PASS, `state.log` 생기지 않음 |
| R9 | ✅ 루트 `.gitignore` 의 `*.log` 가 이미 포함. CLAUDE.md 「상태 기록」 |

- 관찰: Player 의 HP 자연 회복(약 0.1초마다 +0.5)이 줄의 상당 부분을 차지한다. 필요하면 HP 도 위치처럼 「마지막 기록값에서 일정 이상 변할 때」로 바꾼다.
