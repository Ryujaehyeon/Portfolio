# Intent: 스모크 간헐 실패 제거

Author: Ryu Jahyeon
Status: draft

## Problem
x64 Release 스모크가 간헐적으로 `FAIL missing skill BoneSpear` 로 끝난다 — [warning-cleanup](../warning-cleanup/plan.md) 검증에서 발견, 그 적용 전 소스(`master`)로도 13회 중 4회 실패(기존 문제).
앞으로의 리팩터링은 스모크로 검증하므로, 검증 도구가 흔들리면 회귀와 우연을 가리지 못한다. 경로는 `2D-Portfolio/` 기준.

2026-11-12 임시 진단 로그(커밋하지 않음)로 x64 Release 를 반복 실행해 원인 두 가지를 확인했다.

1. **게임 버그 — 키 상태 미초기화로 플레이어 선택이 무작위로 풀린다.**
   `CObj` 생성자 셋 중 이름을 받는 생성자(`Client/Obj.cpp:34`, 플레이어·몬스터·스킬·UI 가 쓴다)만 `m_dwKey` 를 초기화하지 않는다.
   `CPlayer::CheckKey` 는 `CharacterSelect()`(F1~F4 로 조종 캐릭터 선택)를 **이번 프레임 키를 읽기 전에** 부르므로, 첫 프레임에 쓰레기 `m_dwKey` 를 본다.
   관측: 스테이지 진입 직후 `select Player 1->0 key=0xff777374` — F2·F3 비트가 켜진 쓰레기 값으로 선택이 해제됐고, 이후 플레이어가 입력에 반응하지 않는다.
   실제 게임에서도 Release 로 시작하자마자 캐릭터 조작이 안 될 수 있다.
2. **스모크 설계 빈틈 — 시전 시도가 「목표 지점에 서 있을 때」 분기에만 있다.**
   `Player.cpp` 의 스모크 시전(`bSmokeCast`)은 목표 도달 분기 안에만 있는데, 실제 우클릭 시전은 「도달하지 못했을 때」 분기에도 있다.
   몬스터와 부딪혀 밀리면(`CrashAndSlide`) 도달하지 못한 분기로 넘어가 모션이 RUN 이 되고, 시전 애니메이션(16프레임, 약 1초)이 한 바퀴를 채우지 못해
   생성 조건(`fStart <= 0`)이 오지 않는다. 관측: 시전 시도 수백 회, `fStart` 최솟값 1.1 — 0.5~0.7초마다 밀려 끊김. 30회 중 끊김형 2, 선택 해제형 1.

## Proposed outcome
- `CObj` 의 모든 생성자가 `m_dwKey` 를 0 으로 시작한다 — 시작 직후 선택이 풀리지 않는다.
- 스모크 시전이 실제 우클릭 시전과 같은 두 자리에서 시도된다 — 밀려도 시전이 이어진다.
- x64 Release 를 포함한 4개 구성 Client 스모크가 반복 실행에서 안정적으로 PASS 한다.

## Affected users and systems
사용자: 게임을 Release 로 실행하는 사람(시작 직후 조작 불가 가능성 제거), 이 repo 개발자(믿을 수 있는 스모크)
시스템: `Client/Obj.cpp`, `Client/Player.cpp`(스모크 분기)

## Constraints
반드시 지킬 것:
- 스모크용 코드는 `IsSmokeMode()` 안에만 — `-smoke` 없는 동작은 `m_dwKey` 초기화 외에 바뀌지 않는다.
- 경고 0(`/WX`), 4개 구성 빌드·스모크.

범위 밖:
- 같은 생성자의 다른 미초기화 멤버 정리(`m_pObjName`·`m_pTagetList` 등, 세 생성자 공통) — 이번 실패와 무관. 발견만 기록한다.
- `GetAsyncKeyState`(창이 비활성이어도 전역 키를 읽음) 구조 → `input-unify`.
- 시전 애니메이션·생성 조건 자체 변경.

## Open questions
없음 — 사용자 지시(2026-11-12, 「스모크 안정화부터 진행」). 원인 1은 게임 버그지만 스모크 실패의 원인이라 함께 고친다.
