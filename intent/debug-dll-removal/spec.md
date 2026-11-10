# Spec: debug-dll-removal

Author: Ryu Jahyeon
Status: draft

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | 코드·프로젝트에서 `Debug.lib`·`Debug.dll`·`Debug.h`·`DebugMsg`·`CreateConsole`·`DestroyConsole` 참조가 0 | `git grep -n -E "Debug\.h\b\|Debug\.lib\|DebugMsg\|CreateConsole\|DestroyConsole"` 결과가 `intent/`·`docs/` 뿐 |
| R2 | `Client/Debug.lib`·`Client/Debug.dll`·`Client/Debug/Debug.dll`·`Headers/Debug.h` 가 저장소에 없다 | `git ls-files` |
| R3 | Release·Debug `Client.exe` 의 의존 DLL 에 `Debug.dll` 이 없다(따라서 `MSVCR100D.dll` 도 체인에서 사라짐) | `dumpbin /dependents` |
| R4 | Debug 빌드: 디버그 콘솔이 뜨고, 기존 로그(`Player.cpp:98`)가 찍히며, 장면 전환 시 `Scene : <번호>` 가 찍힌다 | 수동 실행 |
| R5 | Release 빌드: 콘솔이 뜨지 않고 장면 로그 코드가 남지 않는다 | 수동 실행 + `MenuD2.cpp` 에 매크로 밖 로그 코드 없음 |
| R6 | MFC_000 의 C4005(`EXPORT` 재정의) 경고가 사라진다 | 빌드 경고 비교 |
| R7 | Win32 Debug·Release 빌드 성공, 게임 흐름(로고→메뉴→스테이지) 정상 | MSBuild 종료 코드 0, 수동 실행 |
| R8 | (x64 구성이 있으면) x64 링크의 LNK4272·`DebugMsgw` LNK2019 가 사라진다 | x64 빌드 |

## 설계

- **DLL 을 다시 만들지 않고 repo 의 `Headers/Debug.hpp` 로 통일** — 같은 기능(콘솔 생성·`DebugLog`)이 이미 소스로 있다.
  중복 구현을 늘리지 않고(DRY), 쓰는 곳이 1곳뿐인 기능에 별도 DLL·플랫폼별 빌드·배포를 두지 않는다(YAGNI·KISS).
- **배포 제외는 컴파일 시점에** — `Debug.hpp` 는 `#ifdef _DEBUG` 밖에서 모든 매크로가 빈 정의(`Debug.hpp:132-140`)라
  Release exe 에 코드·문자열·콘솔이 남지 않는다. DLL 방식은 무조건 링크돼 오히려 Release 에 따라갔다.
- **`MenuD2.cpp` 는 문자열 조립까지 매크로 안으로** — `wsprintf` 두 줄은 매크로 밖이라 Release 에서도 매 프레임 실행되던 코드.
  `DebugLog(L"Scene : %d", iScene)` 한 줄이면 포맷 조립도 Debug 전용이 된다.
- **장면이 바뀔 때만 기록**(`iScene > SCENEID_NONPASS`) — 매 프레임 기록은 콘솔의 다른 로그를 밀어낸다. 바로 위 `InitScene` 조건과 동일.
- `Include.h` 에서 `Debug.h` 만 빼고 `Debug.hpp` include 는 유지 — 두 프로젝트 공용 진입점을 그대로 둔다.

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| `DebugMsg` 가 실제로 어디에도 출력하지 않는지는 정적 분석 추정(`CreateConsole` 호출처 0) | 근거 강도 | plan 1단계에서 변경 전 실행으로 확인, 출력이 있었다면 PR 에 기록 |
| `Debug.hpp` 자체 품질(`itoa` 사용, x64 C4267 2건) | 코드 품질 | 범위 밖 → `warning-cleanup` |
| Release 에서 로그가 필요해질 수 있음 | 운영 | 범위 밖. 필요 시 파일 로그 등 별도 설계 |
| `Client.vcxproj` Post-Build 를 x64 작업(WIP 브랜치)도 수정 | 작업 충돌 | 이 인텐트를 먼저 머지 → x64 브랜치를 rebase 하며 해소 |
| 바이너리 소스가 없어 원래 동작을 완전히 알 수 없음 | 복구 가능성 | 이력 복원 가능. 기능은 `Debug.hpp` 가 대체 |
