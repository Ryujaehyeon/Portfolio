# Plan: ub-fixes

Author: Ryu Jahyeon
Status: approved (사용자 위임 — 「추천안 + 머지까지」, 2026-11-11)

<!-- 입력: 승인된 spec.md (PR #25). 경로는 2D-Portfolio/ 기준. 줄 번호는 master `00c0580` 기준. -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Client/Obj.cpp` | `FrameMove` 의 `#if _DEBUG` 분기 제거, 리셋 한 가지로(R1) |
| `Client/UIObj.cpp` | `VelueToPercentage` 끝에 `return -1.f;`(R3) |
| `Client/ObjMgr.cpp` | `Progress`: `iScene` 초기화(R4), 증가 위치 이동·`erase` 경로(R5), `ColCircle` 호출 이동(R6), `static_cast` 제거(R7). `ColCircle` 재작성(R6) |
| `Client/SceneMgr.cpp` | `Progress` 에서 반환값 보고 `InitScene`(R8) |
| `Client/Logo.cpp`·`MenuD2.cpp`·`Stage.cpp` | `Progress` 의 `InitScene` 호출 삭제(R8) |
| `CLAUDE.md`, `docs/FIX-GUIDE.md`, `intent/smoke-log/plan.md` | 「알려진 실패」 제거, 경고 기준, 기록(R11) |

## 작업 순서

브랜치 `feature/ub-fixes`. 각 단계 뒤 Win32 Debug 빌드 오류 0 + Win32 Debug 스모크 PASS 를 확인하고 커밋한다.

1. **`FrameMove` 통일** → Win32 **Release** 스모크도 돌려 `missing skill BoneSpear` 가 사라지는지 먼저 확인(이 인텐트의 핵심 가설 검증).
   커밋 `fix(ub-fixes): FrameMove 를 구성과 무관하게 같은 규칙으로`
2. **반환값** — `VelueToPercentage` `-1.f`, `ObjMgr::Progress` `iScene` 초기화. 커밋 `fix(ub-fixes): 반환하지 않거나 초기화 안 된 값 수정`
3. **`ObjMgr::Progress` 반복·캐스트** — `erase` 건너뛰기, `static_cast`. 커밋 `fix(ub-fixes): ObjMgr::Progress 의 원소 건너뛰기와 잘못된 캐스트 수정`
4. **`ColCircle`** — 재작성 + 한 프레임 한 번 호출. 커밋 `fix(ub-fixes): ColCircle 이 모든 스킬·몬스터 쌍을 한 번씩 검사`
5. **장면 전환** — `CSceneMgr::Progress` 로 이동. 커밋 `fix(ub-fixes): 장면 교체를 CSceneMgr 에서`
6. **전체 검증**(아래) → **문서** → PR → 머지 → 브랜치 삭제.

## 테스트

dead-code-removal 과 같은 절차: `Exe`·`Temp` 삭제 → restore → 4개 구성 빌드(`-m:1 -nodeReuse:false`) → 4개 구성 Client·툴 스모크 → `-smoke` 없이 8초 실행.
- R2: Client 스모크 4개 구성 모두 `RESULT PASS (fail=0)`, 종료 코드 0.
- R9: 경고 Win32 196·196, x64 Debug 268·Release 206, C4715 0.
- R7·R8: `git grep -n -E "static_cast<C(Player|Monster)\*>" -- 2D-Portfolio/Client/ObjMgr.cpp` 0줄, `git grep -n InitScene -- 2D-Portfolio/Client` 가 `SceneMgr.*`·`MainGame.cpp` 뿐.

## 리스크

- **1단계 가설이 틀릴 수 있음** — Release 스모크가 여전히 실패하면 멈추고 원인을 다시 조사한다(다른 Release 전용 차이).
- **Release 에서 새로 동작하는 경로의 크래시**(spec flag) — 스모크 종료 코드가 0 이 아니거나 `RESULT` 줄이 없으면(크래시) 멈추고 보고.
- **`ColCircle` 의 체감 변화** — 자동 검증 불가. PR 에 명시.

## 구현 중 변경

- 4단계에서 `ColCircle` 호출을 옮기며 `Progress` 루프 안의 `iterSkill` 이 쓰이지 않게 돼 함께 지웠다(변경으로 생긴 고아).
- 1단계 Debug 빌드가 LNK1168 로 실패 — dead-code-removal 검증 때 `timeout 8` 로 띄운 Client·툴이 Windows 에 남아 exe 를 잡고 있었다.
  그 두 프로세스를 끝내고 다시 빌드했다. 이번 검증의 일반 실행은 PowerShell 로 띄우고 직접 끝냈고, CLAUDE.md 「실행」에 함정을 적었다.

## 검증 결과 (2026-11-11)

`Exe`·`Temp` 삭제 → restore → 4개 구성 빌드(`-m:1 -nodeReuse:false`) → 4개 구성 스모크 → PowerShell 로 8초 일반 실행.

| spec | 결과 |
|---|---|
| R1 `FrameMove` | ✅ `#if _DEBUG` 분기 없음, 리셋 한 가지 |
| R2 4개 구성 Client 스모크 | ✅ Win32 Debug 8.7초·Release 8.6초, x64 Debug 8.0초·Release 7.8초 — 모두 종료 코드 0·`RESULT PASS (fail=0)`. Release 의 `missing skill BoneSpear` 사라짐(1단계 직후 Win32 Release 로 먼저 확인) |
| R3 `VelueToPercentage` | ✅ C4715 0 |
| R4·R5 `Progress` 초기화·건너뛰기 | ✅ 코드 검토, 단계마다 Debug 스모크 PASS |
| R6 `ColCircle` | ✅ 코드 검토 + 4개 구성 스모크(스킬 생성·몬스터 10). 데미지·명중 수는 눈으로 확인하지 않음 |
| R7 `static_cast` | ✅ `ObjMgr.cpp` 0줄 |
| R8 장면 교체 | ✅ `InitScene` 은 `SceneMgr.*`·`MainGame.cpp:21` 뿐, 스모크 `scene 0/1/2 init` PASS |
| R9 경고 | ✅ Win32 197 → 196, x64 Debug 269 → 268·Release 207 → 206 — C4715 만 감소, 새 종류 0 |
| R10 툴·일반 실행 | ✅ 툴 4개 구성 `tiles=800`·종료 코드 0. 일반 실행(Win32 Debug·Release) Client·툴 8초 뒤 살아 있음, 창 제목에 `system err` 없음, `smoke.log` 없음, 남은 프로세스 0 |
| R11 문서 | ✅ CLAUDE.md 「알려진 실패」 삭제·경고 기준·WSL 실행 함정, FIX-GUIDE 순서·경고 안내, smoke-log 기록 |

- 게임 플레이(데미지·광역 명중·몬스터 반응·HP 바 표시)는 눈으로 확인하지 않았다 — `ColCircle` 의 체감 변화는 사용자 결정으로 수용한 범위.
