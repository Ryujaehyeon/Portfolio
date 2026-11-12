# Plan: smoke-input-isolation

Author: Ryu Jahyeon
Status: approved (사용자 결정 — 「스모크 입력 분리 먼저」, 2026-11-12)

<!-- 입력: spec.md. 경로는 2D-Portfolio/ 기준. -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Headers/Function.h` | `GameAsyncKeyState`, `GameCursorPos`(R1) |
| `Client/KeyMgr.cpp`, `Client/MenuButton.cpp` | `GetAsyncKeyState` → `GameAsyncKeyState`(R2) |
| `Client/Obj.cpp`, `Client/ObjMgr.cpp`, `Client/UIObj.cpp` | `GetCursorPos(&pt)` → `GameCursorPos(g_hWnd, &pt)`(R2) |
| `Client/Device.cpp` | `SetKey` 스모크 분기(R3) |
| `CLAUDE.md` | R7 |

## 작업 순서

1. 함수 추가·호출부 교체·`SetKey` → Win32 Debug 빌드·스모크 → 커밋 `fix(smoke-input-isolation): 스모크 모드에서 실제 키·마우스 입력을 무시`
2. 클린 4개 구성 빌드·툴 스모크·일반 실행, x64 Debug·Win32 Release 스모크 각 50회(사용자 평소 사용 중) → 문서 → PR → 머지.

## 테스트

- R2: `git grep -n -E "GetAsyncKeyState|GetCursorPos" -- 2D-Portfolio/Client` → `Function.h` 의 정의 안뿐.
- R5: 반복 실행 — 실제 클릭·키 주입은 하지 않는다(사용자 작업 방해 위험).
