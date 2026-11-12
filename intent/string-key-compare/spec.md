# Spec: string-key-compare

Author: Ryu Jahyeon
Status: draft

<!-- 입력: 승인된 intent.md (PR #30). 경로는 2D-Portfolio/ 기준. -->

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | `Headers/Function.h` 에 `IsSameKey(const TCHAR*, const TCHAR*)` 가 있다 — 둘 다 nullptr 이 아니면 `_tcscmp(a, b) == 0`, 하나라도 nullptr 이면 두 포인터가 같을 때만 참 | 코드 검토 |
| R2 | intent 의 118곳이 `IsSameKey(a, b)`(`==`)·`!IsSameKey(a, b)`(`!=`)로 바뀐다. 피연산자 순서·조건식의 나머지는 그대로다 | 스캔 스크립트(아래 테스트) 결과 0건 |
| R3 | `CTextureMgr`(Client·MFC_000)·Client `CMultiTexture` 의 `map` 키가 `wstring` 이다. 조회·삽입 인자 타입(`const TCHAR*`)은 그대로다 | 코드 검토 |
| R4 | `Client.vcxproj` 의 Release(Win32·x64) 컴파일 설정에 `<StringPooling>false</StringPooling>` — 실제 명령줄에 `/GF-` 가 들어간다 | tlog 확인 |
| R5 | **재현 테스트**: R2·R3 없이 R4 만 적용한 Win32 Release 는 스모크가 실패하고(메뉴에서 멈춤), R2·R3 를 적용하면 통과한다 | 스모크 2회(전·후) |
| R6 | 4개 구성 클린 빌드 오류 0, 경고가 늘지 않는다(Win32 196, x64 Debug 268·Release 206) | 빌드 로그 |
| R7 | 4개 구성 Client·툴 스모크 PASS, 일반 실행 8초 살아 있음 | CLAUDE.md 「테스트」·「실행」 |
| R8 | CLAUDE.md 「코드 관례」에 「이름·키 문자열은 `IsSameKey` 로 비교, `==` 금지」와 Release `/GF-` 의 이유가 있다 | 문서 검토 |

## 설계

- **비교 함수 하나**(`Headers/Function.h`, Client·MFC_000 공용) — 118곳이 같은 지식(「키는 내용으로 비교」)이므로 한 곳에 둔다(DRY).
  `_tcscmp` 를 직접 쓰지 않는 이유: nullptr 일 수 있는 포인터(`m_SkillActiveName` 등)가 있고, 「같다」를 읽기 쉽게 표현한다(PIE).
  이름은 주변 관례(`SAFE_DELETE`, `ErrMsgBox`)와 같은 파스칼 표기.
- **기계적 치환**: 스캔 스크립트(피연산자가 `TCHAR*` 식별자·이름 상수·`L""` 리터럴인 `==`/`!=`)로 찾은 줄만, 그 피연산자 쌍을 함수 호출로 바꾼다.
  치환 후 같은 스캔이 0건이어야 한다. `wstring` 비교(`ObjMgr.cpp:73-81`)는 스캔 예외로 둔다.
- **텍스처 `map` 키를 `wstring`** — `find(pObjKey)`·`insert(make_pair(pObjKey, …))` 가 `wstring` 을 만들어 쓰므로 호출부·인터페이스는 바뀌지 않는다.
  Client·MFC_000 의 `CTextureMgr` 는 사본이라 둘 다 고친다(CLAUDE.md 「중복 코드」).
- **회귀 감지(R4)**: Client Release 만 `/GF-`. Debug 는 `/ZI`(편집하며 계속하기)가 문자열 병합을 요구해 끌 수 없다고 보고 그대로 둔다 — Release 스모크 2개 구성이 감지한다.
  MFC_000 은 비교가 없고, 툴 스모크는 텍스처 조회를 지나가지 않아 감지 효과가 없으므로 그대로 둔다.

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| 스캔이 놓친 주소 비교가 남을 수 있음 | 완전성 | 스캔은 `TCHAR*` 를 다루는 식별자 전수(헤더·소스에서 추출)를 기준으로 한다. 놓친 것 중 스모크 경로에 있는 것은 R4·R5 의 `/GF-` Release 스모크가 잡는다. 스모크 밖(UI 클릭 등)은 코드 검토 |
| 문자열 비교 비용 | 성능 | 프레임당 수백 회 이하의 짧은 문자열 비교 — 무시할 수준 |
| Release `/GF-` 로 같은 리터럴이 중복 저장 | 실행 파일 크기 | 수용 — 수 KB 수준. 회귀 감지 이득이 크다 |
| `map` 키가 `wstring` 이 되면서 정렬 순서가 바뀜 | 동작 불변 | `map` 은 조회와 해제 순회에만 쓰인다 — 순서에 의존하는 코드 없음 |
