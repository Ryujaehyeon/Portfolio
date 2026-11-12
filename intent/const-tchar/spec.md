# Spec: const-tchar

Author: Ryu Jahyeon
Status: draft

<!-- 입력: 승인된 intent.md (PR #33). 경로는 2D-Portfolio/ 기준. -->

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | 이름 상수(`Headers/Constant.h` 38개)가 `const TCHAR* const` 다 | 코드 검토 |
| R2 | 문자열을 읽기만 하는 멤버·인자·반환·지역 포인터가 `const TCHAR*` 다 — 적어도 `/permissive-` 오류를 낸 모든 곳과, 그것을 위해 타입이 바뀌어야 하는 연쇄(멤버 `m_pObjName`·`m_pObjKey`·`m_pStateKey`·`m_pMotion`, 생성자·`Set*` 인자, 텍스처 관리자 인자 등) | `/permissive-` 빌드 오류 0 |
| R3 | 함수가 돌려준 임시 값의 주소를 받지 않는다 — C2102 16곳은 지역 변수에 받아 그 주소를 넘긴다 | `/permissive-` 빌드 오류 0 |
| R4 | `const_cast` 를 새로 쓰지 않는다 | `git diff` 에 `const_cast` 없음 |
| R5 | `Client.vcxproj`·`MFC_000.vcxproj` 의 4개 구성 모두 `<ConformanceMode>true</ConformanceMode>` — 실제 명령줄에 `/permissive-` | tlog |
| R6 | 4개 구성 클린 빌드 오류 0. 경고가 늘지 않는다(Win32 196, x64 Debug 268·Release 206). `/permissive-` 로 새로 생기는 경고가 있으면 고친다 | 빌드 로그 |
| R7 | 4개 구성 Client 스모크(구성마다 3회)·툴 스모크 PASS, 일반 실행 8초 살아 있음 | CLAUDE.md 「테스트」·「실행」 |
| R8 | CLAUDE.md 「빌드」의 ConformanceMode 설명이 「켜져 있다」로 바뀌고, 「코드 관례」의 문자열 항목에 「읽기 전용은 `const TCHAR*`」가 들어간다 | 문서 검토 |

## 설계

- **방법: 컴파일러가 가리키는 대로** — `_CL_=/permissive-` 로 빌드해 오류가 난 곳의 타입에 `const` 를 붙이고, 그 변경이 낳는 새 오류(연쇄)를 같은 방식으로 따라간다. 오류 0 이 될 때까지 반복한 뒤 vcxproj 에 켠다.
  - 멤버를 `const TCHAR*` 로 바꾸면 그것을 받는 생성자·`Set*`·`InsertTexture` 등의 인자도 `const` 가 된다. 반환형(`GetName` 등)은 이미 `const TCHAR*` 다.
  - 문자열을 실제로 고치는 버퍼(`TCHAR szBuf[]`, `wcscpy_s` 대상 등)는 그대로 둔다.
- **C2102** — `PtInRect(&RealRect(), pt)` → `const RECT rcReal = RealRect(); PtInRect(&rcReal, pt)`. 같은 함수 안에서 이름이 겹치면 구분되는 이름을 쓴다(변수 가리기 금지, I-57).
- **`Debug.hpp` 의 `__writew(wchar_t*, …)`** → `const wchar_t*`(포맷 문자열은 읽기만 한다). `__writea` 도 같이.
- **ConformanceMode** 는 마지막에 vcxproj 에 켠다 — 그 전까지는 기존 설정으로 빌드·스모크가 되는 상태를 유지한다.
- 코드는 Client·MFC_000·Headers 를 함께 고친다. 중복 코드(`Device`·`Texture` 등)는 오류가 난 쪽만 고친다 — 다른 쪽이 오류가 없으면 그대로 둔다(동기화하지 않음, CLAUDE.md 「중복 코드」).

원칙 근거: 타입이 의도(읽기 전용)를 말하게 한다(PIE). 새 추상화는 없다.

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| 연쇄 변경이 커져 많은 파일을 건드림 | 최소 변경 | 컴파일러 오류가 요구하는 곳만 바꾼다. 오류와 무관한 `TCHAR*` 는 그대로 둔다 |
| `/permissive-` 가 const 외의 동작(두 단계 이름 조회 등)을 바꿀 수 있음 | 동작 불변 | 시험 빌드에서 그 밖의 오류는 없었다. 4개 구성 스모크로 확인 |
| 툴(MFC_000) 의 동작은 스모크가 타일 읽기만 본다 | 검증 범위 | 툴 변경은 C2102 6곳(지역 변수로 받기)뿐 — 의미 불변. 일반 실행으로 창이 뜨는지 확인 |
