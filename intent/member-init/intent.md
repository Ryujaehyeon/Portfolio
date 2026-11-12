# Intent: 모든 멤버를 생성자 초기화 리스트에서 초기화

Author: Ryu Jahyeon
Status: draft

## Problem
여러 클래스·구조체가 생성자에서 멤버를 초기화하지 않아, 처음 쓰이기 전 값이 **쓰레기**다. 같은 원인의 버그를 이번 세션에서만 세 번 고쳤다.
경로는 `2D-Portfolio/` 기준, 2026-11-12 `master`(`b96faaf`)에서 조사.

- [smoke-log](../smoke-log/plan.md): `CPlayer`·`CSceneObj` 의 `m_pSkillPrototype` — 종료 때 쓰레기 포인터 해제(크래시).
- [string-key-compare](../string-key-compare/plan.md): `CUIObj` 의 `m_PlayerObj` 등 — x64 Release 간헐 접근 위반.
- [smoke-stabilize](../smoke-stabilize/intent.md): `CObj` 이름 받는 생성자의 `m_dwKey` — 시작 직후 플레이어 선택이 무작위로 풀림.
- 남은 규모(대략 집계): Client·툴의 클래스 약 30개 중 대부분이 일부 멤버를 생성자에서 초기화하지 않는다(`CObj` 는 20개 중 15개가 일부 생성자에서 빠짐).
  생성자가 여럿인 클래스는 생성자마다 빠진 멤버가 다르다 — `m_dwKey` 가 정확히 그 경우였다(다른 두 생성자에는 있었다).
- `Headers/Struct.h` 구조체 11개는 생성자가 없거나 비어 있다(`tagObjInfo(){}`, `tagFrame(){}`). `OBJINFO` 의 복사 생성자는 멤버 15개 중 5개만 복사한다.
- 코딩 표준 I-61(「멤버 변수 초기화는 초기화 리스트를 기본으로」)·II-6(한 줄에 하나)을 지키지 못한다.

## Proposed outcome
- 모든 클래스·구조체의 **모든 생성자**가 기본 타입·포인터·열거형·값 구조체(`D3DXVECTOR3`·`D3DXMATRIX`·`RECT` 등) 멤버를 초기화 리스트에서 초기화한다.
  생성자가 하나면 그 생성자에, 여럿이면 각각에 — 어느 생성자로 만들어도 쓰레기 멤버가 없다.
- 4개 구성 스모크·일반 실행이 지금처럼 PASS. 경고 0(`/WX`).
- CLAUDE.md 「코드 관례」에 규칙을 적는다(새 멤버를 추가하면 모든 생성자에 넣는다).

## Affected users and systems
사용자: 게임·툴 사용자(쓰레기 값에 기대던 잠재 버그 제거), 이 repo 개발자
시스템: Client·MFC_000 의 클래스 헤더·소스, `Headers/Struct.h`·`SmokeLog.h`·`Debug.hpp`, CLAUDE.md

## Constraints
반드시 지킬 것:
- 기본값: 포인터 `nullptr`, 수 `0`, `bool` `false`, 열거형은 0 에 해당하는 열거자를 이름으로, 벡터는 0, 행렬은 단위 행렬, 배열·C 구조체는 값 초기화 `()`.
  이미 다른 값을 주고 있는 생성자는 그 값을 초기화 리스트로 옮기기만 한다(값을 바꾸지 않는다).
- `.dat` 호환 — 구조체에 생성자를 더해도 멤버 순서·크기·정렬은 그대로(툴·게임의 `Map.dat` 읽기 스모크로 확인).
- 스스로 초기화되는 클래스 타입 멤버(`wstring`·`list`·`map`·`vector`·`CString`·MFC 창 객체)는 적지 않는다(사용자 결정).
- 기존 명명·구조 관례 유지, 경고 0.

범위 밖:
- 멤버의 의미·사용 방식 변경, 생성자 구조 재설계(위임 생성자 도입 등), 서드파티(`fmod`).

## Open questions
없음 — 사용자 결정(2026-11-12): 클래스와 구조체 모두, 초기화 리스트로. 생성자가 여럿인 클래스는 각 생성자에 모두 적는다(기본 생성자의 리스트는 다른 생성자에서 실행되지 않으므로).
`OBJINFO` 복사 생성자는 빠진 10개 멤버도 원본에서 복사한다(지금은 쓰레기로 남는다 — spec 에서 영향 확인).
