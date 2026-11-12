@AGENTS.md

# 2D-Portfolio

DirectX 9 기반 2D 아이소메트릭 액션 RPG 포트폴리오(2016). 소스는 모두 `2D-Portfolio/` 아래에 있다.

| 프로젝트 | 종류 | 역할 |
|---|---|---|
| `Client/` | Win32 Application | 게임 본체. Logo → Menu → Stage 장면 |
| `MFC_000/` | MFC(Dynamic) Application | 맵·몬스터 배치 툴. `*.dat` 파일로 저장 → Client 가 읽음(`Map.dat`, `Monster.dat`) |
| `Headers/` | 공유 헤더(솔루션 폴더) | `Constant.h`·`Struct.h`·`Macro.h`·`Function.h`·`Defines.h`·`Include.h` |

## Claude Code 전용

- Worker는 부모(Advisor) 모델을 상속한다(위임 시 model 인자를 생략한다). 사용자가 모델을 지정해 주는 경우에는 해당 모델을 Worker의 모델로 설정한다.
- 위임은 넓은 다중 파일 조사처럼 서로 독립적이고 규모가 큰 작업에만 한다. 한 Worker로 끝나는 일에
  여러 명을 띄우지 않고, 자기 작업을 검증·재확인할 목적으로는 Worker를 쓰지 않는다. 단 사용자가 직접
  요청하거나 실행한 리뷰·워크플로의 검증 단계는 예외다.

## 빌드 / 테스트 / 린트

솔루션 `2D-Portfolio/MyProgram.sln` — VS2026(v145), 구성 `Debug`·`Release` × 플랫폼 `Win32`·`x64` 4개.
경위는 `intent/vs2026-migration/`·`intent/x64-migration/`. Win32 는 비교 검증용으로 당분간 유지(제거 여부는 별도 결정).

- D3DX 는 DirectX SDK 대신 NuGet `Microsoft.DXSDK.D3DX`(packages.config)로 받는다. `d3dx9.lib` 를 vcxproj 에
  직접 적지 않는다 — 패키지가 구성별로 `d3dx9.lib`/`d3dx9d.lib` 를 넣고 DLL 도 복사한다.
- 출력·중간 경로는 `2D-Portfolio/Directory.Build.props` 한 곳에서 정한다(vcxproj 에 OutDir/IntDir 를 쓰지 않는다).
  산출물 `Exe\<플랫폼>\<구성>\<프로젝트>\`, 중간 `Temp\<플랫폼>\<프로젝트>\<구성>\` — 둘 다 gitignore.
- Client 런타임 DLL 은 Post-Build 가 플랫폼에 맞는 것 하나만 OutDir 로 복사한다(`Client.vcxproj` 의 `$(FmodDll)`):
  Win32 → `fmodexL.dll`(로깅판, 실행하면 작업 디렉터리에 `fmod.log` 를 남김 — gitignore), x64 → `fmodex64.dll`(비로깅판).
- `ConformanceMode`(`/permissive-`)가 두 프로젝트·4개 구성에 켜져 있다 — 리터럴을 non-const `TCHAR*` 에 담거나 임시 값의 주소(`&RealRect()`, `&D3DXVECTOR3(…)`)를
  넘기면 오류다. 임시 값은 지역 변수로 받는다. 경위는 `intent/const-tchar/`.

```
복원:   MSBuild.exe 2D-Portfolio/MyProgram.sln -t:restore -p:RestorePackagesConfig=true -nologo -v:m
        정상 출력: 패키지 추가 메시지 또는 "복원할 항목 없음", 종료 코드 0
빌드:   MSBuild.exe 2D-Portfolio/MyProgram.sln -p:Configuration=Debug -p:Platform=Win32 -m -nologo -v:m
        x64: -p:Platform=x64 (산출물 Exe\x64\<구성>\..., exe 는 PE32+)
        (WSL: "/mnt/c/Program Files/Microsoft Visual Studio/18/Insiders/MSBuild/Current/Bin/MSBuild.exe")
        정상 출력: "Client.vcxproj -> ...\Exe\<플랫폼>\Debug\Client\Client.exe", 오류 0, 경고 0, 종료 코드 0
        경고는 0 이고 두 프로젝트·4개 구성에 경고를 오류로(`/WX`, TreatWarningAsError) 둔다 — 경고 하나가 빌드를 실패시킨다.
        경고가 나면 끄지 말고(`#pragma warning(disable)`·`/wd` 금지) 코드를 고친다. 축소 변환은 같은 결과의 `static_cast` 로 명시한다.
        경위는 `intent/warning-cleanup/`. 컴파일러가 업데이트돼 새 경고가 생기면 그때 고친다.
실행:   작업 디렉터리를 `2D-Portfolio/Client/`(툴은 `MFC_000/`)로 두고 exe 실행 — 리소스가 상대 경로.
        WSL 에서 「살아 있는지」만 볼 때: `timeout 8 ….exe` 는 WSL 쪽만 끝내고 **Windows 프로세스(창)는 남는다** —
        남은 exe 때문에 다음 빌드가 LNK1168(쓰기용으로 열 수 없음)로 실패한다. PowerShell 로 띄우고 직접 끝낸다:
          powershell.exe -NoProfile -Command "$p=Start-Process -FilePath '<exe 의 Windows 경로>' -WorkingDirectory '<Client 폴더>' -PassThru; Start-Sleep 8; $p.HasExited; Stop-Process -Id $p.Id"
        단, 오류 메시지박스("system err")가 떠도 살아 있으므로 창 제목까지 확인한다. 자동 판정이 필요하면 아래 스모크 모드를 쓴다.
        일반 실행은 게임 창이 비활성이어도 전역 입력(다른 창에서의 클릭·키)을 받는다 — 띄워 둔 채 다른 일을 하면 게임이 움직인다.
테스트: 스모크 모드 (경위 `intent/smoke-log/`) — 입력 없이 로고→메뉴→스테이지→스킬 시전→몬스터 하나 죽여 정리까지 진행하고 스스로 종료한다.
        cd 2D-Portfolio/Client && timeout 60 ../Exe/<플랫폼>/<구성>/Client/Client.exe -smoke; echo $?   → 0 (약 9초)
        cd 2D-Portfolio/MFC_000 && ../Exe/<플랫폼>/<구성>/MFC_000/MFC_000.exe -smoke ../Client/Map.dat; echo $?   → 0
        결과는 작업 디렉터리의 smoke.log(UTF-8, gitignore): 줄마다 PASS/FAIL/INFO, 마지막 줄 "RESULT PASS (fail=0)".
        실패 시 종료 코드 1, FAIL 줄에 원인(ERR_MSG·FMOD 오류는 메시지박스 대신 여기). -smoke 없이 실행하면 아무것도 남기지 않는다.
        스모크는 실제 마우스·키보드를 받지 않는다(키·버튼은 안 눌림, 커서는 창 가운데 고정 — `GameAsyncKeyState`·`GameCursorPos`·`CDevice::SetKey`).
        검증 중에도 PC 를 써도 된다. 입력 읽기를 새로 추가할 때는 `GetAsyncKeyState`·`GetCursorPos` 를 직접 부르지 말고 이 함수를 쓴다(경위 `intent/smoke-input-isolation/`).
        소리가 실제로 들리는지·화면이 제대로 그려지는지는 로그로 판정하지 못한다 — 필요할 때만 눈으로 확인.
린트:   없음 (컴파일러 경고 /W3 이 유일한 정적 검사)
```

## 코드 관례

- **인코딩: C/C++ 소스는 UTF-8 BOM**(`.editorconfig`). 새 파일도 BOM 을 붙인다 — BOM 이 없으면 MSVC 가
  시스템 코드 페이지(한국어 Windows 는 CP949)로 읽어 한글 리터럴이 깨진다. `/utf-8` 옵션은 쓰지 않는다.
  예외: `.rc`·`.rc2`·`MFC_000/resource.h` 는 UTF-16 LE, `MFC_000/MFC_000.reg` 는 CP949(regedit 호환) — 그대로 둔다.
  경위는 `intent/source-utf8/`. 줄바꿈은 `.gitattributes`(`* text=auto`)에 맡긴다.
- 네이밍: 클래스 `C` 접두(`CPlayer`, `CSceneMgr`), 멤버 `m_` + 헝가리안(`m_pObjKey`, `m_fAngle`, `m_bRun`, `m_iDegree`),
  인자 `_` 접두(`_ObjName`), 전역 `g_`(`g_hWnd`, `CObj::g_tScroll`), 상수 대문자(`WINSIZEX`), enum 값 `타입_값`(`SCENEID_LOGO`, `WINMODE_WIN`).
- 문자열: `TCHAR*`/`wstring`, 리터럴은 `L"..."` (CharacterSet=Unicode). 읽기만 하는 문자열 포인터는 `const TCHAR*`(이름 상수는 `const TCHAR* const`).
  이름·키·모션(`TCHAR*`) 비교는 **`IsSameKey(a, b)`**(`Headers/Function.h`, 내용 비교) — `==`/`!=` 는 주소 비교라 쓰지 않는다. `TCHAR*` 를 `map` 키로 쓰지 않는다(`wstring` 키).
  Client Release 는 문자열 병합을 끈다(`StringPooling=false`, `/GF-`) — 주소 비교가 다시 생기면 Release 스모크가 실패한다. 경위는 `intent/string-key-compare/`.
- 파일 구조: 클래스 1개 = `.h`/`.cpp` 1쌍, 프로젝트 폴더에 평평하게 둔다. 모든 `.cpp` 첫 줄은 `#include "StdAfx.h"`(PCH).
- 공용 헤더는 `Headers/Include.h` 하나로 모은다. Client 전용 헤더는 `#ifdef __CLIENT__EXEC__` 블록 안에 추가한다
  (`Client/stdafx.h` 에서 정의).
- `using namespace std;` 가 `stdafx.h` 에 있다 — 기존 코드는 `std::` 없이 쓴다.
- 포매터: 없음. 탭 들여쓰기, 중괄호는 다음 줄(Allman). 주변 코드 스타일을 따른다.
- 주석·문서 언어: 한국어, 식별자는 원문.
- 디버그 출력은 `Headers/Debug.hpp` 만 쓴다: `DebugLog(L"fmt", ...)`(콘솔), `DEBUG_ENTRY_EXIT`·`DEBUG_LINE`(VS 출력 창).
  `_DEBUG` 밖에서는 빈 매크로라 Release 에 남지 않는다 — 포맷 조립·임시 버퍼도 매크로 인자 안에 둔다. 별도 디버그 DLL 을 만들지 않는다.

### C++ 코딩 표준

- 표준: [.harness/standards/cpp11-coding-standard.md](.harness/standards/cpp11-coding-standard.md) (항목 번호 `I-56` = I장 56번, 분류는 `coding-standard` 스킬 참고)
- 언어 표준: C++14 (MSVC 기본값, `/std` 미지정)
- 검사 제외 경로: `2D-Portfolio/Client/fmod/`(FMOD Ex 서드파티), `2D-Portfolio/packages/`(NuGet), `2D-Portfolio/Client/Resource.h`·`2D-Portfolio/MFC_000/resource.h`(리소스 편집기 생성 파일)

| 대상 | 적용 |
|---|---|
| 기존 코드의 **명명·구조 관례**(위 목록: `C` 접두사, `m_`+헝가리안, `SCENEID_*` enum, `SAFE_DELETE` 등) | **기존 관례 유지** — 표준의 I-1·I-10·I-11·I-16·I-40 등과 달라도 일괄 개명·재구조화하지 않는다 |
| 인텐트에서 **고치는 코드** | 표준의 품질 규칙 적용: 경고 0(I-56), `const`(I-26·27), `nullptr`(III-4), `override`(III-1), `constexpr`(III-10), 한 줄 블록도 중괄호(II-4), `default:`(I-23), 변수 가리기 금지(I-57) |
| **새로 만드는** 클래스·파일 | 위 품질 규칙 + 이름은 주변 기존 관례에 맞춘다(한 파일 안에서 두 스타일을 섞지 않는다) |
| 표준이 「미정」으로 둔 항목(I-62, III-11, III-12) | 규칙으로 해석하지 않는다 |

충돌하면 이 CLAUDE.md → 표준 순으로 따른다(표준 문서도 「프로젝트 규칙 우선」을 명시).

## 커밋 규약

- `type(scope): 제목` — Conventional Commits. 제목은 한국어·명령형·~50자, 식별자는 원문.
- type: `feat` `fix` `docs` `refactor` `style` `test` `chore`.
- scope: SDLC 변경이면 **기능 슬러그**(`intent/<슬러그>/` 와 동일 문자열),
  아니면 코드 영역 `client` · `tool`(MFC_000) · `headers` · `resource` · `build`.
- 본문은 필요할 때만 — **무엇을 / 왜**.
- 슬러그를 scope 로 쓰면 `git log --grep <슬러그>` 로 그 변경의 intent→spec→plan→코드가 묶여 나온다.

## 아키텍처 패턴

구조·흐름·다이어그램 전체는 [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md). 아래는 작업 시 필요한 요약.


- **게임 루프**: `CMainGame::Initialize/Progress/Render/Release`. `Progress()` 가 `SCENEID` 를 반환해 장면 전환을 알린다.
  객체·장면 모두 같은 수명주기(`Initialize` → `Progress` → `Render` → `Release`)를 따른다.
- **매니저 = 싱글톤**: `SINGLETON(type)` 매크로(`Headers/Macro.h`) + `GET_SINGLE(type)->...` 로 접근,
  `DESTROY_SINGLE` 로 해제. `CDevice` `CTextureMgr` `CSceneMgr` `CObjMgr` `CTimeMgr` `CKeyMgr` `CAStar` `CSortMgr` 등.
- **장면**: `CSceneObj` 파생 `CLogo` `CMenuD2` `CStage`, `CSceneMgr::InitScene(SCENEID)` 로 교체.
- **오브젝트 계층**(장면 계층과 별개): `CObj` → 장면별 `C{Logo,Menu,Stage}{Static,Dynamic}` →
  `CStageDynamic` → `CPlayer` `CMonster`, `CStageStatic` → `CStageBackGround` `CUIObj`.
  스킬은 `CObj` → `CSkill` → `CBoneSpear` `CFireWall` `CBlizzard`. 스킬트리는 객체가 아니라 `CUIObj` + `struct SkillTree`.
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
- 커밋된 바이너리는 이것뿐이다 — 서드파티라 소스가 없다. 교체·삭제는 사용자 결정.
  - FMOD Ex(단종, 재배포처 없음): `Client/fmodexL.dll`+`fmod/lib/fmodexL_vc.lib`(Win32), `Client/fmodex64.dll`+`fmod/lib/fmodex64_vc.lib`(x64), `fmod/inc/`
  - 지운 바이너리는 이력에서 복원: `git show <삭제 커밋>^:<경로> > <파일>` (repo-cleanup 참고)

## Claude 가 반복하는 실수

<!-- 같은 실수가 2회 나오면 여기에 한 줄 추가한다. 예방책을 함께 적는다. -->

- (아직 없음)

## SDLC

- 변경 1개 = `intent/<기능-슬러그>/` 폴더 1개. 그 안에 `intent.md` → `spec.md` → `plan.md`.
- 폴더 규칙·SSOT 표는 `intent/README.md`.
- 단계별 운영 지침은 `.harness/sdlc/`.
- 직접 수정할 때의 순서·명령·확인 체크리스트는 [docs/FIX-GUIDE.md](docs/FIX-GUIDE.md).
