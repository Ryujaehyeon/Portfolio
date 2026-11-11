# Spec: dead-code-removal

Author: Ryu Jahyeon
Status: draft

<!-- 입력: 승인된 intent.md (PR #20). 경로는 2D-Portfolio/ 기준. -->

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | 미사용 클래스 `CCollisionMgr`·`CNonPlayer`·`CSkillTree`·`CMouse` 의 `.h/.cpp` 8개가 없다 | 파일 없음 + R4 |
| R2 | 도달 불가 장면 `CStage2`·`CStage3` 의 `.h/.cpp` 4개, `SCENEID_STAGE2`·`SCENEID_STAGE3`, `CSceneMgr::InitScene` 의 두 case 가 없다 | 파일 없음 + R4 |
| R3 | 툴의 `CPage4`(`Page4.h/.cpp`)와 대화상자 리소스 `IDD_PAGE4`(`.rc` 의 DIALOGEX 블록·DESIGNINFO 항목, `resource.h` 의 `#define`)가 없다. `.rc`·`resource.h` 는 UTF-16 LE(BOM) 그대로이고 변경은 그 블록·줄뿐이다 | `file` 인코딩 확인, UTF-8 로 변환한 diff |
| R4 | 지운 이름이 소스·프로젝트 파일에 남아 있지 않다: `CCollisionMgr` `CNonPlayer` `CSkillTree` `CMouse` `CStage2` `CStage3` `CPage4` `SCENEID_STAGE2` `SCENEID_STAGE3` `IDD_PAGE4`, 그리고 `Include.h`·두 vcxproj·두 `.filters` 의 해당 파일 항목(빈 필터 폴더 포함) | `git grep -n -w -E "<이름들>" -- 2D-Portfolio` 결과 0줄(`.rc`·`resource.h` 는 UTF-8 변환 후 검색) |
| R5 | `CFullModeMgr`(`FullModeMgr.h/.cpp`)는 **그대로 남는다** — 사용 중(intent 「정정」) | 파일·`Client.cpp:89-91` 호출 유지 |
| R6 | 주석 처리된 코드가 없다. 대상: ① 줄 전체가 주석이고 내용이 C++ 문장·선언·전처리 조각인 줄 ② 코드 줄 안의 `/* 코드 */` 조각 ③ 코드만 담긴 `/* … */` 블록. 설명 주석(문장)은 대상이 아니다 | plan 의 대상 목록 전부 삭제 + 집계 명령(intent 의 휴리스틱) 결과를 남은 줄마다 「설명 주석」으로 판정해 plan 에 기록 |
| R7 | 주석 정리 커밋은 **코드를 바꾸지 않는다** — 그 커밋이 건드린 모든 파일에서, 주석과 공백을 뺀 나머지 텍스트가 변경 전후로 같다 | 검사 스크립트(주석·공백 제거 후 비교) 결과 「차이 0」 |
| R8 | 4개 구성(Win32·x64 × Debug·Release)이 클린 빌드된다 | restore + MSBuild 4회 종료 코드 0·오류 0 (`-m:1 -nodeReuse:false`) |
| R9 | 경고가 늘지 않고 `Stage2.cpp` 의 C4715 1건만 줄어든다: Win32 Debug·Release 197, x64 Debug 275·Release 207. 새 종류의 경고 0 | 빌드 로그의 경고 번호별 집계를 변경 전(198·198·276·208)과 비교 |
| R10 | 동작 불변 — Client `-smoke`: Debug 는 두 플랫폼 모두 종료 코드 0·`RESULT PASS (fail=0)`, Release 는 두 플랫폼 모두 FAIL 이 알려진 `missing skill BoneSpear` **1건뿐**. 툴 `-smoke ../Client/Map.dat`: 4개 구성 모두 `tiles=800`·종료 코드 0. `-smoke` 없이 8초 실행해 살아 있고 오류 메시지박스가 없다 | CLAUDE.md 「테스트」 명령, `smoke.log` |
| R11 | 지운 코드에 담긴 의도가 `docs/ARCHITECTURE.md` 「구현되지 않은 기능」 절에 남는다: 2·3 스테이지(배경음 `Cave`, 몬스터 Viper·SandRaider, 보스 Izual·Andariel·Diablo), `Player.cpp` 의 DirectInput 키보드 이동, 그 밖에 plan 에서 의도가 있다고 판정한 것 | 문서 검토 |
| R12 | CLAUDE.md(프로젝트 표의 「Stage1~3」, 「아키텍처 패턴」의 싱글톤·장면·오브젝트·스킬 목록)와 `docs/ARCHITECTURE.md`(장면 표, 클래스 다이어그램, 「죽은 코드」 행, 툴 `Page4` 언급)가 남은 코드와 일치한다. CLAUDE.md 의 경고 기준 수치가 R9 측정값으로 갱신된다 | 문서 검토, 문서에 지운 이름이 「구현되지 않은 기능」 절 밖에 없음 |
| R13 | 삭제가 종류별 커밋으로 나뉜다: 미사용 클래스 / 도달 불가 장면 / 툴 Page4 / 주석 처리된 코드 / 문서 | `git log --oneline` |

## 설계

- **삭제 범위는 「컴파일 단위와 그 등록」까지** — 파일을 지우고, 그 파일을 가리키는 곳(`Headers/Include.h` 의 include 5줄,
  `Client.vcxproj`·`MFC_000.vcxproj` 의 `ClInclude`/`ClCompile`, `.filters` 의 항목과 비게 되는 필터 폴더)을 함께 지운다.
  남는 코드의 로직은 건드리지 않는다. 지운 헤더가 정의하던 것은 그 클래스뿐이라(헤더 선언 전수 확인) 다른 코드가 전이 include 로 얻던 것이 없다.
- **`CStage2`·`CStage3`**: `SceneMgr.cpp` 의 `case SCENEID_STAGE2/3` 두 블록과 `Constant.h` 의 enum 값 2개를 지운다.
  `SCENEID_END = -2`·`SCENEID_NONPASS = -1` 이 명시 값이고 지우는 값이 enum 끝이라 `LOGO` 0·`MENU` 1·`STAGE` 2 는 그대로다
  (스모크의 `scene 0·1·2 init` 판정도 그대로). `SceneObj.cpp:6` 주석의 「`CStage·CStage3` 의 Release」는 `CStage` 로 고친다.
  몬스터 이름 상수(`VIPER`·`SANDRAIDER`·`IZUAL`·`ANDARIEL`·`DIABLO`)는 남긴다 — 일부는 `ObjMgr.cpp` 가 쓰고, 상수 단위 정리는 범위 밖(intent).
- **`CMouse`** 는 vcxproj 에 없어 파일 삭제뿐이다(빌드 영향 0).
- **`CFullModeMgr` 는 남긴다** — `WinMain` 이 `ModeSetter` 로 호출한다. `Client.cpp:235-237` 의 주석 처리된 `ModeSetter` 코드는 R6 대상이다.
- **`IDD_PAGE4` 리소스**: `MFC_000.rc`·`resource.h` 는 UTF-16 LE. 텍스트로 디코드 → 해당 블록·줄 삭제 → 같은 인코딩·BOM·CRLF 로 다시 쓴다.
  `resource.h` 의 `_APS_NEXT_*` 값은 건드리지 않는다(다음 리소스 번호일 뿐, 빈 번호가 생겨도 무해). 리소스 편집기를 쓰지 않는 이유: 재저장 시 무관한 줄이 바뀔 수 있다.
- **주석 처리된 코드**: 판정은 사람이 한다 — intent 의 휴리스틱 집계(약 170줄)는 후보 목록일 뿐이다. plan 에 파일별 대상 줄을 적고,
  남기는 줄(설명 주석)도 이유와 함께 적는다. 삭제 후 연속 빈 줄이 3줄 이상 생기는 곳만 1줄로 줄인다(그 밖의 포맷은 건드리지 않는다).
  `Headers/Struct.h` 는 주석만 지우므로 구조체 배치가 그대로다(R7 이 보증).
- **R7 검사 스크립트**: C++ 토큰화 없이, 문자열·문자 리터럴을 보존하며 `//`·`/* */` 주석과 모든 공백을 제거한 텍스트를 비교한다.
  인텐트 폴더에 두지 않고 일회용으로 쓴다(YAGNI — 저장소에 남길 도구가 아니다). 사용한 명령과 결과만 plan 에 기록한다.
- **커밋 분리(R13)**: 종류별로 되살리기 쉽게(intent 제약). 각 커밋은 빌드가 되는 상태로 둔다 — 파일 삭제와 그 등록 삭제를 같은 커밋에 넣는다.
- **검증은 한 번에**: 코드 변경이 모두 삭제라 커밋마다 4개 구성을 돌리지 않는다. 커밋마다 Win32 Debug 빌드만 확인하고, 마지막에 R8~R10 전체를 돌린다.

원칙 근거: 실행되지 않는 코드는 「지금 필요하지 않은 것」(YAGNI)이고, 문서가 그것을 구조로 설명하는 것은 코드가 의도를 표현하지 못하게 한다(PIE).
새 추상화·구조 변경은 없다(Surgical Changes).

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| 오딧·intent 초안이 `CFullModeMgr` 를 미사용으로 **잘못 분류** | 동작 불변 | **대상에서 제외**(R5), intent 정정. 나머지 대상은 헤더가 선언하는 모든 이름으로 재검색해 외부 사용 0 확인. `legacy-audit/review.md` 5번에는 정정 주석을 단다 |
| 주석 처리된 코드 판정이 휴리스틱·사람 눈에 의존 | 정확성 | 실수로 코드를 지우는 경우는 R7(주석·공백 제거 후 동일)이 기계적으로 막는다. 설명 주석을 잘못 지우는 경우는 PR diff 검토 |
| 줄 번호가 바뀌어 `__LINE__` 을 쓰는 디버그 출력(`DEBUG_LINE` 등)의 숫자가 달라짐 | 동작 불변 | 수용 — 표시되는 줄 번호만 다르고 로직은 같다. 바이너리 해시 비교(`/Brepro`)는 이 때문에 쓰지 않는다 |
| `.rc` 를 스크립트로 편집 | 리소스 편집기 생성 파일 | R3 판정(인코딩 유지 + 변환 diff 가 해당 블록뿐) + 툴 빌드·스모크·실행. 프로퍼티 시트의 Page1~3 모양은 자동 검증하지 못한다 → 아래 항목 |
| 스모크가 보지 못하는 것: 화면·소리·조작, 툴의 프로퍼티 시트 UI | 검증 범위 | 수용 — 삭제 대상이 실행 경로 밖임을 R4·R7·R8 로 보증하고, 툴은 `-smoke` 없이 실행해 창이 뜨는지만 본다. 눈 확인은 PR 에 「미실시」로 적는다 |
| Release 스모크는 스킬 생성까지 가지 못함(알려진 실패) | 검증 범위 | 수용 — 같은 경로를 Debug 스모크가 확인한다. FAIL 이 그 1건뿐인지를 판정 기준으로 둔다(R10) |
| 지운 주석에 담긴 미완성 기능의 의도 손실 | 포트폴리오 이력 | R11 메모 + git 이력(R13 의 종류별 커밋) |
| `Resource/` 의 `Cave.mp3` 등 2·3 스테이지 리소스가 고아가 됨 | 저장소 크기 | 수용 — `Resource/` 는 건드리지 않는다(CLAUDE.md). 오딧 「미조사 범위」의 리소스 실사용 조사 몫 |
