# Plan: err-msg-format

Author: Ryu Jahyeon
Status: approved (사용자, 2026-11-10) · 구현·검증 완료(2026-11-10)

<!-- 입력: intent/err-msg-format/spec.md (PR #12 머지로 승인, 2026-11-10). 경로는 2D-Portfolio/ 기준.
     승인되면 feature/err-msg-format 브랜치에서 이 내용을 intent/err-msg-format/plan.md 로 저장해 코드와 함께 커밋한다. -->

## Context

`ERR_MSG(hwnd, sz)` 가 인자 2개짜리 매크로라 `Client/Stage.cpp` 84곳이 넘기는 리소스 이름이 버려지고(C4002 84건),
메시지박스에 `%d`·`%s` 가 글자 그대로 찍힌다. spec 대로 가변 인자 인라인 함수 `ErrMsgBox` 를 만들고 `ERR_MSG` 를 그 함수로 넘기며,
84곳의 `%d`(인자가 모두 `TCHAR*`)를 `%s` 로 고친다. 결과: 실패 메시지에 리소스 이름이 보이고 Debug 경고 282 → 198.

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Headers/Function.h` | 끝에 `inline void ErrMsgBox(const HWND _hWnd, const TCHAR* const _pFormat, ...)` 추가 (spec 「설계」 코드 그대로) |
| `Headers/Macro.h:22` | `#define ERR_MSG(hwnd, sz) MessageBox(...)` → `#define ERR_MSG(hwnd, fmt, ...) ErrMsgBox(hwnd, fmt, __VA_ARGS__)` |
| `Client/Stage.cpp` | `ERR_MSG(g_hWnd, L"%d ` → `ERR_MSG(g_hWnd, L"%s ` 82줄(62~1636줄 범위). 이미 `%s` 인 2줄(62·74)은 그대로 |
| `intent/err-msg-format/plan.md` | 이 계획 저장, 끝에 검증 결과 기록 |

조사로 확인한 사실:
- 포맷 인자를 넘기는 84줄은 모두 `ERR_MSG(g_hWnd, L"%d ` 또는 `L"%s ` 로 시작한다(예외 0) → 한 패턴 치환으로 끝난다.
- `Function.h`·`Macro.h` 는 BOM 없음(ASCII). 추가 내용도 ASCII 라 BOM 을 붙이지 않는다(spec flag). `Stage.cpp` 는 BOM 있음 — 유지.
- `Defines.h` 가 `Macro.h` → `Function.h` 순으로 include, 두 프로젝트 `stdafx.h` 가 Windows 헤더를 먼저 넣는다.

## 작업 순서

0. **브랜치**: `master`(`3141e64`)에서 `feature/err-msg-format`.
1. **기준선**: Debug|Win32 Rebuild(`-m:1 -nodeReuse:false`, `CL` 미설정) → 경고 282(C4002 84·C4244 177·C4838 18·C4715 2·C4018 1) 확인.
2. **`ErrMsgBox` 추가 + `ERR_MSG` 변경**(`Function.h`, `Macro.h`) → 빌드: 오류 0, 경고 수는 아직 282 이하
   (매크로가 가변 인자가 되므로 C4002 84 는 여기서 사라지고 198 이 기대값).
3. **`Stage.cpp` `%d` → `%s`**: `sed -i 's/ERR_MSG(g_hWnd, L"%d /ERR_MSG(g_hWnd, L"%s /'` 후
   → `git diff --stat` 이 82줄 변경, BOM(`efbbbf`) 유지, `git grep -n "ERR_MSG(.*%d" -- 2D-Portfolio` 0건, `%s` 84건.
4. **빌드 전부**: Debug·Release Rebuild → 종료 코드 0, Debug 경고 198(C4002 0, 나머지 코드별 수 불변).
5. **실행 스모크**: Debug·Release 각각 `cd Client && timeout 8 ../Exe/Win32/<구성>/Client/Client.exe` 상당(창 응답·제목 확인, 기존 ODS 리스너 스크립트 사용) → 오류 메시지박스 없이 메뉴까지.
6. **R6 수동 확인**(사용자): 로컬에서만 `Stage.cpp:406` 경로를 `Stand_D_X_%02d.png` 로 바꾼 Debug 빌드 →
   VS F5 → 「시작」 → 메시지박스 `Player Stand D 로드 실패`(제목 `system err`) 확인 → **되돌리고** 재빌드, `git diff` 에 그 줄 없음 확인.
7. **plan.md 저장 + 검증 결과 기록 → 커밋** `fix(err-msg-format): ERR_MSG 가 포맷 인자를 받도록 ErrMsgBox 로 전환` → PR.

## 테스트

```
MSBuild.exe 2D-Portfolio/MyProgram.sln -t:Rebuild -p:Configuration=Debug   -p:Platform=Win32 -m:1 -nodeReuse:false -nologo -v:m
MSBuild.exe 2D-Portfolio/MyProgram.sln -t:Rebuild -p:Configuration=Release -p:Platform=Win32 -m:1 -nodeReuse:false -nologo -v:m
git grep -n "ERR_MSG(.*%d" -- 2D-Portfolio      # 기대: 0건
git grep -c "ERR_MSG(g_hWnd, L\"%s " -- 2D-Portfolio/Client/Stage.cpp   # 기대: 84
git diff --stat master                           # 기대: Headers/Function.h, Headers/Macro.h, Client/Stage.cpp, intent/err-msg-format/plan.md 만
```
기대: 두 구성 종료 코드 0·오류 0. Debug 경고 198 = C4244 177 + C4838 18 + C4715 2 + C4018 1, C4002 0.
MFC_000 도 같은 솔루션에서 빌드됨(2인자 호출 2곳 컴파일 확인).

| spec | 확인 단계 |
|---|---|
| R1 형식 조립 | 2 코드 + 6 |
| R2 C4002 0·경고 198 | 4 |
| R3 `%d` 0·`%s` 84 | 3 |
| R4 기존 호출 무수정 | 4 + `git diff --stat` |
| R5 제목·흐름 불변 | diff 검토(호출부는 지정자 글자만 변경) |
| R6 리소스 이름 표시 | 6 (사용자 수동) |
| R7 Release 동작 | 4 Release 빌드 + `ErrMsgBox` 가 `_DEBUG` 밖 |

## 리스크

- **`__VA_ARGS__` 빈 경우 쉼표**: MSVC 기존 전처리기가 지우는 데 기댄다 → 2단계 빌드에서 2인자 호출 약 130곳이 컴파일되면 확인됨. 실패 시 `ERR_MSG(hwnd, ...) ErrMsgBox(hwnd, __VA_ARGS__)` 형태로 바꿔 쉼표 문제 자체를 없앤다.
- **MFC_000 에서 `TCHAR`/`_vsnwprintf_s` 가 안 보일 가능성**: MFC `stdafx.h` 의 `<afxwin.h>` 가 포함하므로 낮음. 실패하면 `Function.h` 에 필요한 표준 헤더(`<stdarg.h>`·`<stdio.h>`)를 include.
- **R6 테스트 중 `InitScene` 실패 후 상태**: 텍스처 실패 시 `CStage::Initialize` 가 `E_FAIL` 을 돌려도 호출부가 무시해 이후 동작이 이상할 수 있다 — 메시지만 확인하고 종료(기존 동작, 범위 밖 `ub-fixes`).
- **MSBuild 노드 재사용**: 이전 세션처럼 다른 환경 변수(`CL`)가 남은 노드를 쓰면 경고 수가 달라진다 → 경고 판정 빌드는 `-m:1 -nodeReuse:false`.
- **sed 가 줄바꿈(CRLF)·BOM 을 바꿀 가능성**: GNU sed 는 둘 다 보존 — 3단계에서 `git diff --stat` 82줄과 BOM 으로 확인.

## 검증 결과 (2026-11-10)

| spec | 결과 |
|---|---|
| R1 형식 조립 | ✅ `Headers/Function.h` `ErrMsgBox` + `Headers/Macro.h:22` `ERR_MSG(hwnd, fmt, ...)` |
| R2 C4002 0·경고 198 | ✅ Debug·Release Rebuild(`-m:1 -nodeReuse:false`) 모두 198 = C4244 177 · C4838 18 · C4715 2 · C4018 1, C4002 0 (기준선 282). 2단계(헤더만 변경) 시점에 이미 198 |
| R3 `%d` 0·`%s` 84 | ✅ `git grep "ERR_MSG(.*%d"` 0건, `Stage.cpp` `%s` 84건. 변경 82줄 모두 지정자 한 글자 차이, BOM 유지 |
| R4 기존 호출 무수정 | ✅ 소스 변경은 `Headers/Function.h`·`Headers/Macro.h`·`Client/Stage.cpp` 뿐, 2인자 호출(Client·MFC_000) 컴파일 성공 |
| R5 제목·흐름 불변 | ✅ 제목 `system err`·`MB_OK` 유지, 호출부는 지정자만 변경 |
| R6 리소스 이름 표시 | ✅ 사용자 확인 — `Stage.cpp:406` 경로를 로컬에서 일시 변경 → 「시작」 → 메시지박스 `Player Stand D 로드 실패`(제목 `system err`). 변경은 되돌림(커밋 안 함) |
| R7 Release 동작 | ✅ Release 빌드 성공, `ErrMsgBox` 는 `_DEBUG` 조건 밖 |
| 실행 스모크 | ✅ Debug·Release 8초 생존, 보이는 창 `Client`(Debug 는 콘솔 포함)뿐 — `system err` 없음. Debug 출력 창 `Scene : 0`·`Scene : 1` |
