# Spec: err-msg-format

Author: Ryu Jahyeon
Status: draft

<!-- 입력: intent/err-msg-format/intent.md (PR #11 머지로 승인, 2026-11-10). 경로는 2D-Portfolio/ 기준. -->

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | `ERR_MSG(hwnd, fmt, ...)` 가 형식 문자열과 가변 인자를 받아 조립한 문자열을 메시지박스로 보여 준다 | 코드 검토 + R6 |
| R2 | Debug\|Win32 Rebuild 에서 C4002 가 0건이고, 전체 경고가 282 → 198 이다(다른 경고는 늘지도 줄지도 않음) | 빌드 출력 경고 코드별 집계 비교 |
| R3 | `Client/Stage.cpp` 의 포맷 인자 84곳이 인자 타입에 맞는 지정자를 쓴다 — 인자가 모두 `TCHAR*` 이므로 `%s` | `git grep -n "ERR_MSG(.*%d" -- 2D-Portfolio` 결과 0건, `%s` 84건 |
| R4 | 포맷 인자 없는 기존 호출부(Client·MFC_000 약 130곳)는 수정 없이 컴파일되고 같은 메시지를 보여 준다 | `git diff --stat` 에 `Headers/` 2개와 `Client/Stage.cpp` 외 소스 없음 + Debug·Release 두 프로젝트 빌드 성공 |
| R5 | 메시지박스 제목(`system err`)·버튼(`MB_OK`)·호출 뒤 흐름(`return E_FAIL` 등)이 그대로다 | 코드 검토(diff 에 호출부 흐름 변경 없음) |
| R6 | 포맷 인자를 넘긴 호출이 실패하면 메시지박스에 리소스 이름이 보인다(예: `Skeleton Run L 로드 실패`) | 수동: 로컬에서만 `Stage.cpp` 의 리소스 경로 하나를 일시적으로 틀리게 해 실행 → 메시지 확인 → 되돌림(커밋 안 함). `Resource/` 는 건드리지 않는다 |
| R7 | Release 에서도 R1 이 동작한다(디버그 전용 코드가 아님) | Release 빌드 성공 + `ErrMsgBox` 가 `_DEBUG` 조건 밖에 있음(코드 검토) |

## 설계

```cpp
// Headers/Function.h — 새 함수(Client·MFC_000 공용, Defines.h 경유)
inline void ErrMsgBox(const HWND _hWnd, const TCHAR* const _pFormat, ...)
{
	TCHAR szMsg[256];
	va_list ap;
	va_start(ap, _pFormat);
	_vsnwprintf_s(szMsg, _TRUNCATE, _pFormat, ap);
	va_end(ap);
	MessageBox(_hWnd, szMsg, L"system err", MB_OK);
}

// Headers/Macro.h:22 — 이름과 호출 모양은 유지, 함수로 넘긴다
#define ERR_MSG(hwnd, fmt, ...) ErrMsgBox(hwnd, fmt, __VA_ARGS__)
```

- **매크로 본문 대신 인라인 함수**(사용자 결정, 2026-11-10) — 인자 타입(`HWND`·`const TCHAR*`)이 컴파일러로 검사되고,
  디버거로 들어갈 수 있으며, 인자가 한 번만 평가된다(PIE). 매크로는 호출부 약 210곳을 바꾸지 않으려는 얇은 이름 유지 용도로만 남긴다(KISS).
- **위치는 `Function.h`** — 자유 함수는 이미 이 파일(`SAFE_DELETE`)에 모인다. `Defines.h` 가 `Macro.h` 다음에 include 하지만
  매크로는 호출 지점에서 펼쳐지므로 순서 문제는 없다. 두 프로젝트의 `stdafx.h` 가 `<windows.h>`(MFC 는 `<afxwin.h>`)를 먼저 넣어
  `HWND`·`MessageBox`·`va_list`·`_vsnwprintf_s` 가 보인다.
- **조립은 `_vsnwprintf_s(..., _TRUNCATE, ...)`** — 넘치면 잘라낸다(`DebugOut` 과 같은 이유: `vswprintf_s` 는 넘치면 Debug CRT 가 중단).
  버퍼 256자 — 가장 긴 메시지도 수십 자.
- **2인자 호출이 그대로 동작하는 근거** — 현재 2인자 호출의 형식 문자열은 모두 `L"..."` 리터럴이고 `%` 를 포함한 것이 없다
  (`git grep` 확인). 따라서 형식 처리를 거쳐도 같은 문자열이 나온다. `__VA_ARGS__` 가 비면 MSVC 기존 전처리기가 앞의 쉼표를 지운다.
- **호출부는 `%d` → `%s` 만** — 84곳의 문구·인자·흐름은 그대로(최소 변경). 다른 `ERR_MSG` 호출은 건드리지 않는다.
- **새 코드 품질 규칙**(CLAUDE.md 「새로 만드는 클래스·파일」): 매개변수 `const`(I-26), 경고 0(I-56), 이름은 주변 관례(`_` 접두 인자, 헝가리안 지역 변수).
- 추상화를 더 두지 않는다(YAGNI) — 로그 파일 기록·제목 인자화 같은 확장은 요구가 생기면 그때.

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| 가변 인자는 형식 지정자와 인자 타입 불일치를 컴파일러가 잡지 못한다(`%d` 에 문자열 → 쓰레기 값, `%s` 에 정수 → 크래시 가능) | 정확성 | R3 grep 으로 이번 84곳 확인. 이후 새 호출의 불일치는 수용(리뷰에서 확인) |
| 형식 문자열이 리터럴이 아니면 `%` 가 든 문자열을 그대로 넘길 때 오작동 | 정확성·보안 | 현재 0건. 수용 — 외부 입력을 메시지로 넘기는 호출 없음 |
| `__VA_ARGS__` 빈 경우 쉼표 제거는 MSVC 기존 전처리기 동작 | 이식성 | 수용. `/Zc:preprocessor` 를 켜는 작업(ConformanceMode 와 함께)에서 다시 본다 |
| `Headers/` 는 두 프로젝트 공용 — MFC_000 에도 영향 | 회귀 | R4 에서 MFC_000 빌드 포함. MFC_000 의 2곳은 2인자 호출 |
| 메시지 256자 초과 시 잘림 | 정확성 | 수용. 현재 메시지는 수십 자 |
| `Stage.cpp` 84줄 동시 수정 — diff 가 크다 | 리뷰 부담 | 수용. 줄마다 `%d`→`%s` 한 글자 쌍뿐이라 기계적으로 확인 가능 |
| `Function.h`·`Macro.h` 에 UTF-8 BOM 없음 | 인코딩 관례 | 추가하는 내용이 ASCII 라 문제없음. BOM 은 붙이지 않는다(최소 변경) |
