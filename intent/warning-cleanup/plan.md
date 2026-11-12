# Plan: warning-cleanup

Author: Ryu Jahyeon
Status: approved (사용자 위임 — 「진행시켜」·`/WX` 결정, 2026-11-12)

<!-- 입력: 승인된 spec.md (PR #38). 경로는 2D-Portfolio/ 기준. -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Headers/Constant.h` | 프레임 수 상수 18개 `constexpr int`(R1) |
| `Headers/Debug.hpp` | `wcslen` → `static_cast<DWORD>`(C4267) |
| `Client/{UIObj,Monster,Player,Obj,ObjMgr,Blizzard,BoneSpear,FireWall,Client,FullModeMgr,StageBackground}.cpp` | 축소 변환 명시(spec 「변환 규칙」) |
| `MFC_000/{MFC_000View,MiniView,Page2}.cpp` | 축소 변환 명시 |
| `Client/Client.vcxproj`·`MFC_000/MFC_000.vcxproj` | 4개 구성 `TreatWarningAsError=true`(R5) |
| `CLAUDE.md`, `docs/FIX-GUIDE.md` | 경고 기준 → 「경고 0, `/WX`」(R7) |

## 작업 순서

1. 프레임 수 상수 — 시험 빌드에서 196 → 74, 새 경고 0 확인 후 커밋.
2. 나머지 68곳(같은 줄 중복 제외) — 변환 규칙대로. Win32·x64 Debug 리빌드 경고 0, Debug 스모크 PASS 후 커밋.
3. `/WX` 켜기 → 커밋.
4. 전체 검증(클린 4개 구성, Client 스모크 3회씩, 툴 스모크, 일반 실행) → 문서 → PR → 머지.

## 테스트

- R2·R5: 4개 구성 빌드 `warning` 0줄, tlog 에 `/WX`(Client 44·MFC_000 16 파일).
- R3: `git diff master -- 2D-Portfolio | grep -E "pragma warning|/wd|DisableSpecificWarnings"` 0줄.
- R6: CLAUDE.md 「테스트」·「실행」.

## 검증 결과 (2026-11-12)

| spec | 결과 |
|---|---|
| R1 | ✅ 18개 `constexpr int` |
| R2 경고 0 | ✅ 4개 구성 클린 빌드 경고 0·오류 0(Win32 196 → 0, x64 Debug 268 → 0·Release 206 → 0) |
| R3 경고 끄기 없음 | ✅ 0줄 |
| R4 값 동일 | ✅ 코드 검토 — 모두 암묵 변환과 같은 결과의 `static_cast`(`double` 식은 리터럴 유지, 결과만 캐스트) |
| R5 `/WX` | ✅ 4개 구성 tlog 에 `/WX`(Client 44·MFC_000 16 파일) |
| R6 스모크·실행 | ⚠️ Client 스모크: Win32 Debug·Release, x64 Debug 각 3/3 PASS. **x64 Release 는 간헐 실패** `missing skill BoneSpear`(첫 검증 1/3, 재실행 9/10 PASS). 툴 4개 구성 PASS, 일반 실행 4개 구성 × Client·툴 살아 있음 |
| R7 문서 | ✅ CLAUDE.md 「빌드」, FIX-GUIDE |

### x64 Release 간헐 실패 — 기존 문제(이 인텐트와 무관)

- **비교**: 이 인텐트 적용 전 소스(`master`, 작업 트리만 잠시 되돌림)로 x64 Release 를 빌드해 같은 스모크를 13회 — **4회 같은 이유로 실패**. 회귀가 아니다.
- **추정 원인**(미확정): 스모크의 스킬 시전 분기(`Player.cpp` 의 `bSmokeCast`)가 「플레이어가 목표 지점에 있을 때」 분기 안에만 있다. 몬스터와 부딪혀 밀리면(`CrashAndSlide`) 그 분기를 타지 않아,
  스테이지 2~4초 창에서 시전 기회를 놓칠 수 있다. ub-fixes 이후 Release 에서도 몬스터가 다가와 공격하게 되며 드러난 것으로 보인다.
- 후속: 스모크 안정화를 별도 인텐트로 다룬다(사용자 결정 대기).
