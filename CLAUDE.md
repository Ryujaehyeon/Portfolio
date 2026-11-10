@AGENTS.md

# 2D-Portfolio

DirectX 9 기반 2D 아이소메트릭 액션 RPG 포트폴리오(2016). 소스는 모두 `2D-Portfolio/` 아래에 있다.

| 프로젝트 | 종류 | 역할 |
|---|---|---|
| `Client/` | Win32 Application | 게임 본체. Logo → Menu → Stage1~3 장면 |
| `MFC_000/` | MFC(Dynamic) Application | 맵·몬스터 배치 툴. `*.dat` 파일로 저장 → Client 가 읽음(`Map.dat`, `Monster.dat`) |
| `Headers/` | 공유 헤더(솔루션 폴더) | `Constant.h`·`Struct.h`·`Macro.h`·`Function.h`·`Defines.h`·`Include.h` |

## Claude Code 전용

- Worker는 부모(Advisor) 모델을 상속한다(위임 시 model 인자를 생략한다). 사용자가 모델을 지정해 주는 경우에는 해당 모델을 Worker의 모델로 설정한다.
- 위임은 넓은 다중 파일 조사처럼 서로 독립적이고 규모가 큰 작업에만 한다. 한 Worker로 끝나는 일에
  여러 명을 띄우지 않고, 자기 작업을 검증·재확인할 목적으로는 Worker를 쓰지 않는다. 단 사용자가 직접
  요청하거나 실행한 리뷰·워크플로의 검증 단계는 예외다.

## 빌드 / 테스트 / 린트

솔루션 `2D-Portfolio/MyProgram.sln` — VS2010 형식(ToolsVersion 4.0), 구성 `Debug|Win32`·`Release|Win32` 만 있다.

**현재 이 PC에서는 빌드되지 않는다** (2026-11-10 확인):
- v100(VS2010) 툴셋이 없다 — 설치된 건 VS 18 Insiders(v145)·VS2022(v143).
- DirectX SDK (June 2010) 가 없다 — `d3dx9.h`/`d3dx9.lib` 는 Windows SDK 에 포함되지 않는다.
  v145 로 시험 빌드 시 `Client\stdafx.h(27,10): error C1083: ... 'd3dx9.h'` 에서 멈춘다.

빌드를 살리려면 (1) DXSDK June 2010 설치(`DXSDK_DIR` 환경변수) + Include/Lib 경로 추가,
(2) 툴셋 지정이 필요하다. 이건 코드 변경이 아닌 환경 결정이므로 사용자에게 먼저 묻는다.

```
빌드(WSL에서, 툴셋 오버라이드·산출물은 repo 밖으로):
  "/mnt/c/Program Files/Microsoft Visual Studio/18/Insiders/MSBuild/Current/Bin/MSBuild.exe" \
    2D-Portfolio/MyProgram.sln /p:Configuration=Debug /p:Platform=Win32 /p:PlatformToolset=v145 \
    "/p:OutDir=C:\Users\Public\pf-build\\" /m /nologo /v:m
  정상 출력: 오류 0개, 종료 코드 0   (현재는 d3dx9.h 누락으로 실패)
테스트: 없음 (자동화 테스트 없음 — 실행해서 눈으로 확인)
린트:   없음 (컴파일러 경고 /W3 이 유일한 정적 검사)
```

빌드 산출물을 repo 안에 만들지 않는다 — `.gitignore` 가 VS 산출물을 제대로 막지 않는다.

## 코드 관례

- **인코딩: 소스는 CP949(EUC-KR)**, BOM 없음. 한글 주석이 있으므로 UTF-8 로 변환해 저장하지 않는다.
  읽을 때는 `iconv -f cp949 -t utf-8`, 수정 시에도 CP949 를 유지한다. 줄바꿈은 `.gitattributes`(`* text=auto`)에 맡긴다.
- 네이밍: 클래스 `C` 접두(`CPlayer`, `CSceneMgr`), 멤버 `m_` + 헝가리안(`m_pObjKey`, `m_fAngle`, `m_bRun`, `m_iDegree`),
  인자 `_` 접두(`_ObjName`), 전역 `g_`(`g_hWnd`, `CObj::g_tScroll`), 상수 대문자(`WINSIZEX`), enum 값 `타입_값`(`SCENEID_LOGO`, `WINMODE_WIN`).
- 문자열: `TCHAR*`/`wstring`, 리터럴은 `L"..."` (CharacterSet=Unicode).
- 파일 구조: 클래스 1개 = `.h`/`.cpp` 1쌍, 프로젝트 폴더에 평평하게 둔다. 모든 `.cpp` 첫 줄은 `#include "StdAfx.h"`(PCH).
- 공용 헤더는 `Headers/Include.h` 하나로 모은다. Client 전용 헤더는 `#ifdef __CLIENT__EXEC__` 블록 안에 추가한다
  (`Client/stdafx.h` 에서 정의).
- `using namespace std;` 가 `stdafx.h` 에 있다 — 기존 코드는 `std::` 없이 쓴다.
- 포매터: 없음. 탭 들여쓰기, 중괄호는 다음 줄(Allman). 주변 코드 스타일을 따른다.
- 주석·문서 언어: 한국어, 식별자는 원문.

## 커밋 규약

- `type(scope): 제목` — Conventional Commits. 제목은 한국어·명령형·~50자, 식별자는 원문.
- type: `feat` `fix` `docs` `refactor` `style` `test` `chore`.
- scope: SDLC 변경이면 **기능 슬러그**(`intent/<슬러그>/` 와 동일 문자열),
  아니면 코드 영역 `client` · `tool`(MFC_000) · `headers` · `resource` · `build`.
- 본문은 필요할 때만 — **무엇을 / 왜**.
- 슬러그를 scope 로 쓰면 `git log --grep <슬러그>` 로 그 변경의 intent→spec→plan→코드가 묶여 나온다.

## 아키텍처 패턴

- **게임 루프**: `CMainGame::Initialize/Progress/Render/Release`. `Progress()` 가 `SCENEID` 를 반환해 장면 전환을 알린다.
  객체·장면 모두 같은 수명주기(`Initialize` → `Progress` → `Render` → `Release`)를 따른다.
- **매니저 = 싱글톤**: `SINGLETON(type)` 매크로(`Headers/Macro.h`) + `GET_SINGLE(type)->...` 로 접근,
  `DESTROY_SINGLE` 로 해제. `CDevice` `CTextureMgr` `CSceneMgr` `CObjMgr` `CTimeMgr` `CKeyMgr` `CCollisionMgr` `CAStar` `CSortMgr` 등.
- **장면**: `CSceneObj` 파생 `CLogo` `CMenuD2` `CStage` `CStage2` `CStage3`, `CSceneMgr::InitScene(SCENEID)` 로 교체.
- **오브젝트 계층**(장면 계층과 별개): `CObj` → 장면별 `C{Logo,Menu,Stage}{Static,Dynamic}` →
  `CStageDynamic` → `CPlayer` `CMonster` `CNonPlayer`, `CStageStatic` → `CStageBackGround` `CUIObj`.
  스킬은 `CObj` → `CSkill` → `CBoneSpear` `CFireWall` `CBlizzard`, 스킬트리는 `CSkillTree`.
- **프로토타입 패턴**: `CPrototype` → `CObjProto`(`map<wstring, CObj*>`) → `CLogoObjProto`/`CMenuObjProto`/`CStageObjProto`/`CSkillProto`.
  새 객체는 원본을 `Clone()` 해서 만든다.
- **텍스처**: `CTexture` → `CSingleTexture`/`CMultiTexture`, `CTextureMgr` 가 키(`ObjKey`/`StateKey`/`Motion`)로 관리.
- **사운드**: FMOD Ex(`Client/fmod/`, `GameSound.hpp`).
- **중복 코드**: `Device`·`Texture`·`TextureMgr`·`SingleTexture`·`MultiTexture` 가 Client 와 MFC_000 에 각각 사본으로 있고
  내용이 서로 다르다. 한쪽을 고칠 때 다른 쪽도 고쳐야 하는지 확인한다(자동으로 동기화하지 않는다).

### 건드리면 안 되는 부분

- `2D-Portfolio/Resource/` (약 76MB 텍스처·리소스) — 코드 작업에서 수정·재배치하지 않는다.
- `Client/*.dat` (`Map.dat`, `Monster.dat`, `Monster1.dat`) — MFC_000 툴이 만든 바이너리 데이터. 직접 편집하지 않는다.
  구조체(`Headers/Struct.h`)를 바꾸면 기존 `.dat` 와 호환이 깨진다.
- `Client/fmod/`, 커밋돼 있는 `*.dll`·`*.lib` — 서드파티/빌드 산출물. 교체는 사용자 결정.

## Claude 가 반복하는 실수

<!-- 같은 실수가 2회 나오면 여기에 한 줄 추가한다. 예방책을 함께 적는다. -->

- (아직 없음)

## SDLC

- 변경 1개 = `intent/<기능-슬러그>/` 폴더 1개. 그 안에 `intent.md` → `spec.md` → `plan.md`.
- 폴더 규칙·SSOT 표는 `intent/README.md`.
- 단계별 운영 지침은 `.harness/sdlc/`.
