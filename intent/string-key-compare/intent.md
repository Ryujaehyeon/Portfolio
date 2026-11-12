# Intent: 이름·키 문자열을 주소가 아니라 내용으로 비교

Author: Ryu Jahyeon
Status: draft

## Problem
게임이 객체 이름·텍스처 키·모션을 **문자열 내용이 아니라 포인터 주소로 비교한다.** 컴파일러가 같은 문자열 리터럴을 한 주소로 합쳐 줄 때만(`/GF` 문자열 병합)
동작한다. 근거: [legacy-audit/review.md](../legacy-audit/review.md) 2번, 2026-11-12 `master`(`5733f35`)에서 다시 확인. 경로는 `2D-Portfolio/` 기준.

- **`==`/`!=` 비교 118곳**(Client 7개 파일: `UIObj.cpp` 44, `Monster.cpp` 33, `Player.cpp` 25, `ObjMgr.cpp` 9, `MenuButton.cpp` 5, `BoneSpear.cpp` 1, `Obj.h` 1).
  피연산자가 `TCHAR*` 인 이름·키·모션(`m_pMotion`, `m_pObjKey`, `GetName()` 등)이고 다른 쪽이 이름 상수(`Headers/Constant.h` 의 `TCHAR* const` 38개)나 `L"…"` 리터럴이다.
  `ObjMgr.cpp:73-81` 의 `pObjKey == BONESPEAR` 는 `pObjKey` 가 `wstring` 이라 이미 내용 비교다(대상 아님). 툴(MFC_000)에는 이런 비교가 없다.
- **텍스처 조회도 주소에 기댄다** — `CTextureMgr`(Client·MFC_000 사본)와 Client `CMultiTexture` 가 `map<const TCHAR*, …>` 로 키를 **주소**로 찾는다.
- **재현됨**: `/GF-`(문자열 병합 끔)를 명령줄 끝에 붙여(`_CL_=/GF-`) Win32 Release 를 빌드하면 스모크가 메뉴에서 멈춘다 —
  메뉴 「시작」 버튼의 `m_pObjKey == L"MenuButton_Start"`(`MenuButton.cpp`)가 거짓이 돼 스테이지로 가지 못하고 30초 타임아웃(`FAIL missing scene 2 init` 외 8건).
  지금은 Debug(`/ZI`)·Release(`/O2`)가 `/GF` 를 암묵적으로 켜서 우연히 동작한다. 빌드 옵션을 바꾸거나 다른 모듈의 문자열이 들어오면 **조용히** 깨진다.
- repo 전체에 `_tcscmp`/`lstrcmp`/`wcscmp` 사용이 0건이다.

## Proposed outcome
- 이름·키·모션 비교와 텍스처 조회가 문자열 **내용**으로 이뤄진다. 문자열 병합 여부와 무관하게 같은 결과를 낸다.
- `/GF-` 로 빌드해도 스모크가 통과한다. 앞으로 주소 비교가 다시 생기면 스모크가 바로 잡는다(Release 에 문자열 병합을 끈 상태로 고정).
- 게임 동작은 그대로다.

## Affected users and systems
사용자: 게임·툴 사용자(보이는 변화 없음), 이 repo 개발자(빌드 옵션에 따라 조용히 깨지는 함정 제거)
시스템: `Headers/Function.h`(비교 함수), Client 7개 파일의 비교, `Client/TextureMgr.*`·`MultiTexture.*`, `MFC_000/TextureMgr.*`, `Client/Client.vcxproj`(Release 문자열 병합), CLAUDE.md

## Constraints
반드시 지킬 것:
- 동작 불변 — 4개 구성 스모크 PASS. 경고가 늘지 않는다. 고치는 줄에는 CLAUDE.md 「C++ 코딩 표준」 품질 규칙.
- 기존 명명·구조 관례(`TCHAR*` 멤버, 이름 상수)는 유지한다 — 비교만 바꾼다.

범위 밖:
- `TCHAR*` 의 `const` 화와 ConformanceMode(`/permissive-`) 켜기 → 바로 다음 인텐트 `const-tchar`(오딧 13번, 켜면 지금 오류 282건).
- 모션·상태를 `enum` 으로 바꾸는 구조 변경(오딧의 장기 방향) — 사용자 결정으로 이번에는 하지 않는다.
- 경고 정리(→ `warning-cleanup`).

## Open questions
없음 — 사용자 결정(2026-11-12): `Headers/Function.h` 에 nullptr 안전한 내용 비교 함수를 두고 주소 비교를 모두 바꾼다. Release 는 문자열 병합을 끈 상태(`/GF-`)로 고정해 회귀를 스모크가 잡게 한다.
