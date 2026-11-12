# Spec: warning-cleanup

Author: Ryu Jahyeon
Status: draft

<!-- 입력: 승인된 intent.md (PR #37). 경로는 2D-Portfolio/ 기준. -->

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | 프레임 수 상수 18개(`PLAYER_*`·`SKELETON_*`·`SUMMONER_*`, 값이 모두 정수)가 `constexpr int` 다 | 코드 검토 |
| R2 | 두 프로젝트·4개 구성 클린 빌드의 경고가 0 이다(`/W3`) | 빌드 로그 `warning C` 0줄 |
| R3 | 경고를 끄지 않는다 — `#pragma warning(disable…)`·`/wd`·`DisableSpecificWarnings` 가 새로 생기지 않는다 | `git diff` |
| R4 | 축소 변환을 명시할 때 결과 값이 지금과 같다(아래 「변환 규칙」) | 코드 검토 + R6 |
| R5 | 두 프로젝트·4개 구성에 `<TreatWarningAsError>true</TreatWarningAsError>`(`/WX`) — 실제 명령줄에 `/WX` | tlog |
| R6 | 4개 구성 Client 스모크(구성마다 3회)·툴 스모크 PASS, 일반 실행 8초 살아 있음 | CLAUDE.md 「테스트」·「실행」 |
| R7 | CLAUDE.md 「빌드」의 경고 기준 수가 「경고 0, `/WX`」로 바뀐다. FIX-GUIDE 의 경고 관련 안내도 맞춘다 | 문서 검토 |

## 설계

- **프레임 수 상수 → `constexpr int`**: 텍스처 개수(`InsertTexture` 의 `int`)로 넘기는 122곳의 경고가 사라진다. `FrameMove(const float&, …)`·비교식(`fStart >= PLAYER_ATTACK-0.5f`)에 쓰일 때는
  정수 상수가 `float` 로 바뀌는데, 값이 정확히 표현되는 상수라 경고가 없고 값도 같다(시험 빌드로 확인 — 새 경고 0). 근원 한 곳을 고친다(DRY).
- **변환 규칙**(나머지): 암묵 변환을 같은 결과의 `static_cast` 로 바꾼다.
  | 경고 | 꼴 | 고침 |
  |---|---|---|
  | C4244 `int`/`LONG`/`UINT` → `float` | 정수를 실수에 | `static_cast<float>(x)` |
  | C4244 `double` → `float` | `float * 0.5` 같은 `double` 식 | 식 전체를 `static_cast<float>(…)` — 리터럴을 `0.5f` 로 바꾸지 않는다(중간 정밀도가 바뀌면 값이 달라질 수 있다) |
  | C4244 `float` → `LONG`/`int` | 실수를 정수에(0 쪽으로 자름) | `static_cast<LONG>(x)`/`static_cast<int>(x)` — 같은 자름 |
  | C4244 `time_t` → `unsigned int` | `srand(time(…))` | `static_cast<unsigned int>(…)` |
  | C4838 | `RECT rc = { float… }` | 각 원소를 `static_cast<LONG>(…)` |
  | C4267 (x64) | `size()`·`strlen` → `int`/`DWORD` | `static_cast<int>`/`static_cast<DWORD>` — 크기는 2^31 미만(타일 800 등) |
  | C4018 | signed/unsigned 비교 | 실제 값 범위에 맞춰 한쪽을 캐스트(코드를 보고 정함, 음수가 될 수 있는 쪽을 보존) |
- **`/WX`** 는 경고 0 을 확인한 뒤 마지막에 켠다.
- 고치는 줄은 CLAUDE.md 「C++ 코딩 표준」 품질 규칙을 따른다. 주변 정리는 하지 않는다.

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| 캐스트가 실제 버그(잘림·정밀도 손실)를 숨김 | 정확성 | 이 인텐트는 「지금과 같은 결과」를 명시만 한다. 고칠 때 버그로 보이는 곳이 있으면 plan 에 기록만 하고 고치지 않는다 |
| `/WX` 로 이후 작업에서 경고 하나가 빌드를 막음 | 개발 편의 | 사용자 결정으로 수용. 새 경고는 고치는 것이 규칙(I-56) |
| 컴파일러 업데이트로 새 경고가 생겨 빌드가 깨질 수 있음 | 빌드 안정성 | 수용 — 그때 고친다. CLAUDE.md 에 적는다 |
