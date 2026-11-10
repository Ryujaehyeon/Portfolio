# Spec: debug-dll-removal

Author: Ryu Jahyeon
Status: approved (사용자, 2026-11-10) · 2026-11-10 장면 로그 출력을 B안(VS 출력 창)으로 변경

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | 코드·프로젝트에서 `Debug.lib`·`Debug.dll`·`Debug.h`·`DebugMsg`·`CreateConsole`·`DestroyConsole` 참조가 0 | `git grep -n -E "Debug\.h\b\|Debug\.lib\|DebugMsg\|CreateConsole\|DestroyConsole"` 결과가 `intent/`·`docs/` 뿐 |
| R2 | `Client/Debug.lib`·`Client/Debug.dll`·`Client/Debug/Debug.dll`·`Headers/Debug.h` 가 저장소에 없다 | `git ls-files` |
| R3 | Release·Debug `Client.exe` 의 의존 DLL 에 `Debug.dll` 이 없다(따라서 `MSVCR100D.dll` 도 체인에서 사라짐) | `dumpbin /dependents` |
| R4 | Debug 빌드: 디버그 콘솔이 뜨고 기존 상태 로그(`Player.cpp:98`)가 찍힌다. **모든 장면 전환**이 VS 출력 창에 `Scene : 0` → `Scene : 1` → `Scene : 2` 순서로 남는다 | VS F5 출력 창(또는 DebugView), 로고→메뉴→「시작」 |
| R5 | Release 빌드: 콘솔이 뜨지 않고 장면 로그 코드가 남지 않는다 | 수동 실행 + `DebugOut` 이 `#else` 에서 빈 매크로, 매크로 밖 로그 코드 없음 |
| R6 | MFC_000 의 C4005(`EXPORT` 재정의) 경고가 사라진다 | 빌드 경고 비교 |
| R7 | Win32 Debug·Release 빌드 성공, 게임 흐름(로고→메뉴→스테이지) 정상 | MSBuild 종료 코드 0, 수동 실행 |
| R8 | (x64 구성이 있으면) x64 링크의 LNK4272·`DebugMsgw` LNK2019 가 사라진다 | x64 빌드 |
| R9 | `Headers/Debug.hpp` 에 `DebugOut(fmt, ...)` 이 있다 — `_DEBUG` 에서는 형식 문자열을 조립해 `OutputDebugStringW` 로 보내고(줄바꿈 포함, 버퍼 넘침 시 잘라냄), 그 밖에서는 빈 매크로 | 코드 검토 + R4·R5 |
| R10 | 장면 로그 코드는 `CSceneMgr::InitScene` **한 곳**에만 있다(`MenuD2.cpp` 등 장면 쪽에는 없음) | `git grep -n "Scene : " -- 2D-Portfolio` 결과 1건 |

## 설계

- **DLL 을 다시 만들지 않고 repo 의 `Headers/Debug.hpp` 로 통일** — 같은 기능(콘솔 생성·`DebugLog`)이 이미 소스로 있다.
  중복 구현을 늘리지 않고(DRY), 쓰는 곳이 1곳뿐인 기능에 별도 DLL·플랫폼별 빌드·배포를 두지 않는다(YAGNI·KISS).
- **배포 제외는 컴파일 시점에** — `Debug.hpp` 는 `#ifdef _DEBUG` 밖에서 모든 매크로가 빈 정의(`Debug.hpp:132-140`)라
  Release exe 에 코드·문자열·콘솔이 남지 않는다. DLL 방식은 무조건 링크돼 오히려 Release 에 따라갔다.
- **`MenuD2.cpp` 의 `wsprintf`·`DebugMsg` 는 지운다** — `wsprintf` 두 줄은 매크로 밖이라 Release 에서도 매 프레임 실행되던 코드.
- **장면 로그는 `CSceneMgr::InitScene` 한 곳에서** (2026-11-10 변경) — 모든 전환(시작→로고 `MainGame.cpp:35`, 로고→메뉴 `Logo.cpp:69`,
  메뉴→스테이지 `MenuD2.cpp`)이 이 함수를 지난다. 장면마다 넣으면 로고→메뉴(1)처럼 빠지는 전환이 생기고 복붙이 늘어난다(DRY).
  처음 1차 구현은 메뉴 안에 두어 「메뉴를 떠날 때」만 찍혔다.
- **장면 로그는 콘솔이 아니라 VS 출력 창으로**(B안) — 콘솔은 스테이지에서 `CPlayer::Progress()` 가 매 프레임 `DebugLogClear`(`Player.cpp:97`)로
  지우는 **상태 표시판**이다. 한 번 일어나는 이벤트(장면 전환)를 거기 쓰면 바로 지워진다. 이벤트 로그(출력 창, 누적)와
  상태 표시(콘솔, 매 프레임 갱신)를 분리한다(SRP). `Debug.hpp` 의 `DEBUG_ENTRY_EXIT`·`DEBUG_LINE` 도 이미 출력 창을 쓴다.
- **`DebugOut` 은 `DebugLog` 와 같은 모양**(`fmt, ...`)으로 — 호출부는 매크로 이름만 고르면 된다. 고정 크기 스택 버퍼에
  `_snwprintf_s(..., _TRUNCATE, ...)` 로 조립해 넘침 시 잘라낸다(`swprintf_s` 는 넘치면 Debug CRT 가 invalid parameter 로 중단).
- `Include.h` 에서 `Debug.h` 만 빼고 `Debug.hpp` include 는 유지 — 두 프로젝트 공용 진입점을 그대로 둔다.

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| `DebugMsg` 가 실제로 어디에도 출력하지 않는지는 정적 분석 추정(`CreateConsole` 호출처 0) | 근거 강도 | plan 1단계에서 변경 전 실행으로 확인, 출력이 있었다면 PR 에 기록 |
| `Debug.hpp` 자체 품질(`itoa` 사용, x64 C4267 2건) | 코드 품질 | 범위 밖 → `warning-cleanup` |
| 범위 확장: `DebugOut` 매크로 신설, 로그 위치 이동 | intent 범위 | 사용자 결정(2026-11-10, B안) — 원래 목표(DLL 제거)의 대체 로그가 실제로 보이게 하는 데 필요 |
| `OutputDebugString` 은 디버거(VS)나 DebugView 가 붙어 있어야 보인다 | 사용성 | 수용. VS F5 가 기본 실행 방법(CLAUDE.md) |
| `DebugOut` 버퍼 256자 초과 시 잘림 | 정확성 | 수용. 장면 로그는 수십 자 |
| Release 에서 로그가 필요해질 수 있음 | 운영 | 범위 밖. 필요 시 파일 로그 등 별도 설계 |
| `Client.vcxproj` Post-Build 를 x64 작업(WIP 브랜치)도 수정 | 작업 충돌 | 이 인텐트를 먼저 머지 → x64 브랜치를 rebase 하며 해소 |
| 바이너리 소스가 없어 원래 동작을 완전히 알 수 없음 | 복구 가능성 | 이력 복원 가능. 기능은 `Debug.hpp` 가 대체 |
