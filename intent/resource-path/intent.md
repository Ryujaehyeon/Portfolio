# Intent: exe 를 어디서 실행해도 리소스를 찾게

Author: Ryu Jahyeon
Status: draft

## Problem
리소스 경로가 작업 디렉터리 기준이다(오딧 [12번](../legacy-audit/review.md)). 경로는 `2D-Portfolio/` 기준.

- 텍스처는 `L"../Resource/Texture/…"`(Client 약 160곳, 툴 몇 곳), 사운드는 `L"Tristram.mp3"`, 맵은 `L"Map.dat"` — 모두 작업 디렉터리가 `Client/`(툴은 `MFC_000/`)라고 가정한다.
- exe 는 `Exe/<플랫폼>/<구성>/<프로젝트>/` 에 생긴다. 탐색기에서 더블클릭하면 작업 디렉터리가 exe 폴더가 되어 리소스를 못 찾고 오류 메시지박스가 뜬다. VS 디버거·CLAUDE.md 명령처럼 작업 디렉터리를 맞춰 줄 때만 동작한다.

## Proposed outcome
- 저장소 안의 exe 를 더블클릭하거나 다른 작업 디렉터리에서 실행해도 Client·툴이 리소스를 찾는다.
- 작업 디렉터리를 맞춰 실행하는 지금 방식(VS 디버거, 스모크 명령)은 그대로 동작한다.

## Affected users and systems
사용자: 이 repo 개발자·포트폴리오를 받아 보는 사람(빌드한 exe 를 바로 실행할 수 있다)
시스템: `Headers/Function.h`, `Client/Client.cpp`, `MFC_000/MFC_000.cpp`, CLAUDE.md

## Constraints
반드시 지킬 것:
- 리소스 경로 문자열(176곳)과 `Resource/`·`*.dat` 는 건드리지 않는다.
- 스모크 동작·로그 위치는 그대로다. 경고 0.

범위 밖:
- 저장소 밖으로 배포할 수 있는 패키지(리소스를 exe 옆에 복사) — 필요해지면 별도 인텐트.
- 경로 해석 함수로 호출부를 모두 감싸는 방식.

## Open questions
없음 — 사용자 결정(2026-11-12, 「시작 시 작업 폴더 맞춤」).
