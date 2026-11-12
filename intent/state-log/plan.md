# Plan: state-log

Author: Ryu Jahyeon
Status: draft

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
