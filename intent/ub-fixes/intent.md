# Intent: 미정의 동작·잘못된 반복 수정

Author: Ryu Jahyeon
Status: draft

## Problem
게임이 **빌드 구성이나 우연에 따라 다르게 동작한다.** 근거: [legacy-audit/review.md](../legacy-audit/review.md) 3번,
[smoke-log 「알려진 실패」](../smoke-log/plan.md), 2026-11-11 `master`(`9d1b870`)에서 다시 확인. 경로는 `2D-Portfolio/` 기준.

- **Release 에서 프레임 기반 동작이 거의 일어나지 않는다** — `CObj::FrameMove`(`Client/Obj.cpp:59-75`)가 애니메이션이 한 바퀴 돌면
  Debug 는 `fStart = 0`, Release 는 `fStart -= fMax` 로 처리한다(`#if _DEBUG` 분기). 그런데 「한 바퀴의 시작」을 `fStart <= 0` 으로 판정하는 곳이 7곳이다:
  스킬 생성 3곳(`Player.cpp:819,837,849`), 몬스터 공격 2곳(`Monster.cpp:394,432`), 경험치 지급(`Monster.cpp:808`), 죽은 몬스터 삭제(`ObjMgr.cpp:528`).
  Release 에서는 `fStart` 가 0 이하가 되는 일이 거의 없어 **스킬이 안 나가고, 몬스터가 공격하지 않고, 경험치를 못 받고, 시체가 지워지지 않는다.**
  4개 구성 스모크에서 Release 만 `missing skill BoneSpear` 로 실패하는 원인이다.
- **반환하지 않는 경로** — `CUIObj::VelueToPercentage`(`UIObj.cpp:502-531`)는 `HPBar` 에서 마우스 아래 몬스터가 없으면 값을 반환하지 않는다(C4715, 남은 1건).
  호출부는 `>= 0.f` 로 「표시할지」를 판단해(`UIObj.cpp:361,377`) 쓰레기 값으로 몬스터 HP 바를 그리거나 숨긴다.
- **미초기화 반환값** — `CObjMgr::Progress`(`ObjMgr.cpp:187`)의 `SCENEID iScene;` 는 객체가 하나도 없으면 초기화되지 않은 채 반환된다.
- **원소 건너뛰기** — 같은 함수(`ObjMgr.cpp:248-254`)가 지운 객체를 `erase` 한 뒤 `continue` 해 for 의 `++iter2` 가 **다음 객체를 건너뛴다.** 그 객체는 그 프레임에 갱신되지 않는다.
- **충돌 판정 누락** — `CObjMgr::ColCircle`(`ObjMgr.cpp:410-461`)이 안쪽(몬스터) 루프에서 바깥(스킬) 반복자를 `++iter` 하고, 몬스터 반복자를 처음으로 되돌리지 않는다.
  스킬마다 **몬스터 일부만** 검사하고 스킬도 건너뛴다. 또 이 함수는 스킬 객체 **하나마다** 한 번씩(`ObjMgr.cpp:241-243`) 전체 스킬×몬스터에 대해 불린다.
- **잘못된 다운캐스트** — UI 객체(`CUIObj`)를 `static_cast<CPlayer*>`·`static_cast<CMonster*>` 로 바꿔 `Setlist` 를 부른다(`ObjMgr.cpp:262,269`).
  상속 관계가 아니라 미정의 동작이다. `Setlist` 가 `CObj` 의 가상 함수라 우연히 `CUIObj::Setlist` 가 불린다.
- **장면이 자기 자신을 지운다** — `CLogo`·`CMenuD2`·`CStage` 의 `Progress()` 가 그 안에서 `CSceneMgr::InitScene` 을 불러
  **실행 중인 자기 객체를 delete** 한다. 지금은 그 뒤 멤버 접근이 없어 안전하지만 한 줄만 추가해도 use-after-free 다.

## Proposed outcome
- Debug·Release 가 같은 규칙으로 프레임을 돌려, **4개 구성 스모크가 모두 `RESULT PASS (fail=0)`** 이 된다(CLAUDE.md 「알려진 실패」 삭제).
- C4715 가 0 이 된다(경고 Win32 197 → 196).
- 위 목록의 미정의 동작·반복 오류가 사라진다. 장면 교체는 장면 밖(`CSceneMgr`)에서 일어난다.
- Debug 에서 보던 게임 동작(장면 전환·스킬·몬스터·UI)은 그대로다. 판정 누락 수정으로 달라지는 것은 아래 「Open questions」 1번 결정을 따른다.

## Affected users and systems
사용자: 게임을 Release 로 실행하는 사람(스킬·전투가 동작하게 된다), 이 repo 개발자(Debug·Release 차이 제거)
시스템: `Client/Obj.cpp`(`FrameMove`), `Client/UIObj.cpp`, `Client/ObjMgr.cpp`(`Progress`·`ColCircle`), `Client/SceneMgr.cpp`, `Client/Logo.cpp`·`MenuD2.cpp`·`Stage.cpp`(`Progress`), CLAUDE.md

## Constraints
반드시 지킬 것:
- 4개 구성 빌드·스모크로 검증한다. Debug 스모크 결과는 그대로여야 한다.
- 경고가 늘지 않는다. 고치는 코드에는 CLAUDE.md 「C++ 코딩 표준」의 품질 규칙(`nullptr`, 한 줄 블록도 중괄호 등)을 적용한다.
- `.dat` 포맷(`Headers/Struct.h`)은 건드리지 않는다.

범위 밖:
- `IterRelease` 가 nullptr 을 리스트에 남기는 문제와 객체 소유·수명 정리(→ `objmgr-lifetime`, 오딧 1번). 이번에는 지금의 nullptr 처리 흐름을 유지한다.
- 이름 문자열을 포인터로 비교하는 문제(`L"HP" == VelueName` 등 → `string-key-compare`, 오딧 2번).
- 남은 경고(C4244·C4838·C4018·C4267 → `warning-cleanup`).
- 게임 밸런스 조정(데미지 공식·수치).

## Open questions
없음 — 아래는 사용자 결정(2026-11-11).

1. **`ColCircle` 은 올바르게 고친다** — 한 프레임에 한 번, 모든 스킬 × 모든 몬스터를 검사한다. 여러 몬스터가 한꺼번에 맞고,
   스킬 수만큼 겹치던 중복 데미지는 사라진다(체감 난이도 변화 수용). 스모크는 데미지를 보지 않으므로 이 부분은 눈 확인 대상이다.
2. **`FrameMove` 는 Debug 방식(넘치면 0)으로 통일한다** — `fStart <= 0` 판정 7곳이 지금 Debug 에서처럼 동작한다.
3. **장면 자기 삭제 구조도 고친다** — `InitScene` 호출을 각 장면의 `Progress()` 에서 `CSceneMgr::Progress` 로 옮긴다.
4. **`VelueToPercentage` 는 대상이 없으면 `-1.f`** — 호출부의 `>= 0.f` 판정에 따라 몬스터 HP 바를 숨긴다.
