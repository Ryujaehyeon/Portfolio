# Plan: input-unify

Author: Ryu Jahyeon
Status: approved (PR #54)

<!-- 입력: spec.md. 경로는 2D-Portfolio/ 기준. -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Client/Player.cpp` | `SetKey()` 삭제, `GetMouseState()->rgbButtons[0/1] & 0x80` → `m_dwKey & KEY_LBUTTON/KEY_RBUTTON`(R1) |
| `Client/InputDevice.h/.cpp` | 삭제(R2) |
| `Client/MainGame.cpp` | `CInputDevice` 초기화·`DestroyInstance` 삭제(R2) |
| `Headers/Include.h` | `#include "InputDevice.h"` 삭제(R2) |
| `Client/Client.vcxproj(.filters)` | `InputDevice` 항목, 4개 구성의 `dinput8.lib`·`dxguid.lib` 삭제(R2·R3) |
| `Client/stdafx.h` | `<dinput.h>`·`DIRECTINPUT_VERSION` 삭제(R3) |
| `Headers/Function.h` | `GameAsyncKeyState` 에 `GetActiveWindow() == nullptr` → 0(R4) |
| `CLAUDE.md`, `docs/ARCHITECTURE.md`, `docs/FIX-GUIDE.md` | R7, FIX-GUIDE 「다음 순서」 정리 |

## 작업 순서

1. Player 판정 교체 → DirectInput 제거(R1~R3) → Win32 Debug 빌드·스모크 → 커밋 `refactor(input-unify): 마우스 버튼도 GameAsyncKeyState 로 읽고 DirectInput 을 지운다`
2. 비활성 판정(R4) → 빌드·스모크 → 커밋 `fix(input-unify): 게임 창이 비활성이면 키·버튼 입력을 무시`
3. 클린 4개 구성 빌드·스모크·툴 스모크·일반 실행(R6) → 문서(R7) → PR → 머지. R5 는 사용자 수동 확인을 PR 에 요청한다.

## 테스트

- R2·R3: `git grep -n -i -E "CInputDevice|dinput|DIRECTINPUT" -- 2D-Portfolio` → 결과 없음.
- R6: CLAUDE.md 「빌드」·「테스트」 명령.
- R4: 일반 실행 중 다른 창을 클릭해도 캐릭터가 움직이지 않는지 — 자동 주입은 하지 않는다(사용자 작업 방해 위험). 사용자 확인.

## 검증 결과 (2026-11-12)

| spec | 결과 |
|---|---|
| R1~R3 | ✅ Player 판정 4곳이 `KEY_LBUTTON`/`KEY_RBUTTON`. `CInputDevice`·`dinput`·`DIRECTINPUT` 은 코드에 없음(Engine `stdafx.h` 의 「넣지 않는다」 주석만). `dxguid.lib` 를 빼도 링크됨 |
| R4 | ✅ `GameAsyncKeyState` 에 `GetActiveWindow() == nullptr` → 0(코드 검토) |
| R5 | ⏳ 사용자 수동 확인 대기 — 활성 창 조작, 다른 창 클릭 시 캐릭터가 움직이지 않는지 |
| R6 | 부분 — 클린 4개 구성 경고 0·오류 0, 툴 스모크 4개 구성 `tiles=800`, Client x64 Debug 20/20, Win32 Debug 스모크 PASS. Win32 Release·x64 Release 반복 스모크와 일반 실행은 사용자 지시로 중단(「테스트 그만하고 다음 작업으로」) |
| R7 | ✅ CLAUDE.md·ARCHITECTURE.md·FIX-GUIDE.md |
