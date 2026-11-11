# Plan: dead-code-removal

Author: Ryu Jahyeon
Status: approved (사용자 위임 — 「권장안으로 PR 머지까지 진행」, 2026-11-11)

<!-- 입력: 승인된 spec.md (PR #21). 경로는 2D-Portfolio/ 기준. 줄 번호는 master `fea1d7a` 기준. -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Client/{CollisionMgr,NonPlayer,SkillTree,Mouse}.{h,cpp}` | 삭제(8) |
| `Client/{Stage2,Stage3}.{h,cpp}` | 삭제(4) |
| `MFC_000/Page4.{h,cpp}` | 삭제(2) |
| `Headers/Include.h` | `CollisionMgr.h`·`Stage2.h`·`Stage3.h`·`NonPlayer.h`·`SkillTree.h` include 5줄 삭제 |
| `Headers/Constant.h` | `SCENEID_STAGE2`·`SCENEID_STAGE3` 삭제, 주석 처리된 `ITEM_*` 블록 삭제 |
| `Client/SceneMgr.cpp` | `case SCENEID_STAGE2/3` 두 블록 삭제 |
| `Client/SceneObj.cpp` | 주석의 「`CStage·CStage3`」 → 「`CStage`」 |
| `Client/Client.vcxproj`·`.filters` | 위 Client 파일 항목(`Mouse` 는 원래 없음)과 비는 필터 `Stage2`·`Stage3`·`SkillTree` 삭제 |
| `MFC_000/MFC_000.vcxproj`·`.filters` | `Page4` 항목·필터 삭제 |
| `MFC_000/MFC_000.rc`·`resource.h` | `IDD_PAGE4` DIALOGEX 블록, DESIGNINFO 항목, `#define` 삭제(UTF-16 LE·BOM·CRLF 유지) |
| 주석 정리 대상(아래 표) | 주석 처리된 코드 삭제 |
| `CLAUDE.md`, `docs/ARCHITECTURE.md`, `intent/legacy-audit/review.md` | 남은 코드에 맞춤, 「구현되지 않은 기능」 절, 오딧 5번 정정 주석 |

### 주석 처리된 코드 — 대상 줄

| 파일 | 줄 | 내용 |
|---|---|---|
| `Client/BoneSpear.cpp` | 160 | `FrameMove(…Tail…)` |
| `Client/Client.cpp` | 234-240 | `WM_CHAR` 전체화면 토글(`ModeSetter`) |
| `Client/Device.cpp` | 72 | `m_pd3dDevice->SetRenderState` |
| `Client/KeyMgr.h` | 3, 4(줄 끝) | `SingletonTemplate` include·상속 |
| `Client/Logo.cpp` | 59-64 | 옛 `Progress` 본문 |
| `Client/MainGame.cpp` | 15-28 | `#if _DEBUG` 창/전체화면 분기 |
| `Client/MenuButton.h` | 23-34 | `RealRect()` |
| `Client/Monster.cpp` | 70-71, 196-210, 329 | 스크롤 보정, FireWall 그리기, 방향 계산 |
| `Client/MultiTexture.h` | 9 | `vector<TEXINFO*>` 멤버 |
| `Client/Obj.cpp` | 153 | `return D3DXVECTOR3(…)` |
| `Client/ObjMgr.cpp` | 124-182, 257-272, 441-443, 515-521, 533-536, 560, 620(줄 안) | `OBJ_NONPLAYER`·`OBJ_BROKENBUILDING`·`OBJ_BUILDING` case, 플레이어 정보 `DebugLog`, 타겟 설정, 충돌 시 삭제 등 |
| `Client/Player.cpp` | 246, 266-278, 284·312·330·343(줄 안) | DirectInput 키보드 이동, `m_dwKey & KEY_*BUTTON` |
| `Client/Skill.h` | 3 | `#include "Defines.h"` |
| `Client/SortMgr.cpp` | 23-28, 34 | y축 정렬 루프, NULL 검사 |
| `Client/Stage.cpp` | 1494-1535 | 스킬 `AddObject` 블록(`/* … */`) |
| `Client/StageBackground.cpp` | 59-65, 94-103 | 옛 `Progress`·`Render` 본문 |
| `Client/StageObjProto.cpp` | 86-87 | `ItemInfoRect` UI 원형 |
| `Client/UIObj.cpp` | 317-321, 578 | `DEBUG_ENTRY_EXIT`·`DEBUG_LINE`, `return D3DXVECTOR3(…)` |
| `Headers/Constant.h` | 16-20 | `ITEM_*` 플래그 블록 |
| `Headers/Struct.h` | 55-56, 60-61(줄 안), 65-66 | `FRAME` 의 `fScene`·`dwTime` |
| `MFC_000/MiniView.cpp` | 104-108 | 개별 스케일 |
| `MFC_000/Page2.cpp` | 120, 177, 205-206 | `~vector()`, 옛 인덱스 계산 |
| `MFC_000/Page3.cpp` | 58-62, 77-89 | 슬라이더 설정 예시 |

코드 설명이 딸린 주석(예: `// 타겟 설정`)은 그 코드와 함께 지운다. 삭제로 생기는 연속 빈 줄은 1줄로 줄인다.

**남기는 것(설명 주석으로 판정)**: `MFC_000.cpp:107-108`(MFC 마법사 안내문), `MFC_000Doc.cpp:96-98`(마법사 안내문),
`Obj.cpp:96-124`·`MFC_000View.cpp:455-473`(직선 방정식 설명), `Player.cpp:245`(`rgbButtons` 인덱스 설명), `ObjMgr.cpp:618`(작업 메모),
`UIObj.cpp:324`(코드 아님), `MFC_000View.cpp` 의 `/*pDC*/` 류(미사용 인자 이름 — 관용 표기), `MainFrm.cpp:185`(설명), `Include.h` 의 구분선.

## 작업 순서

브랜치 `feature/dead-code-removal`. 각 단계 뒤 Win32 Debug 빌드(오류 0)를 확인하고 커밋한다.

1. **미사용 클래스** — 파일 8개 삭제, `Include.h` 3줄(`CollisionMgr.h`·`NonPlayer.h`·`SkillTree.h`), `Client.vcxproj`·`.filters` 항목.
   → 커밋 `refactor(dead-code-removal): 쓰지 않는 클래스 4개 삭제`
2. **도달 불가 장면** — 파일 4개 삭제, `Include.h` 2줄, `Constant.h` enum 2줄, `SceneMgr.cpp` case 2블록, `SceneObj.cpp` 주석, vcxproj·filters.
   → 커밋 `refactor(dead-code-removal): 도달할 수 없는 CStage2·CStage3 삭제`
3. **툴 Page4** — 파일 2개 삭제, vcxproj·filters, `.rc`·`resource.h`(파이썬으로 UTF-16 디코드→삭제→같은 인코딩으로 기록).
   → 확인: `file` 이 두 파일을 UTF-16 LE·CRLF 로 보고, UTF-8 변환 diff 가 `IDD_PAGE4` 블록·줄뿐. 커밋 `refactor(dead-code-removal): 툴의 쓰지 않는 Page4 삭제`
4. **주석 처리된 코드** — 위 표대로 삭제. → 확인: R7 검사(변경 파일마다 주석·공백 제거 후 `HEAD` 와 동일).
   커밋 `refactor(dead-code-removal): 주석 처리된 코드 삭제`
5. **전체 검증** — 아래 「테스트」.
6. **문서** — CLAUDE.md(「Stage1~3」, 싱글톤·장면·오브젝트·스킬 목록, 경고 기준), `docs/ARCHITECTURE.md`(장면 표·다이어그램·죽은 코드 행·툴 Page4·
   「구현되지 않은 기능」 절), `legacy-audit/review.md` 5번 정정, 이 문서의 「검증 결과」.
   → 커밋 `docs(dead-code-removal): 문서를 남은 코드에 맞추고 구현되지 않은 기능 기록` → PR → 머지 → 브랜치 삭제.

## 테스트

```
rm -rf 2D-Portfolio/Exe 2D-Portfolio/Temp
MSBuild.exe 2D-Portfolio/MyProgram.sln -t:restore -p:RestorePackagesConfig=true -nologo -v:m
MSBuild.exe 2D-Portfolio/MyProgram.sln -p:Configuration=<Debug|Release> -p:Platform=<Win32|x64> -m:1 -nodeReuse:false -nologo -v:m   # 4회
```
- R8: 4회 모두 종료 코드 0·오류 0.
- R9: `grep -c "warning C"` — Win32 197·197, x64 Debug 275·Release 207. 경고 번호별 집계에서 C4715 만 2 → 1.
- R10: CLAUDE.md 「테스트」의 Client·툴 스모크를 4개 구성에서. Debug `RESULT PASS (fail=0)`, Release 는 `FAIL missing skill BoneSpear` 1줄뿐.
  `-smoke` 없이 `timeout 8` 로 Client·툴 실행 → 종료 코드 124(살아 있음), `smoke.log` 없음.
- R4: `git grep -n -w -E "CCollisionMgr|CNonPlayer|CSkillTree|CMouse|CStage2|CStage3|CPage4|SCENEID_STAGE2|SCENEID_STAGE3|IDD_PAGE4" -- 2D-Portfolio` → 0줄
  (`.rc`·`resource.h` 는 `iconv -f UTF-16 -t UTF-8` 후 `grep`).
- R7: 일회용 스크립트 — 문자열·문자 리터럴은 보존하고 `//`·`/* */` 주석과 공백을 모두 제거한 텍스트를 4단계 커밋 전후로 비교, 파일마다 「같음」.

## 리스크

- **줄 번호 어긋남** — 1·2단계가 `Constant.h` 등을 먼저 고친다. 4단계는 줄 번호가 아니라 「지울 줄이 주석·빈 줄인지」를 확인하며 지우고, R7 이 최종 보증.
- **`.rc` 손상** — 인코딩·줄바꿈이 바뀌면 리소스 컴파일러가 깨진 문자열을 만들 수 있다. 3단계 확인 + 툴 빌드·실행으로 본다.
- **vcxproj 항목 누락** — 파일만 지우고 항목을 남기면 빌드 오류(C1083)로 바로 드러난다.
- **첫 스모크 타임아웃** — 클린 빌드 직후 첫 실행은 리소스 첫 읽기로 30초에 근접할 수 있다(x64-migration 기록). 타임아웃 FAIL 이면 한 번 더 돌린다.
