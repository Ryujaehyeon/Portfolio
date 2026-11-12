# Plan: string-key-compare

Author: Ryu Jahyeon
Status: approved (사용자 위임 — 「진행해」·비교 방식 결정, 2026-11-12)

<!-- 입력: 승인된 spec.md (PR #31). 경로는 2D-Portfolio/ 기준. 줄 번호는 master `be4d380` 기준. -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Headers/Function.h` | `IsSameKey` 추가(R1) |
| `Client/{UIObj,Monster,Player,ObjMgr,MenuButton,BoneSpear}.cpp`, `Client/Obj.h` | 주소 비교 118곳 → `IsSameKey`(R2) |
| `Client/TextureMgr.*`·`MultiTexture.*`, `MFC_000/TextureMgr.*` | `map` 키 `wstring`(R3) |
| `Client/Client.vcxproj` | Release(Win32·x64) `<StringPooling>false</StringPooling>`(R4) |
| `CLAUDE.md` | 「코드 관례」 비교 규칙·Release `/GF-` 이유(R8) |

## 작업 순서

1. **재현(전)** — `StringPooling=false` 만 적용한 Win32 Release 리빌드 → tlog 에 `/GF-`, 스모크 실패 확인(커밋하지 않음).
2. **수정** — `IsSameKey` + 스캔 기반 치환(스크립트: 스캔과 같은 패턴의 피연산자 쌍만 함수 호출로, `ObjMgr.cpp:73-81` 의 `wstring` 비교 제외) + 텍스처 `map` 키.
   → 재스캔 0건(제외 3건만), Win32 Release(`/GF-`)·Debug 스모크 PASS. 커밋 `fix(string-key-compare): 이름·키 문자열을 내용으로 비교`
3. **회귀 감지** — `Client.vcxproj` 커밋 `build(string-key-compare): Client Release 의 문자열 병합을 끈다`(커밋 type 은 규약의 `chore`)
4. **전체 검증**(클린 4개 구성 빌드·스모크·일반 실행) → **문서** → PR → 머지 → 브랜치 삭제.

## 테스트

- 스캔: 피연산자가 `TCHAR*` 식별자(헤더·소스에서 추출한 전수)·이름 상수·`L""` 리터럴인 `==`/`!=` — 수정 후 `ObjMgr.cpp:73,77,81`(`wstring`)만 남는다.
- 재현: 1단계 실패(메뉴에서 30초 타임아웃) → 2단계 후 같은 설정에서 `RESULT PASS (fail=0)`.
- 그 밖은 dead-code-removal·ub-fixes 와 같은 클린 4개 구성 절차. 경고 Win32 196, x64 268·206.

## 리스크

- 스캔이 놓친 비교 — 스모크 경로는 `/GF-` Release 가 잡는다. 스모크 밖은 코드 검토.
- `Obj.h` 의 `IsSameKey` 사용이 `Function.h` 포함 순서에 의존 — 빌드로 확인.
