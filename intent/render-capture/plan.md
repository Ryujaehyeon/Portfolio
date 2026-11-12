# Plan: render-capture

Author: Ryu Jahyeon
Status: draft

<!-- 입력: spec.md. 경로는 2D-Portfolio/ 기준. -->

## 영향받는 파일

| 파일 | 할 일 | 요구 |
|---|---|---|
| `Headers/FixedStep.h`(새) | 결정적 모드 상태, `IsFixedStepMode()`·`GameTickMs()`·프레임 진행·캡처 시점 표 | R1~R3 |
| `Headers/Function.h` 또는 `Defines.h` | 새 헤더 포함 | — |
| `Client/Client.cpp` | `-shot <폴더>` 해석, `srand` 분기, 메인 루프에서 프레임 진행·캡처 요청, 스모크 일정의 시계 교체 | R1~R4 |
| `Client/TimeMgr.cpp` | `DeltaTime()` 결정적 모드 분기 | R1 |
| `Client/Monster.cpp`·`Client/Player.cpp` | 스모크 일정의 `GetTickCount64` → `GameTickMs` | R2 |
| `Engine/Device.h/.cpp` | `RequestCapture(경로)`, `Render_End` 에서 저장 | R4 |
| `scripts/compare-shots.ps1`(새) | 비교 스크립트 | R6 |
| `.gitignore` | `2D-Portfolio/Client/shots/` | R8 |
| `CLAUDE.md` | 「테스트」에 캡처·비교 사용법 | R8 |

## 작업 순서

1. **결정적 모드와 게임 시계.** `FixedStep.h`, 시드·`DeltaTime`·스모크 일정 교체.
   → Win32 Debug 빌드, 기존 스모크 PASS(R7).
   → 커밋 `feat(render-capture): -shot 으로 시드와 프레임 시간을 고정하는 결정적 실행 모드`
2. **캡처.** `CDevice::RequestCapture`·저장, 메인 루프의 시점 표.
   → `-smoke -shot shots/a` 실행, 이미지 수와 내용 확인(직접 열어 본다).
   → 커밋 `feat(render-capture): 정해 둔 프레임마다 백버퍼를 PNG 로 저장`
3. **비교 스크립트.**
   → 같은 빌드로 `shots/a`·`shots/b` 를 찍어 비교하면 완전 일치(R5).
   → 일부러 다른 이미지를 넣어 실패·diff 이미지가 나오는지 확인(R6).
   → 커밋 `feat(render-capture): 캡처 폴더를 픽셀 단위로 비교하는 스크립트`
4. **마무리.** 4개 구성 빌드·기존 스모크(R7·R8), 문서, PR, 머지.

## 캡처 시점(초안 — 2번에서 실제 화면을 보고 확정)

| 장면 | 진입 후 프레임 | 담기는 것 |
|---|---|---|
| 0 로고 | 5 | 로고 배경 |
| 1 메뉴 | 30 | 메뉴 배경·버튼 |
| 2 스테이지 | 10 | 시작 직후 — 플레이어·몬스터 겹침, UI |
| 2 스테이지 | 100 | 몬스터 공격 중 |
| 2 스테이지 | 150 | 스킬(BoneSpear) 시전 중 — 2초 = 120프레임 뒤 |
| 2 스테이지 | 220 | 몬스터 처치·스킬 이동 뒤 |

## 테스트

```
cd 2D-Portfolio/Client
../Exe/Win32/Debug/Client/Client.exe -smoke -shot shots/a; echo $?    → 0
../Exe/Win32/Debug/Client/Client.exe -smoke -shot shots/b; echo $?    → 0
powershell.exe -NoProfile -File ../../scripts/compare-shots.ps1 -Expected shots/a -Actual shots/b; echo $?   → 0, 모든 이미지 다른 픽셀 0
```

- R5 가 실패하면(같은 빌드인데 다르면) diff 이미지로 어느 객체가 다른지 보고, `state.log`(`-log`)를 두 실행에서 비교해 처음 갈라지는 프레임을 찾는다.
