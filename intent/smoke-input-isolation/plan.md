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

## 검증 결과 (2026-11-12)

| spec | 결과 |
|---|---|
| R1·R3 | ✅ `GameAsyncKeyState`·`GameCursorPos`(`Function.h`), `CDevice::SetKey` 스모크 분기 |
| R2 | ✅ `GetAsyncKeyState`·`GetCursorPos` 직접 호출은 `Function.h` 정의 안뿐 |
| R4 | ✅ 일반 실행 4개 구성 × Client·툴 살아 있음(스모크가 아니면 원래 API 그대로) |
| R5 | ✅ 사용자가 평소처럼 PC 를 쓰는 동안 x64 Debug 50/50. Win32 Release 49/50 — 1회 접근 위반(아래) |
| R6 | ✅ 클린 4개 구성 경고 0·오류 0, 툴 스모크 4개 구성 `tiles=800`, Win32 Debug 5/5, x64 Release 10/10 |
| R7 | ✅ CLAUDE.md 「테스트」·「실행」 |

- **Win32 Release 1/50 접근 위반(종료 코드 5, 스킬 생성 직후 6.6초)** — 입력이 분리된 상태라 실제 크래시다. 이 빌드는 member-init 전 소스 + 입력 분리라 **기존 버그**.
  member-init 검증 중 Debug 에서 경고창으로 잡힌 텍스처 프레임 범위 초과(`CPlayer::Render` → `CMultiTexture::GetTexture`, `vector subscript out of range`)가 Release 에서 크래시로 나타난 것으로 추정 — 별도 인텐트에서 스택으로 확인한다.
