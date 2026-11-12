# Plan: member-init

Author: Ryu Jahyeon
Status: approved (사용자 결정 — 클래스·구조체 모두, 생성자마다 모두 적기, 2026-11-12)

<!-- 입력: 승인된 spec.md (PR #44). 경로는 2D-Portfolio/ 기준. -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Headers/Struct.h` | 구조체 10개 기본 생성자 추가·보완(`AStarNode`·`tagItem`·`tagInfo`·`tagBoneSpear`·`tagFireWall`·`tagBlizzard`·`tagSkill`·`tagTexture`·`tagTile`, `tagObjInfo`·`tagFrame`), `OBJINFO` 복사 생성자 전 멤버 |
| `Headers/SmokeLog.h` | `SMOKESTATE` 기본 생성자, 정적 변수의 집합체 초기화 제거 |
| `Headers/Debug.hpp` | `__debugclass` 생성자 초기화 리스트 |
| `Client/GameSound.hpp` | `SoundDevice` 생성자에 빠진 멤버 |
| `Client/{Obj,AStar,Blizzard,BoneSpear,FireWall,Device,FullModeMgr,KeyMgr,MenuButton,Monster,ObjMgr,Player,SceneObj,TimeMgr}.cpp` | 생성자마다 빠진 대상 멤버 |
| `MFC_000/{Device,MFC_000,MFC_000View,Page1,Page3}.cpp` | 같음 |
| `CLAUDE.md` | 「코드 관례」 규칙(R8) |

## 작업 순서

1. 검사 스크립트(일회용)로 클래스별·생성자별 누락 집계 — 시작 시 누락 189.
2. 구조체: 타입별 기본값 규칙으로 기본 생성자 생성. `OBJINFO` 복사 생성자 전 멤버 복사.
3. 클래스: 규칙 도구로 각 생성자의 초기화 리스트를 다시 쓴다 — 기존 항목(기반 생성자 호출·기존 값)은 유지, 빠진 대상 멤버를 기본값으로, 선언 순서, 한 줄에 하나.
   본문의 고정 값 대입은 리스트로 옮긴다(`CObjMgr` 0 두 개, `CMFC_000App::m_bHiColorIcons = TRUE`). 기반 멤버를 대입하는 파생 생성자 본문(`m_pObjKey = _ObjName` 등)은 그대로 둔다.
4. 헤더 정의 클래스(`SMOKESTATE`·`__debugclass`·`SoundDevice`)와 Client·툴 사본(`CDevice`) 은 직접 확인·수정.
5. 재검사 누락 0, 빌드·스모크 → 커밋 `fix(member-init): 모든 생성자가 멤버를 초기화 리스트에서 초기화` → 전체 검증 → 문서 → PR → 머지.

## 테스트

- 검사 스크립트 누락 0(같은 이름 사본 `CDevice` 의 오탐 5건은 툴 헤더로 직접 확인), 헤더 인라인 생성자 전수 확인.
- 4개 구성 클린 빌드 경고 0·오류 0, Client 스모크 각 5회(x64 Release 10회), 툴 스모크, 일반 실행.

## 리스크

- 스캐너가 놓치는 생성자 — 실제로 `GameSound.hpp` 끝의 한 줄짜리 `SoundDevice():_volume(1.0f){}` 를 놓쳐 중복 생성자 컴파일 오류(C2535)로 드러났다. 헤더 인라인 생성자를 정규식으로 따로 전수 확인했다.
- 쓰레기 값에 기대던 동작 변화 — 스모크·일반 실행으로 확인.

## 구현 중 변경

- 스캐너가 `GameSound.hpp` 끝의 한 줄짜리 `SoundDevice():_volume(1.0f){}` 를 놓쳐 생성자를 하나 더 만들었고, 중복 생성자 오류(C2535)로 드러나 기존 생성자에 합쳤다(`_volume` 기존 값 1.0f 유지).
- 첫 전체 검증에서 드문 실패(x64 Debug 경고창 1/96, Win32 Release `missing skill` 1/75)가 나왔다. 추적 결과 원인은 member-init 이 아니었다:
  ① 스모크가 사용자의 실제 마우스·키보드 입력을 받고 있었다 → [smoke-input-isolation](../smoke-input-isolation/intent.md)(PR #45~#47),
  ② 경고창은 각도 경계값에서 상태 키가 갱신되지 않아 텍스처 프레임 번호가 장 수를 넘던 기존 버그 → [frame-index-bounds](../frame-index-bounds/intent.md)(PR #48~#50).
  두 인텐트를 먼저 머지한 뒤 이 브랜치에 `master` 를 머지해 다시 검증했다.

## 검증 결과 (2026-11-12, `master` 49bd1d7 머지 후)

| spec | 결과 |
|---|---|
| R1·R2 누락 0 | ✅ 검사 스크립트 — 남은 8건은 오탐(툴 `CDevice` 를 Client 헤더로 대조한 5건, 스캐너가 형식을 못 읽는 `SoundDevice` 3건 — 둘 다 직접 확인) + 헤더 인라인 생성자 전수 확인 |
| R3·R4·R5 | ✅ 코드 검토 — 기본값 규칙, 한 줄에 하나·선언 순서(C5038 없음, 경고 0), `OBJINFO` 복사 생성자 전 멤버 |
| R6 `.dat` 호환 | ✅ 툴 스모크 4개 구성 `tiles=800`, Client 스모크의 `map tiles=800` |
| R7 | ✅ 클린 4개 구성 경고 0·오류 0. Client 스모크(입력 분리) x64 Debug 50/50, Win32 Release 50/50, Win32 Debug 5/5, x64 Release 5/5 — 변경 전(`master`) 같은 조건 50/50·50/50 과 동일. 일반 실행 4개 구성 × Client·툴 |
| R8 | ✅ CLAUDE.md 「코드 관례」 |
