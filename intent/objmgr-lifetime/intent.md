# Intent: 죽은 객체를 순회 중에 지우지 않기

Author: Ryu Jahyeon
Status: draft

## Problem
게임 객체를 **순회하는 도중에 지우고, 리스트에는 nullptr 을 남긴다.** 지금 순서에서는 우연히 터지지 않지만,
다른 객체의 상태를 바꾸는 코드가 하나만 생겨도 해제된 메모리를 역참조한다. 근거: [legacy-audit/review.md](../legacy-audit/review.md) 1번(P1, 오딧이 「가장 위험한 버그」로 분류),
2026-11-12 `master`(`9ec72c7`)에서 다시 확인. 경로는 `2D-Portfolio/` 기준.

- `CObjMgr::IterRelease`(`Client/ObjMgr.cpp:520-570`)는 전체 맵을 돌며 죽은 몬스터(`DEATH` 모션·프레임 한 바퀴 끝)와 죽은 스킬(`DEATH`)을
  `SAFE_DELETE` 하지만 **리스트에서 빼지 않고 nullptr 을 남긴다.**
- 이 함수가 `CObjMgr::Progress` 안에서 **객체 하나를 처리할 때마다** 불리고(`:244`), `Progress` 는 **지금 처리 중인 객체**가 nullptr 일 때만 `erase` 한다(`:246-250`).
  리스트의 다른 위치에서 지워진 객체는 nullptr 인 채 남아, 다음 차례에 `(*iter2)->Progress()`, 다음 `IterRelease` 의 `(*iter2)->GetObjType()` 이 nullptr 을 역참조한다.
- 지금 크래시가 나지 않는 이유는 삭제 조건이 대개 **그 객체 자신의 `Progress()` 안에서** 만들어져 자기 차례에 지워지기 때문이다. 구조가 보장하는 것이 아니다.
- **대상 포인터**: 플레이어의 `m_pTagetObj` 는 공격한 몬스터를 가리키고(`Player.cpp:377`), 그 몬스터가 지워져도 그대로 남는다. 지금은 플레이어가 이 포인터를 역참조하지 않아 문제가 드러나지 않을 뿐이다.
- **정렬 레이어**: `CSortMgr` 는 프레임 동안 객체 포인터를 모아 `Render` 에서 그린다. 객체가 등록된 뒤 같은 프레임에 지워지면 해제된 객체를 그리게 된다(지금은 위와 같은 이유로 일어나지 않음).
- 원작자도 null 원소를 만났던 흔적: `ColCircle` 의 nullptr 검사, `IterRelease` 의 주석 「0817 오전 몬스터가 제대로 삭제되지 않음」.
- [ub-fixes](../ub-fixes/intent.md) 로 Release 에서도 시체 삭제가 처음 동작하게 돼, 이 경로가 모든 구성에서 실제로 쓰인다.
- **검증 수단이 없다**: 스모크는 스테이지에서 4초를 돌지만 몬스터가 죽어 지워지는 경로를 지나간다는 보장이 없다.

## Proposed outcome
- 객체 순회(`Progress`·`ColCircle`·`ObjInteraction` 등) 도중에는 객체가 지워지지 않는다. 리스트에 nullptr 이 남지 않는다.
- 죽은 객체는 **프레임 시작에 한 곳에서** 한 번에 `delete` 되고 리스트에서 빠진다. 그 객체를 가리키던 다른 객체의 대상 포인터는 그때 끊긴다.
- 스모크가 「몬스터 하나가 죽어 정리되는」 경로를 실제로 지나가고 결과를 남긴다(4개 구성 PASS).
- 게임 동작은 그대로다. 차이는 죽은 객체가 지워지는 시점이 같은 프레임의 끝에서 다음 프레임 시작으로 한 프레임 늦어지는 것뿐이다.

## Affected users and systems
사용자: 게임을 실행하는 사람(잠재 크래시 제거, 보이는 변화 없음), 이 repo 개발자(객체 수명 규칙이 한 곳에 모임)
시스템: `Client/ObjMgr.cpp`·`ObjMgr.h`, `Client/Monster.cpp`(스모크 분기), `Client/Client.cpp`(스모크 필수 항목), CLAUDE.md·`docs/ARCHITECTURE.md`

## Constraints
반드시 지킬 것:
- 4개 구성 빌드·스모크로 검증한다. 경고가 늘지 않는다. 고치는 코드에는 CLAUDE.md 「C++ 코딩 표준」 품질 규칙을 적용한다.
- 스모크용 코드는 `IsSmokeMode()` 안에만 둔다 — `-smoke` 없이 실행하면 동작이 같아야 한다(smoke-log 의 원칙).
- 삭제 조건(몬스터: `DEATH` + 프레임 한 바퀴 끝, 스킬: `DEATH`)은 바꾸지 않는다.

범위 밖:
- 객체 소유권을 스마트 포인터로 바꾸는 일, `CObjMgr` 구조 재설계.
- 이름 문자열 포인터 비교(→ `string-key-compare`), 경고 정리(→ `warning-cleanup`).
- 장면 해제 때의 싱글턴 파괴 순서(`CStage::Release` 가 `CObjMgr` 를 통째로 파괴) — 지금 문제가 확인되지 않았다.

## Open questions
없음 — 사용자 결정(2026-11-12): 프레임 시작에 한 번 정리(죽음 마지막 프레임이 한 프레임 더 그려질 수 있음 수용), 대상 포인터 끊기, 스모크 모드에서만 몬스터 하나를 죽여 정리 경로 검증.
