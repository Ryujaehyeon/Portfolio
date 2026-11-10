# Review: 레거시 오딧 (2026-11-10)

기준 커밋: `9575c44` (master, VS2026 전환·UTF-8 BOM 통일 직후). 모든 경로는 `2D-Portfolio/` 기준.

## 방법

- **컴파일러**: v145 Debug|Win32 리빌드 경고를 중복 제거해 집계(283건).
- **기계적 조사**: vcxproj 미등록 파일, 자기 파일 밖 참조가 0인 클래스, `new`/`delete` 짝,
  문자열 `==` 비교, 주석 처리된 코드, `git ls-files` 로 커밋된 바이너리.
- **정독**: `ObjMgr`, `SceneMgr`, `ObjProto`, `Stage*::Progress`, `UIObj::VelueToPercentage`,
  `Macro.h`/`Function.h`/`Constant.h`/`Debug.hpp`, 메인 루프(`Client.cpp`), 입력 경로.
- **빌드 옵션 확인**: `Temp/.../CL.command.1.tlog` 로 실제 컴파일 플래그 확인.

## 이 repo 에서 실패란

게임(Client)이 크래시하거나 화면·판정이 잘못되는 것, 그리고 툴(MFC_000)이 만든 `.dat` 를 Client 가
잘못 읽는 것. 빌드 옵션이나 로케일 같은 환경이 바뀌었을 때 **조용히** 동작이 달라지는 것도 실패로 본다.

## 우선순위 기준

모든 항목은 기존 코드 문제(Pre-existing)이므로 REVIEW 템플릿의 심각도 대신 수정 우선순위를 쓴다.

- **P1 버그**: 크래시·미정의 동작·잘못된 결과. 지금 우연히 동작하더라도 포함.
- **P2 레거시 제거**: 쓰지 않는 코드·파일·바이너리. 동작 변화 없이 걷어낼 수 있음.
- **P3 품질**: 경고, 중복, 구식 API. 동작은 맞지만 유지보수·이식을 막음.

판정 표기: **확인** = 코드·빌드로 직접 확인, **추정** = 코드상 경로는 있으나 재현은 안 함.

---

## 요약

| # | 묶음 | 우선 | 규모 | 제안 슬러그 |
|---|---|---|---|---|
| 1 | 죽은 객체 nullptr 이 리스트에 남음 (구조적 크래시 지뢰) | P1 | ObjMgr 2함수 | `objmgr-lifetime` |
| 2 | 문자열을 포인터로 비교 (`==`) | P1 | 90줄 / 6파일 | `string-key-compare` |
| 3 | 반환값 누락·미초기화·잘못된 다운캐스트·순회 스킵·충돌 판정 누락 | P1 | 7곳 | `ub-fixes` |
| 4 | `ERR_MSG` 가 포맷 인자를 버림 | P1 | 84곳 (Stage.cpp) | `err-msg-format` |
| 5 | 쓰지 않는 클래스·파일 | P2 | 6클래스 357줄 | `dead-code-removal` |
| 6 | 주석 처리된 코드 | P2 | 약 185줄 | `dead-code-removal` |
| 7 | 커밋된 빌드 산출물·쓰지 않는 바이너리·데이터 | P2 | 15파일 | `repo-cleanup` |
| 8 | 소스 없는 `Debug.dll` 의존 | P2 | 2곳 | `debug-dll-removal` |
| 9 | Client·MFC_000 의 렌더링 코드 중복 | P3 | 5쌍 | `shared-render-lib` |
| 10 | 입력 처리 이중화 (DirectInput + `GetAsyncKeyState`) | P3 | 3파일 | `input-unify` |
| 11 | 컴파일러 경고 | P3 | 283건 | `warning-cleanup` |
| 12 | 리소스 경로가 작업 디렉터리 기준 | P3 | 전 리소스 | `resource-path` |
| 13 | `const TCHAR*` + ConformanceMode | P3 | 약 28곳 + 연쇄 | `const-tchar` |

---

## P1 버그

### 1. 죽은 객체가 nullptr 로 리스트에 남는다 — 확인(구조) · 크래시 재현은 안 함

- `Client/ObjMgr.cpp:604` `IterRelease()` 는 전체 맵을 돌며 죽은 몬스터·스킬을 `SAFE_DELETE` 하지만
  **리스트에서 지우지 않고 nullptr 를 남긴다.**
- `Client/ObjMgr.cpp:322` 이 함수가 `Progress()` 안에서 **객체 하나마다** 호출되고,
  `:324-329` 는 **현재 객체**가 nullptr 일 때만 `erase` 한다.
- 결과: 객체 A 를 처리하는 중에 리스트 뒤쪽 객체 B 가 삭제되면, 다음 순회에서 B 는 nullptr 인 채
  `:273` `(*iter2)->Progress()` 가 호출된다. `IterRelease` 자신도 다음 호출 때 `(*iter2)->GetObjType()` 로 역참조한다.
- 지금 순서에서는 삭제 조건(DEATH 모션·프레임 소진)이 대개 **자기 `Progress()` 안에서** 충족돼 자기 차례에 지워진다.
  즉 크래시는 "다른 객체가 B 의 상태를 바꾸는" 코드가 하나만 생겨도 터지는 **구조적 지뢰**이고, 재현은 하지 않았다.
- 원작자도 null 원소를 만났던 흔적: `:498` `ColCircle` 의 `if ((*iter) == nullptr || (*iter2) == nullptr) break;`,
  `ObjMgr.cpp:618` 주석 「0817 오전 몬스터가 제대로 삭제되지 않음」.
- 함께 볼 것: 다른 객체의 `m_pTagetObj`(대상)가 삭제된 몬스터를 계속 가리킬 수 있다(미확인).

**수정 방향**: 삭제 표시와 실제 삭제를 분리한다 — 순회 중에는 죽음만 표시하고, 프레임 끝에 한 번
`remove_if` + `delete` 로 정리. 대상 포인터는 정리 시점에 끊는다.

### 2. 문자열을 포인터로 비교한다 — 확인 (지금은 우연히 동작)

- `Headers/Constant.h:157-203` 이름 상수 38개가 `TCHAR* const PLAYER = L"Player";` 형태다.
- 이것이나 리터럴을 `GetName()`/`m_pObjKey`/`m_pStateKey`/`m_pMotion` 과 `==`/`!=` 로 비교한다 — **내용이 아니라 주소 비교.**
  - 90줄(주석 제외, 한 줄에 비교가 여럿인 경우 포함): `Monster.cpp` 32, `UIObj.cpp` 24, `Player.cpp` 20, `ObjMgr.cpp` 9,
    `MenuButton.cpp` 4, `BoneSpear.cpp` 1.
  - 예: `UIObj.cpp:507` `VelueToPercentage` 의 `L"HP" == VelueName`, `ObjMgr.cpp:294` 보스 판정 `GetName() == SUMMONER || ...`,
    `Player.cpp:139` `m_pMotion == RUN`, `MenuButton.cpp:48,56,88`, `BoneSpear.cpp:179`.
- 동작하는 이유: Debug `/ZI`, Release `/O2` 가 둘 다 **암묵적으로 `/GF`(동일 문자열 병합)** 를 켜서
  같은 리터럴이 한 주소로 합쳐진다(tlog 로 확인). `/Zi` 로 바꾸거나 `/GF-`, 다른 모듈(DLL)에서 온 문자열이면
  이 비교가 **조용히 전부 false** 가 된다.
- repo 전체에 `lstrcmp`/`wcscmp`/`_tcscmp` 사용이 0건이다.

**수정 방향**: 비교를 `lstrcmp(a, b) == 0` 또는 `wstring` 비교로 바꾼다. 상수는 `constexpr const wchar_t*` 로.
장기적으로는 상태·모션을 문자열 대신 `enum` 으로(문자열은 텍스처 키 조회에만).

### 3. 미정의 동작 묶음 — 확인

| 위치 | 문제 |
|---|---|
| `Client/Stage2.cpp:55-60` | `Progress()` 가 `SCENEID_END` 가 아니면 **값을 반환하지 않고 끝난다**(C4715). 반환값은 쓰레기. |
| `Client/UIObj.cpp:507-536` | `VelueToPercentage()` 가 어느 조건에도 안 걸리면 반환 없이 끝난다(C4715). 결과가 텍스처 높이 계산(`:330,336`)에 들어간다. |
| `Client/ObjMgr.cpp:246` | `SCENEID iScene;` 미초기화. 맵이 비면 쓰레기 값 반환. |
| `Client/ObjMgr.cpp:326-329` | `erase` 가 다음 원소를 돌려준 뒤 `continue` → for 의 `++iter2` 로 **원소 하나를 건너뛴다.** 그 객체는 그 프레임에 갱신되지 않는다. |
| `Client/ObjMgr.cpp:489-547` | `ColCircle` 이 안쪽(몬스터) 루프에서 바깥(스킬) 반복자를 `++iter` 하고 `iter2` 를 되감지 않는다 → 스킬마다 **몬스터 일부만** 충돌 검사, 바깥 `++iter` 와 겹쳐 스킬도 건너뛴다. 판정 누락. |
| `Client/ObjMgr.cpp:337,344` | `OBJ_UI` 객체(`CUIObj`, `CStageStatic` 계열)를 `static_cast<CPlayer*>`·`static_cast<CMonster*>` 로 내림. 상속 관계가 아니라 미정의 동작 — `Setlist` 가 가상 함수라 우연히 동작. |
| `Client/Stage*.cpp`, `Logo.cpp:69`, `MenuD2.cpp:81` | 장면의 `Progress()` 안에서 `CSceneMgr::InitScene` 을 불러 **실행 중인 자기 자신을 delete** 한다. 이후 멤버 접근이 없어 지금은 안전하지만, 한 줄만 추가해도 use-after-free. |

**수정 방향**: 반환 경로 보강, 초기화, `erase` 패턴 교정, 다운캐스트 제거(`CObj` 가상 함수로 호출).
장면 전환은 `CSceneMgr::Progress` 가 반환값을 보고 장면 밖에서 교체하도록 옮긴다.

### 4. `ERR_MSG` 가 포맷 인자를 버린다 — 확인

- `Headers/Macro.h:22` `#define ERR_MSG(hwnd, sz) MessageBox(hwnd, sz, L"system err", MB_OK)` — 인자 2개.
- `Client/Stage.cpp` 에서 `ERR_MSG(g_hWnd, L"%d Run L 로드 실패", Monster[i]);` 처럼 3개를 넘기는 곳이 84곳(C4002 84건).
  매크로가 세 번째 인자를 버려 **메시지박스에 `%d`·`%s` 가 그대로 찍힌다.** 어떤 리소스가 실패했는지 알 수 없다.
- 덤: 위 줄의 `%d` 에는 `TCHAR*`(`Monster[i]`)가 들어간다 — 포맷을 고치면 `%s` 로 바꿔야 한다.

**수정 방향**: 가변 인자 `ERR_MSG(hwnd, fmt, ...)` 로 바꾸고 내부에서 `swprintf_s` 후 `MessageBox`.

---

## P2 레거시 제거

### 5. 쓰지 않는 클래스·파일 — 확인

자기 `.h/.cpp` 밖 참조가 0 이다. `Headers/Include.h` 에서 include 만 된다.

| 클래스 | 파일 | 비고 |
|---|---|---|
| `CCollisionMgr` | `Client/CollisionMgr.*` | 같은 일을 `CObjMgr::ColCircle` 이 한다. `Include.h:21` |
| `CFullModeMgr` | `Client/FullModeMgr.*` | 자체 싱글턴. 전체화면은 `CDevice::InitDevice(WINMODE_*)` 가 담당. `Include.h:58` |
| `CNonPlayer` | `Client/NonPlayer.*` | 생성자만 있는 빈 클래스. `Include.h:40` |
| `CSkillTree` | `Client/SkillTree.*` | `CObj` 파생 빈 구현. 실제 스킬트리 UI 는 `CUIObj` + `struct SkillTree`. `Include.h:51` |
| `CMouse` | `Client/Mouse.*` | **vcxproj 에 없음** — 컴파일조차 안 되는 파일. 마우스는 `CObjMgr::MouseInfo` 가 담당. |

합계 357줄. `Client/KeyMgr.h:3-4` 의 주석 처리된 `SingletonTemplate` 흔적도 함께.

**도달 불가 장면** (2026-11-10 아키텍처 문서 작성 중 발견): `SCENEID_STAGE2`·`SCENEID_STAGE3` 를 반환하는 코드가 없다
(장면 전환은 `Ball.cpp:41` → MENU, `MenuButton.cpp:51` → STAGE, `:59` → END 뿐). `CSceneMgr::InitScene` 의 case 만 있고
`CStage2`·`CStage3` 는 실행되지 않는다. 둘 다 `Initialize()` 가 `return S_OK;` 뿐이고 `CSceneObj::CSceneObj`(`SceneObj.cpp:5`)도
`m_pPrototype` 을 초기화하지 않아, 도달하면 `AddObject(m_pPrototype, …)` 가 **쓰레기 포인터로 크래시**한다.
보스 소환 등 의도가 담겨 있으면 백로그 메모로 옮기고 제거.

**툴 미사용 페이지**: `MFC_000/Page4.*` — `CMySheet` 에 붙지 않는다(`MySheet.cpp:27-29` 는 Page3·1·2 만).

### 6. 주석 처리된 코드 — 확인

코드처럼 보이는 주석 줄이 약 185줄. 많은 곳: `ObjMgr.cpp`(`:257-271` 디버그 출력 15줄 등), `Stage2.cpp:39-52`(보스 소환),
`Monster.cpp`, `MainGame.cpp:15-26`(전체화면 분기), `Player.cpp:240-258`(DirectInput 키보드 이동), `Logo.cpp:63`.
git 이력에 남아 있으므로 지운다. 의도를 남길 가치가 있는 것(예: 보스 소환)은 백로그 메모로 옮긴다.

### 7. 커밋된 빌드 산출물·쓰지 않는 바이너리·데이터 — 확인

| 파일 | 판단 |
|---|---|
| `Release/Client.exe`, `Release/MFC_000.exe` | 2016년 빌드 산출물. 이제 `Exe/` 로 나온다 → 삭제 |
| `.vs/MyProgram/v14/.suo`, `MyProgram.suo` | VS 사용자 상태 → 삭제(이미 gitignore) |
| `Client/Client.vcxproj.user`, `MFC_000/MFC_000.vcxproj.user` | 사용자 설정 → 삭제(이미 gitignore) |
| `Client/Debug/Debug.dll` | `Client/Debug.dll` 의 중복 사본 → 삭제 |
| `Client/fmodex.dll`, `fmodex64.dll`, `fmodexL64.dll` | Win32 는 `fmodexL_vc.lib`→`fmodexL.dll` 만 쓴다(`GameSound.hpp:6-8`). x64 를 안 하면 삭제 |
| `Client/fmod/lib/fmodex64_vc.lib` | 위와 같음 |
| `Client/Monster.dat` (0 바이트), `Client/Monster1.dat` | 코드 참조 0건(`Map.dat` 만 읽음) → 툴 산출물 잔재, 삭제 |
| `Client/Rogue.mp3` | 코드 참조 0건. `Cave.mp3` 는 도달 불가한 `Stage2.cpp:9` 에서만 → 5번 처리 시 함께 |

`Resource/`(8220 파일, 76MB) 중 실제로 쓰는 파일은 **미조사** — 경로가 `%02d` 포맷으로 조립돼 정적 분석이 어렵다.
런타임 파일 접근 로그로 판정해야 한다(별도 작업).

### 8. 소스 없는 `Debug.dll` 의존 — 확인

- `Client/Client.cpp:8` `#pragma comment(lib, "Debug.lib")` 가 **Release 에도** 링크된다. Release 배포물에 디버그 DLL 이 따라간다.
- **배포 차단 요소**(2026-11-10 추가): `Debug.dll` 은 2016-07-21 VS2010 **디버그 CRT `MSVCR100D.dll`** 에 링크돼 있다(import 테이블 확인).
  디버그 CRT 는 재배포가 허용되지 않고 VS2010 이 없는 PC 엔 없다 → 그런 PC 에선 **Release 빌드조차 시작되지 않는다**
  (개발 PC 는 `System32`·`SysWOW64` 에 2011년 사본이 있어 드러나지 않음). 또한 PE32(x86) 라 x64 링크 불가.
- 정체: export 는 `DebugMsgw/a`, `CreateConsole`, `DestroyConsole` + `DebugClass`(콘솔 생성·`WriteConsoleW`·파일 쓰기).
  즉 `AllocConsole` 로 띄운 디버그 콘솔에 문자열을 찍는 로깅 헬퍼다.
- 쓰는 곳은 `Client/MenuD2.cpp:84` `DebugMsg(Info)` 한 곳 — 메뉴 장면에서 **매 프레임** `"Scene : %d"` 를 보낸다(`Headers/Debug.h` 의 `DebugMsgw`).
  `CreateConsole` 은 호출처 0 이라 콘솔이 없는 상태로 호출된다.
- 같은 역할을 헤더 전용 `Headers/Debug.hpp` 의 `DebugLog` 가 이미 한다(`Player.cpp:97-98`).
- `Headers/Debug.h:12` 의 `EXPORT` 매크로가 MFC 쪽 정의와 충돌한다(C4005).

**수정 방향**: `DebugMsg` 1곳을 `DebugLog`(또는 `OutputDebugString`)로 바꾸고 `Debug.h`·`Debug.lib`·`Debug.dll` 제거.

---

## P3 품질

### 9. 렌더링 코드 중복 — 확인

같은 이름의 5쌍이 Client·MFC_000 에 각각 있고 내용이 갈라졌다.

| 파일 | Client / MFC 줄 | 다른 줄 |
|---|---|---|
| `Device.cpp` | 128 / 94 | 49 |
| `TextureMgr.cpp` | 64 / 59 | 47 |
| `MultiTexture.cpp` | 74 / 55 | 39 |
| `SingleTexture.cpp` | 47 / 43 | 10 |
| `Texture.cpp` | 12 / 12 | 0 |

헤더 5쌍도 6~14줄씩 다르다. DirectX 버전을 올릴 때 두 번 옮겨야 한다 → **DX 전환 전에** 공용 정적 라이브러리
(`Engine/` 등)로 합치는 것을 권장. 차이(Client 의 DirectInput 등)는 합칠 때 정리.

### 10. 입력 처리 이중화 — 확인

- 키보드: `Client/KeyMgr.cpp` 가 `GetAsyncKeyState`(20곳), `MenuButton.cpp` 2곳.
- 마우스 버튼: `Client/Player.cpp:267,293,307` 가 DirectInput(`CDevice::GetMouseState`).
- DirectInput 키보드 경로는 주석으로 남아 있다(`Player.cpp:248-258`). `Client/Device.cpp:74` 에서 키보드·마우스 장치를 모두 만든다.
- DirectInput 은 키보드·마우스 용도로 MS 가 권장하지 않는다. 한쪽(윈도 메시지 또는 `GetAsyncKeyState` 래핑)으로 통일.

### 11. 컴파일러 경고 283건 — 확인

| 코드 | 건수 | 내용 |
|---|---|---|
| C4244 | 177 | `Stage.cpp` 122, `UIObj.cpp` 18, `Monster.cpp` 10, `MFC_000View.cpp` 10 등. Stage.cpp 는 프레임 수 상수가 `const float`(`Constant.h:150-154` 등)인데 `int` 인자로 넘김 |
| C4002 | 84 | 4번 항목(`ERR_MSG`) |
| C4838 | 18 | `UIObj.cpp` 16(`RECT` 초기화에 float), `FullModeMgr.cpp:61` 2(죽은 코드라 5번에서 사라짐) |
| C4715 | 2 | 3번 항목 |
| C4018 | 1 | `MFC_000/MFC_000View.cpp:502` signed/unsigned |
| C4005 | 1 | 8번 항목 |

파일별로는 `Stage.cpp` 206, `UIObj.cpp` 35, `MFC_000View.cpp` 11, `Monster.cpp` 10. P1·P2 를 처리하면 약 90건(C4002 84, C4715 2, C4005 1, C4838 2)이 함께 사라진다.

### 12. 리소스 경로가 작업 디렉터리 기준 — 확인

`L"../Resource/..."`, `L"Map.dat"`, `L"Tristram.mp3"` 등 → exe 를 더블클릭하면 리소스를 못 찾는다(VS 디버거에서만 정상).
exe 위치 기준 경로 해석 함수 하나로 모으고, 리소스·데이터를 Post-Build 로 배포 폴더에 배치할지 결정 필요.

### 13. `const TCHAR*` + ConformanceMode — 확인

앞선 논의 그대로. 헤더의 non-const `TCHAR*` 약 28곳. 2번(문자열 비교)과 같은 코드를 건드리므로 **2번과 묶거나 바로 뒤에** 한다.

---

## 제안 순서

검증 수단: 빌드(경고 수 증감 포함) + 실행 스모크. P2 순수 삭제는 `/Brepro` 해시 비교가 가능한 경우 사용.

1. `repo-cleanup` (7) — 코드 무관, 위험 0. 저장소부터 가볍게.
2. `dead-code-removal` (5, 6) — 동작 불변. 이후 작업 대상 코드량이 줄어든다.
3. `debug-dll-removal` (8)
   → `x64-migration` — 2026-11-10 추가. 32비트 전용 `Debug.lib` 가 유일한 차단 요소라 **반드시 8번 뒤**.
     이후 수정들을 x64 에서 검증하도록 버그 수정보다 앞에 둔다. [x64-migration](../x64-migration/plan.md)
4. `err-msg-format` (4) — 작고 독립적, 경고 84건 감소.
5. `ub-fixes` (3)
6. `objmgr-lifetime` (1) — 가장 위험한 버그. 3번의 `erase` 교정과 맞물린다.
7. `string-key-compare` + `const-tchar` (2, 13)
8. `warning-cleanup` (11 잔여)
9. `shared-render-lib` (9) → `input-unify` (10) → `resource-path` (12)
10. DirectX 전환 (별도 인텐트, 9번 이후)

## 미조사 범위

- `Resource/` 실사용 여부(7번 참고).
- `CAStar`, 몬스터 AI(`Monster.cpp`), 스킬 판정의 로직 정확성 — 읽지 않았다.
- MFC_000 툴 로직과 `.dat` 포맷 호환성(`Headers/Struct.h` 구조체 ↔ `Map.dat`).
- 1번 크래시 경로의 실제 재현.
