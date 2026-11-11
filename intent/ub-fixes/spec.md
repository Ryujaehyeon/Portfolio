# Spec: ub-fixes

Author: Ryu Jahyeon
Status: draft

<!-- 입력: 승인된 intent.md (PR #24). 경로는 2D-Portfolio/ 기준. -->

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | `CObj::FrameMove` 가 구성과 무관하게 같은 규칙으로 동작한다: `fStart > fMax` 이면 `fStart = 0.f`. `#if _DEBUG` 분기가 없다 | 코드 검토 |
| R2 | 4개 구성 Client 스모크가 모두 종료 코드 0·`RESULT PASS (fail=0)` — Release 의 `missing skill BoneSpear` 가 사라진다 | CLAUDE.md 「테스트」 명령 |
| R3 | `CUIObj::VelueToPercentage` 가 모든 경로에서 값을 반환한다. 대상(마우스 아래 몬스터)이 없으면 `-1.f` | C4715 0건 |
| R4 | `CObjMgr::Progress` 의 반환값이 항상 초기화돼 있다 — 갱신할 객체가 없으면 `SCENEID_NONPASS` | 코드 검토 |
| R5 | `CObjMgr::Progress` 가 객체를 `erase` 한 뒤 다음 객체를 건너뛰지 않는다 — 리스트의 모든 객체가 한 프레임에 한 번 `Progress()` 된다 | 코드 검토 |
| R6 | `CObjMgr::ColCircle` 이 한 프레임에 **한 번** 불리고, 그 안에서 nullptr 이 아닌 모든 스킬 × 모든 몬스터 쌍을 검사한다. BoneSpear 는 첫 명중에서 소멸 표시(`fHealthPoint = -1`)하고 그 스킬의 검사를 끝낸다(단일 대상). 그 밖의 스킬은 범위 안의 모든 몬스터에 데미지를 준다. 화면 밖 BoneSpear 소멸 판정은 스킬마다 한 번 한다 | 코드 검토 + Debug·Release 스모크 PASS(스킬 생성·몬스터 존재) |
| R7 | `CObjMgr::Progress` 에 `static_cast<CPlayer*>`·`static_cast<CMonster*>` 가 없다 — UI 객체는 `CObj::Setlist`(가상)로 부른다 | `git grep` |
| R8 | 장면 교체(`CSceneMgr::InitScene`)는 `CSceneMgr::Progress` 에서만 일어난다. `CLogo`·`CMenuD2`·`CStage` 의 `Progress()` 는 다음 장면 ID 를 반환만 한다 | `git grep -n InitScene` 결과가 `SceneMgr.*`·`MainGame.cpp:21` 뿐 + 스모크의 `scene 0/1/2 init` PASS |
| R9 | 4개 구성 클린 빌드 오류 0. 경고는 C4715 1건만 줄어든다: Win32 196·196, x64 Debug 268·Release 206. 새 종류 0 | 빌드 로그 집계 |
| R10 | 툴 스모크 4개 구성 `tiles=800`·종료 코드 0, `-smoke` 없이 8초 실행 시 Client·툴 살아 있음 | CLAUDE.md 「테스트」 |
| R11 | CLAUDE.md 「테스트」의 「알려진 실패」 줄이 없어지고, smoke-log·FIX-GUIDE 의 관련 기록이 갱신된다 | 문서 검토 |

## 설계

- **`FrameMove`** — 분기를 없애고 Debug 쪽 동작만 남긴다. `fStart <= 0` 을 「애니메이션 한 바퀴의 시작」으로 쓰는 7곳(스킬 생성 3·몬스터 공격 2·경험치·시체 삭제)이
  리셋 직후 한 프레임 동안 참이 되는 지금 Debug 의 계약을 Release 에도 적용한다. 판정 쪽(7곳)은 고치지 않는다 — 계약을 한 곳(`FrameMove`)에서 지키는 편이 단순하다(KISS, DRY).
  Release 의 「넘친 만큼 빼기」가 주던 애니메이션 시간 보정은 사라진다(한 바퀴마다 최대 1프레임 분의 오차, 눈에 띄지 않는 수준).
- **`VelueToPercentage`** — 함수 끝에 `return -1.f;`. 호출부(`UIObj.cpp:361,377`)가 이미 `>= 0.f` 로 표시 여부를 판단한다. 비교 방식(포인터 비교)은 그대로 둔다(→ `string-key-compare`).
- **`CObjMgr::Progress`**
  - `SCENEID iScene = SCENEID_NONPASS;`
  - 안쪽 for 의 증가를 루프 끝으로 옮긴다. `erase` 한 경우는 `erase` 가 돌려준 다음 원소에서 그대로 계속하고(증가 없음), 원래처럼 그 객체의 장면 판정·정렬 등록은 건너뛴다.
    `erase` 직후의 `if (iter2 == end) break;` 두 줄은 루프 조건이 같은 일을 하므로 지운다.
  - `ColCircle` 호출을 스킬 객체마다가 아니라 **바깥 루프가 끝난 뒤 한 번**으로 옮긴다(몬스터·스킬 리스트가 모두 있을 때). 장면이 바뀌어 일찍 반환하는 프레임에는 부르지 않는다.
  - UI 분기의 `static_cast` 두 개를 `(*iter2)->Setlist(...)` 로 바꾼다 — `Setlist` 는 `CObj` 의 가상 함수라 같은 `CUIObj::Setlist` 가 불린다(동작 동일, 미정의 동작 제거).
- **`ColCircle`** — 이중 루프를 표준 형태로 다시 쓴다: 바깥 스킬, 안쪽 몬스터, 각각 자기 반복자만 증가. nullptr 은 건너뛴다(`continue`, 지금은 `break` 로 나머지를 버림).
  데미지 공식·`CrashAndSlide` 호출은 그대로다. 화면 밖 판정은 안쪽 루프 밖으로 뺀다.
- **장면 전환** — 세 장면의 `Progress()` 에서 `if (iScene > SCENEID_NONPASS) InitScene(iScene);` 를 지우고, `CSceneMgr::Progress` 가 `m_pSceneObj->Progress()` 의 반환값을 보고
  `InitScene` 을 부른다. 호출 순서는 그대로다(장면의 나머지 처리 → 교체). 이제 장면 객체는 자기 함수가 끝난 뒤에 지워진다.
- **품질 규칙**(CLAUDE.md 「C++ 코딩 표준」)은 고치는 줄에 적용한다: `nullptr`, 한 줄 블록도 중괄호, `const`. 고치지 않는 주변 줄의 기존 경고(예: `VelueToPercentage` 안 `RECT` 의 C4838)는 `warning-cleanup` 몫으로 둔다.

원칙 근거: 「한 바퀴 시작」 계약을 `FrameMove` 한 곳에서 지킨다(DRY). 장면 교체 책임을 `CSceneMgr` 로 모은다(SRP — 장면은 다음 장면을 알리기만 한다).
새 추상화는 넣지 않는다(YAGNI).

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| `ColCircle` 수정으로 데미지·몬스터 반응이 달라짐(여러 몬스터 명중, 스킬 수만큼 겹치던 데미지 제거, `CrashAndSlide` 가 몬스터 방향을 정하는 횟수 변화) | 동작 불변 | **사용자 결정으로 수용**(intent 1번). 스모크는 데미지를 보지 않으므로 PR 에 「눈 확인 미실시」로 적는다 |
| Release 의 애니메이션 시간 보정 제거 | 동작 일관성 | 수용 — Debug 와 같아진다. 한 바퀴당 최대 1프레임 오차 |
| Release 에서 스킬·몬스터 공격·경험치·시체 삭제가 **처음으로** 동작함 → Release 에서만 드러나던 다른 문제(예: 시체 삭제 후 nullptr 처리, 오딧 1번)가 나타날 수 있음 | 안정성 | 4개 구성 스모크(스킬 시전·종료까지)로 확인. 새 크래시가 나오면 멈추고 보고, 원인이 수명 문제면 `objmgr-lifetime` 으로 넘긴다 |
| `erase` 건너뛰기 수정으로 nullptr 원소가 그 프레임에 방문될 가능성 | 안정성 | 분석: `IterRelease` 는 지금 막 갱신한 객체만 삭제 조건을 만족시킨다(삭제 조건이 그 객체의 `Progress` 안에서 만들어짐) → 다음 원소는 nullptr 이 아니다. 그래도 nullptr 은 오딧 1번 몫이라 방어 코드를 더하지 않고 스모크로 확인 |
| 장면 전환을 옮긴 뒤 교체 시점 차이 | 동작 불변 | 같은 프레임, 같은 순서(장면 처리 끝 → 교체). 스모크 `scene 0/1/2 init` 로 확인 |
