# Intent: 객체 상태 변화 기록(state.log)

Author: Ryu Jahyeon
Status: draft

## Problem
게임 중 문제가 생겨도 「그 직전에 무슨 일이 있었는지」 남는 기록이 없다. 경로는 `2D-Portfolio/` 기준.

- Debug 콘솔(`Headers/Debug.hpp`)은 `CPlayer::Progress` 가 매 프레임 지우고 마우스·좌표·스크롤 3줄만 다시 쓴다 — 이력이 없고, 로고·메뉴에서는 비어 있다.
- `smoke.log` 는 스모크 판정(PASS/FAIL)만 남긴다.
- 예: Release 에서 드물게 나던 접근 위반(smoke-input-isolation·frame-index-bounds)은 기록이 없어 원인을 좁히는 데 반복 실행과 추정에 기대야 했다.

## Proposed outcome
- `-log` 로 실행하면 Client 가 `state.log` 에 객체 상태의 **변화**를 시간·프레임 순으로 남긴다 — 장면 전환, 객체 생성·삭제, 모션·HP·대상·충돌·선택 변화, 일정 거리 이상의 이동.
- `-log` 가 없으면 아무것도 하지 않는다(일반 실행·스모크 동작 그대로). `-smoke` 와 함께 쓸 수 있다.

## Affected users and systems
사용자: 이 repo 개발자(문제 재현 시 직전 상태 변화를 읽는다). 게임 사용자에게는 변화 없음
시스템: 새 `Client/StateRecorder.h/.cpp`, `Client/ObjMgr.h`(읽기 접근자 1개), `MainGame.cpp`·`SceneMgr.cpp`·`Client.cpp`(호출), vcxproj, `.gitignore`, CLAUDE.md

## Constraints
반드시 지킬 것:
- 객체 클래스(`CObj` 계열)는 고치지 않는다 — 이미 있는 공개 const 접근자(`GetInfo`·`GetStatas`·`GetpMotion` 등)만 읽는다. friend 를 쓰지 않는다.
- 저장한 객체 포인터는 비교 키로만 쓰고 역참조하지 않는다(삭제된 객체 접근 금지).
- 경고 0. 스모크 결과 그대로.

범위 밖:
- 값을 바꾼 코드(호출 위치) 기록, 한 프레임 안에서 바뀌었다 돌아온 값 — 프레임 경계 비교의 한계로 받아들인다.
- 입력·난수 기록과 재생(리플레이), 툴(MFC_000) 기록.
- Debug 콘솔 출력 정리.

## Open questions
없음 — 사용자 결정(2026-11-12): friend 대신 기존 getter 로 `m_MapObject` 를 돌며 스냅숏·diff.
