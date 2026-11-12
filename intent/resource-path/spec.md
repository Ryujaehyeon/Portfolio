# Spec: resource-path

Author: Ryu Jahyeon
Status: draft

<!-- 입력: intent.md. 경로는 2D-Portfolio/ 기준. -->

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | `Headers/Function.h` 에 `SetResourceWorkDir(const TCHAR* const _pAppFolder)` 를 둔다. 작업 디렉터리에 `../Resource` 폴더가 있으면 아무것도 하지 않는다. 없으면 exe 폴더부터 위로 올라가며 `Resource` 와 `<_pAppFolder>` 를 함께 가진 폴더를 찾고, 작업 디렉터리를 그 `<_pAppFolder>` 로 바꾼다. 못 찾으면 그대로 둔다 | 코드 검토 |
| R2 | Client 는 `_tWinMain` 맨 앞(스모크 로그 열기 전)에서 `SetResourceWorkDir(L"Client")` 를 부른다 | 코드 검토 |
| R3 | 툴은 `InitInstance` 의 스모크 분기 뒤에서 `SetResourceWorkDir(L"MFC_000")` 를 부른다 — 툴 스모크의 상대 경로 인자가 원래 작업 디렉터리 기준으로 남는다 | 코드 검토 |
| R4 | 작업 디렉터리를 exe 폴더로 두고 실행해도 동작한다 — Client 스모크 종료 코드 0, 툴 일반 실행이 오류 없이 살아 있음 | 실행 |
| R5 | 기존 방식(작업 디렉터리 `Client/`·`MFC_000/`)의 스모크 통과, 4개 구성 빌드 경고 0 | CLAUDE.md |
| R6 | CLAUDE.md 「실행」에 「exe 를 어디서 실행해도 된다(저장소 안에 있을 때)」를 반영 | 문서 검토 |

## 설계

- **작업 디렉터리를 한 번 맞춘다(KISS).** 상대 경로는 모두 「앱 폴더 기준」이라는 하나의 가정을 공유한다. 그 가정을 시작할 때 성립시키면 호출부 176곳은 바꿀 필요가 없다.
- **이미 맞으면 건드리지 않는다.** `../Resource` 확인으로 VS 디버거·스모크·CLAUDE.md 명령의 동작이 바이트 단위로 같다.
- **위로 올라가며 찾는다.** `Exe/<플랫폼>/<구성>/<프로젝트>/` 깊이(`Directory.Build.props`)를 코드에 박지 않는다. 출력 경로가 바뀌어도 저장소 안이면 찾는다.
- 경로 버퍼는 `MAX_PATH`. `GetModuleFileName` 이 잘리면 아무것도 하지 않는다(지금과 같은 실패).
- 사용하는 API(`GetModuleFileName`·`GetFileAttributes`·`SetCurrentDirectory`)는 kernel32 라 링크 의존성이 늘지 않는다.

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| 저장소 밖으로 exe 만 복사하면 여전히 못 찾음 | 배포 | 범위 밖(intent). 오류 메시지는 지금과 같다 |
| 실행 중 작업 디렉터리가 바뀜 | 부작용 | 바뀌는 경우는 원래 리소스를 못 찾아 실패하던 경우뿐이다 |
