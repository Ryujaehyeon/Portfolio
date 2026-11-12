# Intent: 스모크를 실제 마우스·키보드 입력과 분리

Author: Ryu Jahyeon
Status: draft

## Problem
스모크가 **사용자의 실제 입력을 받는다** — 무인 검증 결과가 그 시각 사용자가 무엇을 하고 있었는지에 따라 달라진다.
2026-11-12 [member-init](../member-init/plan.md) 검증 중 드문 실패(멈춤·`missing skill`·접근 위반)가 나와 원인을 좇다가, 사용자가 그동안 마우스·키보드를 쓰고 있었다는 점에서 확인했다. 경로는 `2D-Portfolio/` 기준.

- DirectInput 키보드·마우스 장치가 `DISCL_BACKGROUND | DISCL_NONEXCLUSIVE`(`Client/Device.cpp:84-93`) — 게임 창이 비활성이어도 다른 창에서의 클릭·키 입력을 받는다(`CDevice::SetKey`).
- `CKeyMgr::CheckKey` 와 `CMenuButton`(`MenuButton.cpp:61,69`)이 `GetAsyncKeyState` 로 **시스템 전역** 키 상태를 읽는다.
- 커서 좌표를 `GetCursorPos`(`Obj.cpp`·`ObjMgr.cpp`·`UIObj.cpp` 5곳)로 **화면 전역** 좌표에서 읽는다.
- 결과: 스모크 도중 사용자가 왼쪽 클릭하면 플레이어가 커서로 이동하고(시전 끊김 → `missing skill`), 커서를 움직이면 바라보는 방향·상태 키가 바뀌며, TAB·SPACE·F1~F4 가 달리기·공격·캐릭터 선택을 바꾼다.
  변경 전·후 비교 실험(변경 전 x64 Debug 96/96, 변경 후 95/96 등)도 그 시각의 사용자 활동이 섞여 믿을 수 없게 됐다.

## Proposed outcome
- `-smoke` 로 실행하면 실제 키·마우스 버튼은 「아무것도 안 눌림」, 커서는 게임 창 가운데에 고정된 것으로 본다. 스모크 결과가 사용자 활동과 무관해진다.
- `-smoke` 없는 일반 실행의 입력 동작은 그대로다.
- 이후 member-init 을 이 스모크로 변경 전·후 다시 비교해 머지 여부를 정한다.

## Affected users and systems
사용자: 이 repo 개발자(검증 중에도 PC 를 쓸 수 있고, 결과를 믿을 수 있다). 게임 사용자에게는 변화 없음
시스템: `Headers/Function.h`(입력 읽기 함수), `Client/KeyMgr.cpp`·`Device.cpp`·`MenuButton.cpp`·`Obj.cpp`·`ObjMgr.cpp`·`UIObj.cpp`, CLAUDE.md

## Constraints
반드시 지킬 것:
- 스모크 분기는 `IsSmokeMode()` 안에만. 일반 실행 동작 불변. 경고 0.
- 검증 중 실제 클릭·키를 주입하지 않는다(사용자 작업을 방해할 수 있다).

범위 밖:
- 일반 실행에서도 게임 창이 비활성일 때 입력을 무시하게 하는 것(`DISCL_FOREGROUND`, 포커스 확인) → `input-unify`.
- 텍스처 프레임 범위 초과(member-init 검증 중 Debug 경고창으로 발견) → 별도 인텐트.

## Open questions
없음 — 사용자 결정(2026-11-12, 「스모크 입력 분리 먼저」).
