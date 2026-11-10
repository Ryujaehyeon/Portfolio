# Intent: 소스 인코딩 UTF-8 BOM 통일

Author: Ryu Jahyeon
Status: approved (사용자, 2026-11-10 대화 승인)

## Problem
C/C++ 소스 70개가 BOM 없는 CP949 다. MSVC 는 BOM 없는 소스를 시스템 코드 페이지로 읽으므로
한국어 Windows(ACP 949)에서만 올바르게 빌드되고, 다른 로케일에서는 `L"장치 초기화 실패"` 같은 리터럴이
조용히 깨진다. GitHub(UTF-8 전제)에서는 공개 포트폴리오의 한글 주석이 전부 깨져 보인다.
VS2026 편집기는 UTF-8 을 기본으로 저장하므로 편집하다 보면 두 인코딩이 섞인다.

## Proposed outcome
- 한글이 든 소스가 모두 UTF-8 BOM 이 되고, 어느 로케일에서나 같은 바이너리가 나온다.
- GitHub 에서 한글 주석이 읽힌다.
- `.editorconfig` 로 이후 저장 인코딩이 고정된다.

## Affected users and systems
사용자: 이 repo 개발자, GitHub 열람자
시스템: `2D-Portfolio/` 의 `.cpp`·`.h`, `MFC_000/ReadMe.txt`

## Constraints
반드시 지킬 것: 인코딩만 바꾼다(내용·줄바꿈 불변). 컴파일 결과가 바뀌면 안 된다.
범위 밖: UTF-16 리소스 파일(`.rc`·`.rc2`·`resource.h`), `MFC_000.reg`(regedit 은 UTF-8 `.reg` 미지원),
ASCII 전용 파일(변환 불필요).

## Open questions
없음.
