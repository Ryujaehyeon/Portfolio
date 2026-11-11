# Intent: 쓰지 않는 코드 제거

Author: Ryu Jahyeon
Status: draft

## Problem
실행되지 않는 코드가 빌드·문서·이후 작업 범위에 계속 끼어 있다. 근거: [legacy-audit/review.md](../legacy-audit/review.md) 5·6번,
2026-11-11 `master`(`4568842`)에서 다시 확인. 경로는 `2D-Portfolio/` 기준.

- **참조가 없는 클래스 5개** — 자기 `.h/.cpp` 밖에서 이름이 쓰이지 않는다(`git grep -w`). `Headers/Include.h` 가 include 만 한다.
  `CCollisionMgr`(114줄, 같은 일은 `CObjMgr::ColCircle`), `CFullModeMgr`(128줄, 전체화면은 `CDevice::InitDevice`),
  `CNonPlayer`(29줄, 빈 클래스), `CSkillTree`(52줄, 빈 구현 — 실제 UI 는 `CUIObj` + `struct SkillTree`),
  `CMouse`(34줄, **vcxproj 에 없어 컴파일도 안 됨**).
- **도달할 수 없는 장면 2개** — `CStage2`(88줄)·`CStage3`(77줄). `SCENEID_STAGE2`·`SCENEID_STAGE3` 를 반환하는 코드가 없고
  `CSceneMgr::InitScene`(`SceneMgr.cpp:34-39`)의 case 만 있다. 둘 다 `Initialize()` 가 `return S_OK;` 뿐이라
  도달하면 초기화 안 된 `m_pPrototype` 으로 `AddObject` 를 불러 크래시한다.
- **툴의 붙지 않는 페이지** — `MFC_000/Page4.*`(55줄). `CMySheet` 는 Page3·1·2 만 붙인다. 대화상자 리소스 `IDD_PAGE4` 도 남아 있다.
- **주석 처리된 코드 약 170줄**(코드처럼 보이는 `//` 줄, 휴리스틱 집계). 많은 곳: `ObjMgr.cpp` 51, `Stage2.cpp` 15, `Monster.cpp` 11,
  `MainGame.cpp` 10, `StageBackground.cpp` 9, `Player.cpp` 9. `KeyMgr.h:3-4` 의 `SingletonTemplate` 흔적 포함.
- 이 코드가 만드는 부담:
  - 경고 3건이 여기서 나온다(Win32 Debug 198 중 `FullModeMgr.cpp` C4838 2건, `Stage2.cpp` C4715 1건 — 반환 없는 경로).
  - CLAUDE.md·`docs/ARCHITECTURE.md` 가 없는 것과 다름없는 클래스를 구조로 설명한다(「Stage1~3 장면」, `CCollisionMgr` 싱글톤 등).
  - 뒤따르는 `ub-fixes`·`objmgr-lifetime`·`warning-cleanup` 이 실행되지 않는 코드까지 읽고 고쳐야 한다.

## Proposed outcome
- 위 클래스·파일이 저장소와 프로젝트 파일(`Client.vcxproj`·`MFC_000.vcxproj`·`.filters`·`Include.h`)에서 사라진다.
- 주석 처리된 코드가 사라지고, 설명 주석만 남는다. 의도가 담긴 것(보스 소환 등)은 문서 한 곳에 메모로 남는다.
- **게임·툴 동작은 그대로다** — 4개 구성 스모크 결과가 변경 전과 같다(Debug PASS, Release 는 알려진 실패 1건 그대로).
- 경고가 늘지 않고, 삭제한 파일에서 나던 만큼 줄어든다(Win32 198 → 195 예상).
- CLAUDE.md 「아키텍처 패턴」·`docs/ARCHITECTURE.md` 가 남은 코드와 일치한다.

## Affected users and systems
사용자: 이 repo 개발자(읽고 고칠 코드가 줄어든다). 게임·툴 사용자에게 보이는 변화는 없다.
시스템: `Client/`(삭제 14파일 + `SceneMgr.cpp`·주석 정리 대상 파일), `MFC_000/`(`Page4.*`, 리소스), `Headers/Include.h`·`Constant.h`,
두 vcxproj·filters, CLAUDE.md, `docs/ARCHITECTURE.md`

## Constraints
반드시 지킬 것:
- 동작 불변 — 실행되는 코드의 로직을 바꾸지 않는다. 판정은 4개 구성 빌드(경고 수 비교) + `-smoke`.
- `.dat` 호환 유지 — `Headers/Struct.h` 구조체 배치를 건드리지 않는다(주석만 지운다).
- `2D-Portfolio/Resource/` 는 손대지 않는다(Stage2 전용 `Cave.mp3` 같은 리소스가 고아가 되어도 그대로 둔다).
- 지운 것은 git 이력으로 되살릴 수 있어야 한다 — 삭제 커밋을 종류별로 나눠 찾기 쉽게 한다.

범위 밖:
- 버그·미정의 동작 수정(→ `ub-fixes`, `objmgr-lifetime`), 남는 경고 정리(→ `warning-cleanup`).
- 쓰이지 않는 함수·멤버·상수 단위의 정리(오딧이 조사한 것은 클래스·파일 단위). 눈에 띄면 기록만 한다.
- `Resource/` 실사용 조사, Client·툴 중복 코드(→ `shared-render-lib`).

## Open questions
없음 — 아래는 사용자 결정(2026-11-11, 권장안 채택).

1. **`CStage2`·`CStage3` 는 지운다.** 의도(스테이지별 몬스터·보스 구성)는 메모로 남긴다.
2. **의도 메모는 `docs/ARCHITECTURE.md` 의 「구현되지 않은 기능」 절**에 둔다 — 구조를 읽는 사람이 찾는 곳이다.
3. **`SCENEID_STAGE2`·`SCENEID_STAGE3` enum 값은 지운다** — `SCENEID_END = -2`·`SCENEID_NONPASS = -1` 이 명시 값이라 지워도
   남는 값(`LOGO` 0·`MENU` 1·`STAGE` 2)이 바뀌지 않는다. 몬스터 이름 상수(`VIPER`·`SANDRAIDER` 등)는 상수 단위 정리라 범위 밖(기록만).
4. **`IDD_PAGE4` 리소스도 같이 지운다** — `.rc`·`resource.h` 는 UTF-16 LE 를 유지하고, 변경이 해당 블록·줄뿐인지 diff 로 확인한다.
5. **주석 처리된 코드는 예외 없이 지운다.** 남길 가치가 있는 의도는 2번 메모로 옮긴다. 설명 주석(코드가 아닌 문장)은 대상이 아니다.
