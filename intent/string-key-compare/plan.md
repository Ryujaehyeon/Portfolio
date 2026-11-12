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
3. **회귀 감지** — `Client.vcxproj` 커밋 `chore(string-key-compare): Client Release 의 문자열 병합을 끈다`
4. **전체 검증**(클린 4개 구성 빌드·스모크·일반 실행) → **문서** → PR → 머지 → 브랜치 삭제.

## 테스트

- 스캔: 피연산자가 `TCHAR*` 식별자(헤더·소스에서 추출한 전수)·이름 상수·`L""` 리터럴인 `==`/`!=` — 수정 후 `ObjMgr.cpp:73,77,81`(`wstring`)만 남는다.
- 재현: 1단계 실패(메뉴에서 30초 타임아웃) → 2단계 후 같은 설정에서 `RESULT PASS (fail=0)`.
- 그 밖은 dead-code-removal·ub-fixes 와 같은 클린 4개 구성 절차. 경고 Win32 196, x64 268·206.

## 리스크

- 스캔이 놓친 비교 — 스모크 경로는 `/GF-` Release 가 잡는다. 스모크 밖은 코드 검토.
- `Obj.h` 의 `IsSameKey` 사용이 `Function.h` 포함 순서에 의존 — 빌드로 확인.

## 구현 중 변경

- **범위 확장(사용자 결정)**: 첫 전체 검증에서 x64 Release 스모크가 간헐적으로(4회 중 3회) 스테이지 진입 직후 접근 위반으로 끝났다.
  디버거(cdb)는 디버그 힙 때문에 재현하지 못해 `_NO_DEBUG_HEAP=1` 로 잡은 스택: `CUIObj::VelueToPercentage` ← `CUIObj::Render`(`UIObj.cpp:325`, HP·MP 바).
  원인은 `CUIObj` 의 `m_PlayerObj`·`m_PlayerData`·`m_MonsterData` 를 어느 생성자도 초기화하지 않은 기존 버그 — 플레이어가 선택되기 전에는 쓰레기 포인터를 역참조했다.
  Release `/GF-` 로 메모리 배치가 바뀌며 드러났다. 세 포인터를 `nullptr` 로 시작하고, 선택된 플레이어가 없으면 첫 플레이어를 표시하며, HP 바 조회에 몬스터 리스트 `nullptr` 검사를 넣었다.
  수정 후 x64 Release 스모크 10/10 PASS.
- 전체 검증의 Client 스모크를 구성마다 3회로 늘렸다(간헐 문제 확인용).

## 검증 결과 (2026-11-12)

`Exe`·`Temp` 삭제 → restore → 4개 구성 빌드(`-m:1 -nodeReuse:false`) → Client 스모크 구성마다 3회·툴 스모크 → PowerShell 로 8초 일반 실행.

| spec | 결과 |
|---|---|
| R1 `IsSameKey` | ✅ |
| R2 118곳 치환 | ✅ 재스캔 결과 `ObjMgr.cpp:73,77,81`(`wstring`, 원래 내용 비교)만 남음 |
| R3 텍스처 `map` 키 | ✅ Client `TextureMgr`·`MultiTexture`, MFC_000 `TextureMgr` 모두 `wstring` |
| R4 Release `/GF-` | ✅ Client Release Win32·x64 tlog 에 `/GF-` 44개 파일 전부, Debug 는 0 |
| R5 재현 테스트 | ✅ 수정 전(`/GF-` 만 적용) Win32 Release 스모크 메뉴에서 30초 타임아웃 FAIL → 수정 후 PASS |
| R6 경고 | ✅ Win32 196, x64 Debug 268·Release 206 — 변화 없음 |
| R7 스모크·일반 실행 | ✅ Client 4개 구성 × 3회 모두 `RESULT PASS (fail=0)`, 툴 4개 구성 `tiles=800`, 일반 실행 Client·툴 살아 있음·`smoke.log` 없음·남은 프로세스 0 |
| R8 CLAUDE.md | ✅ 「코드 관례」 문자열 비교 규칙 |

- 스모크 밖의 UI 클릭(스킬트리 버튼 등) 비교는 코드 검토로만 확인했다.
