# Spec: objmgr-lifetime

Author: Ryu Jahyeon
Status: draft

<!-- 입력: 승인된 intent.md (PR #27). 경로는 2D-Portfolio/ 기준. -->

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | 죽은 객체 정리(`CObjMgr::IterRelease`)는 `CObjMgr::Progress` **시작에 한 번만** 불린다. 순회 도중(객체 처리마다)에는 불리지 않는다 | `git grep -n IterRelease` — 정의·선언·`Progress` 첫머리 호출 1곳 |
| R2 | 정리는 죽은 객체를 `delete` 하고 **리스트에서 뺀다**. 정리가 끝난 리스트에 nullptr 이 없다. 삭제 조건은 그대로다(몬스터: `DEATH` 모션 + `fStart <= 0`, 스킬: `DEATH` 모션) | 코드 검토 + R5 |
| R3 | 정리할 때, 살아 있는 객체 중 죽은 객체를 대상으로 가리키는 것(`GetTagetObj()`)은 대상이 `nullptr` 로 끊긴다 | 코드 검토 |
| R4 | `CObjMgr::Progress` 에서 「nullptr 이면 `erase`」 경로와 그 플래그(`bErased`)가 사라진다 — 순회 중 리스트가 줄지 않는다 | 코드 검토 |
| R5 | 스모크 모드에서만: 스테이지 진입 약 1.5초 뒤 몬스터 하나의 체력을 0 으로 만든다(`INFO smoke kill <이름>`). 그 몬스터가 정리되면 `PASS removed monster <이름>` 을 남긴다. 필수 항목에 `removed monster` 를 더한다 | `smoke.log` |
| R6 | 4개 구성 Client 스모크 모두 종료 코드 0·`RESULT PASS (fail=0)`(R5 포함). 툴 스모크 4개 구성 `tiles=800` | CLAUDE.md 「테스트」 |
| R7 | `-smoke` 없이 실행하면 R5 의 코드가 아무 일도 하지 않는다. Client·툴이 8초 뒤 살아 있다 | 코드 검토(`IsSmokeMode()` 안) + PowerShell 실행 |
| R8 | 4개 구성 클린 빌드 오류 0, 경고가 늘지 않는다(Win32 196, x64 Debug 268·Release 206) | 빌드 로그 |
| R9 | CLAUDE.md 「테스트」(스모크가 확인하는 것), `docs/ARCHITECTURE.md` 「7. 객체 생성과 소멸」이 새 수명 규칙과 맞는다 | 문서 검토 |

## 설계

- **정리 시점: 프레임 시작** — `CObjMgr::Progress` 첫머리에서 `IterRelease()` 를 한 번 부른다.
  - 프레임 끝이 아니라 시작인 이유: `Progress` 가 객체를 `CSortMgr` 에 등록하고 그 프레임의 `Render` 가 그린다. 프레임 끝에서 지우면 등록된 포인터가 해제된 채 그려진다.
    시작에서 지우면 그 프레임의 등록·그리기·충돌 검사가 모두 살아 있는 객체만 본다.
  - 그 대가로 죽은 객체는 「삭제 조건을 만족한 프레임」에 한 번 더 그려지고 다음 프레임 시작에 지워진다(사용자 결정으로 수용).
  - 삭제 조건 판정 시점이 같다: 몬스터의 `fStart <= 0` 은 자기 `Progress` 의 `FrameMove` 가 0 으로 되돌린 직후부터 다음 자기 `Progress` 전까지 유지되므로, 다음 프레임 시작에도 참이다.
    경험치 지급(`ExpAcquired`, 같은 조건)은 그 객체의 `Progress` 안에서 이미 한 번 일어났고, 다음 프레임에는 `Progress` 전에 지워지므로 두 번 주지 않는다.
- **`IterRelease` 의 새 동작**(이름은 기존 관례대로 유지):
  1. 모든 리스트를 돌며 죽은 객체(기존 조건)를 모은다.
  2. 살아 있는 객체의 대상(`GetTagetObj()`)이 모은 객체 중 하나면 `SetTagetObj(nullptr)`(둘 다 `CObj` 의 기존 가상 함수).
  3. 모은 객체를 `SAFE_DELETE` 하고 `list::remove_if`(조건: nullptr)로 리스트에서 뺀다.
  - 대상 끊기는 「객체 수 × 죽은 수」만큼 돈다. 몬스터 10여 마리·스킬 몇 개라 비용은 무시할 수 있다(KISS — 역참조 표 같은 구조를 만들지 않는다).
- **`Progress` 루프**: ub-fixes 가 넣은 `bErased`·`erase` 경로와 루프 안의 `IterRelease()` 호출을 지운다. 증가 위치(루프 끝)는 그대로 둔다.
- **스모크 분기**(smoke-log 의 기존 패턴 — `Player.cpp` 의 스킬 시전 분기와 같은 꼴):
  - `CMonster::Progress`: 스모크 모드이고 아직 아무도 죽이지 않았으면, 처음 불린 시각부터 1.5초 뒤 자기 체력을 0 으로 두고 `SmokeLog(L"smoke kill %s", 이름)`. 한 마리만.
    체력 0 이면 기존 `CheckKey` 가 `DEATH` 로 바꾸고 죽음 애니메이션(약 1초) 뒤 삭제 조건이 된다.
  - `CObjMgr::IterRelease`: 스모크 모드에서 그 몬스터를 지울 때 `SmokeCheck(true, L"removed monster %s", 이름)`. 어느 몬스터인지는 `CMonster` 의 스모크 분기가 기억한 포인터와 비교한다.
  - `Client.cpp`: 필수 항목에 `L"removed monster"`.
- **`ColCircle` 의 nullptr 검사**는 이제 필요 없지만 남긴다 — 비용이 없고, 지우는 것은 이 인텐트의 목적(수명 규칙)과 무관한 정리다.

원칙 근거: 객체 삭제를 「프레임 시작의 정리」 한 곳으로 모은다(SRP — 순회 코드는 수명을 신경 쓰지 않는다). 새 추상화(소유 포인터·이벤트)는 넣지 않는다(YAGNI).

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| 죽은 객체가 한 프레임 더 그려짐 | 동작 불변 | 사용자 결정으로 수용. 죽음 애니메이션 마지막 프레임이 한 번 더 보이는 정도 |
| 스모크가 몬스터를 직접 죽임 — 테스트 코드가 게임 코드에 들어감 | 스모크 원칙 | `IsSmokeMode()` 안에만, smoke-log 의 기존 패턴(`Player.cpp`)과 같은 방식. R7 로 일반 실행 불변 확인 |
| 스모크 시간 안에 정리가 안 끝날 수 있음(죽음 애니메이션 길이·몬스터 상태) | 검증 안정성 | 1.5초 + 애니메이션 약 1초 < 스테이지 4초. 실패하면 `missing removed monster` 로 드러나며, 그때 시각을 조정 |
| `CStage::Release` 의 `DESTROY_SINGLE(CObjMgr)` 등 장면 해제 경로 | 범위 | 범위 밖(intent). 스모크 종료 경로가 이미 지나가며 종료 코드로 크래시를 잡는다 |
