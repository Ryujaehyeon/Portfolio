# Spec: state-log

Author: Ryu Jahyeon
Status: draft

<!-- 입력: intent.md. 경로는 2D-Portfolio/ 기준. -->

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | `CStateRecorder`(Client 싱글턴, `StateRecorder.h/.cpp`): `Begin(경로)` 로 켜고, 꺼져 있으면 모든 함수가 즉시 반환 | 코드 검토 |
| R2 | `Client.cpp` 가 명령줄에 `-log` 가 있으면 `Begin(L"state.log")`. 작업 디렉터리 기준(= `Client/`, resource-path) | 실행 |
| R3 | `Record()` 를 `CMainGame::Progress` 의 장면 갱신 직후 프레임마다 1번 부른다. `CObjMgr::GetObjectMap() const` 로 모든 객체를 돌며 getter 로 스냅숏을 만들고 직전 스냅숏과 비교해 **바뀐 것만** 한 줄씩 쓴다 | 로그 검토 |
| R4 | 기록 항목 — 생성 `+ #id 이름 (키) @(x,y)`, 삭제 `- #id 이름`, 모션 `motion A → B`, HP `hp a → b`, 대상 `target #a → #b`, 충돌·선택 `crash`·`select` 변화, 이동은 마지막으로 기록한 위치에서 **32px 이상** 움직였을 때 `pos (x,y)` | 로그 검토 |
| R5 | `CSceneMgr::InitScene` 이 `OnScene(ID)` 를 불러 `scene N` 을 쓰고 직전 스냅숏을 비운다(장면 교체로 사라진 객체는 따로 삭제 줄을 쓰지 않는다) | 로그 검토 |
| R6 | 줄 형식 `[  경과 ms #프레임] 내용`, UTF-8, 줄마다 바로 쓴다(버퍼 없음 — 크래시해도 마지막 줄까지 남음) | 로그 검토 |
| R7 | 저장한 포인터는 키로만 쓴다. 같은 주소에서 이름이 달라지면 삭제 + 생성으로 기록한다 | 코드 검토 |
| R8 | `-log` 없는 실행·스모크는 그대로. `-smoke -log` 스모크가 PASS 이고 `state.log` 에 `scene 2`·`+ `·`- ` 줄이 있다. 4개 구성 빌드 경고 0 | 실행 |
| R9 | `state.log` gitignore. CLAUDE.md 「테스트」 또는 「실행」에 `-log` 사용법 | 문서 검토 |

## 설계

- **기존 공개 접근자만 읽는다.** `CObj` 에 `GetName`·`GetInfo`·`GetStatas`·`GetpMotion`·`GetTagetObj`·`GetCrash`·`GetSelect`·`GetObjType` 이 이미 있다. 기록기는 공개 인터페이스에만 의존하고(DIP), 객체 클래스는 바뀌지 않는다. friend 는 결과가 같으면서 결합만 강하다.
- **`CObjMgr` 에 읽기 접근자 하나** — `const map<wstring, list<CObj*>>& GetObjectMap() const`. 맵 키(`OBJ_PLAYER` 등)를 줄에 함께 쓴다.
- **스냅숏은 값으로 복사** — 이름·모션은 `wstring` 으로 복사한다(원본 포인터 수명에 기대지 않는다). 대상은 포인터를 키로 바꾼 `#id`(직전 또는 이번 스냅숏에서 찾고, 없으면 `?`).
- **id** 는 처음 본 순서대로 1부터. 같은 포인터가 이어지는 동안 유지한다.
- **이동 기준 32px** — 매 프레임 위치를 쓰면 줄이 폭증하고, 타일 크기 정도의 이동은 경로를 따라가기에 충분하다. 생성·삭제 줄에는 항상 위치를 쓴다.
- 파일 쓰기는 `SmokeWriteLine` 과 같은 방식(UTF-8 변환 후 `WriteFile`)이지만 머리말(프레임 번호)이 달라 기록기 안에 둔다.
- 해제는 `CMainGame::Release` 에서 `DestroyInstance`(파일 닫기).

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| 누가 값을 바꿨는지 모름, 프레임 안의 왕복 변화 누락 | 진단력 | 범위 밖(intent). 프레임 번호로 좁힌 뒤 디버거 |
| 객체가 많은 장면에서 프레임마다 비교 비용 | 성능 | `-log` 일 때만. 객체 수십 개 × 필드 몇 개 비교 |
| 해제 후 같은 주소에 다른 객체 | 정확성 | 이름이 바뀌면 삭제 + 생성. 이름까지 같으면 이어진 것으로 보인다 — 진단 도구로 허용 |
