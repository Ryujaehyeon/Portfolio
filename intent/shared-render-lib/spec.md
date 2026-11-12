# Spec: shared-render-lib

Author: Ryu Jahyeon
Status: draft

<!-- 입력: intent.md. 경로는 2D-Portfolio/ 기준. -->

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | `Engine/Engine.vcxproj`(정적 라이브러리, Win32·x64 × Debug·Release)가 솔루션에 있고, Client·MFC_000 이 프로젝트 참조로 링크한다. 설정: v145, Unicode, PCH(`Engine/stdafx.h`), `/W3`·`/WX`·`/permissive-`, Release `/GF-`, 포함 경로 `../Headers`, NuGet D3DX | vcxproj 검토 + 빌드 |
| R2 | `CDevice`·`CTexture`·`CTextureMgr`·`CSingleTexture`·`CMultiTexture` 가 `Engine/` 에만 있다(Client·MFC_000 사본 삭제) | 파일 목록 |
| R3 | API: `CDevice::InitDevice(HWND, WINMODE)`, `Render_Begin(D3DCOLOR)`, `CTextureMgr::InsertTexture(파일, ObjKey, TEXTYPE, StateKey = 0, 장 수 = 1, 컬러키 = 검정)`, `GetTexture(ObjKey, StateKey = 0, 번호 = 0)` | 코드 검토 |
| R4 | DirectInput 은 Client 의 `CInputDevice`(싱글턴)에 — `Initialize(HINSTANCE, HWND)`·`SetKey`·`GetKeyBoardState`·`GetMouseState`. 스모크 분기(입력 무시) 유지. `CMainGame::Initialize` 가 장치 다음에 초기화, 종료 때 해제 | 코드 검토 |
| R5 | 동작 불변: Client 는 창 모드·`D3DRS_ZENABLE` TRUE·지우는 색 검정·컬러키 검정, 툴은 창 모드·지우는 색 파랑·컬러키 흰색 | 코드 검토 + 스모크·일반 실행 |
| R6 | 4개 구성 클린 빌드 경고 0·오류 0. Client 스모크 x64 Debug·Win32 Release 각 20회·나머지 5회, 툴 스모크 4개 구성, 일반 실행 | CLAUDE.md |
| R7 | CLAUDE.md(프로젝트 표·「중복 코드」·빌드)와 `docs/ARCHITECTURE.md`(빌드 구성·클래스·툴)가 새 구조와 맞는다 | 문서 검토 |

## 설계

- **바탕은 Client 사본**(두 단계 키·`wstring` 키·해제가 올바름). 툴과 다른 동작은 인자로 받는다 — 앱마다 다른 값은 호출하는 앱이 정한다(공용 코드는 앱을 모른다, DIP).
  지우는 색은 `Render_Begin(D3DCOLOR)`, 컬러키는 `InsertTexture` 의 마지막 인자(기본 검정), 깊이 렌더 상태는 Client 가 `InitDevice` 다음에 직접 켠다.
- **`Engine/stdafx.h`** 는 그리기에 필요한 것만(`windows.h`·`tchar.h`·STL·`d3d9`·`d3dx9`, `using namespace std`). `Headers/Defines.h` 의 `#include "stdafx.h"` 는 포함한 쪽 프로젝트의 것을 찾으므로 그대로 둔다.
- **툴의 텍스처 키** — 한 단계였던 것을 상태 키에 오브젝트 키와 같은 문자열을 넣어 두 단계로 쓴다(`InsertTexture(…, L"Tile", TEXTYPE_MULTI, L"Tile", n, 흰색)`, `GetTexture(L"Tile", L"Tile", i)`).
- **`CInputDevice`** — `CDevice` 의 DirectInput 멤버·초기화·해제·`SetKey` 를 그대로 옮긴다. 사용처 5곳(`(*CDevice::GetInstance())->SetKey()` 등)을 `CInputDevice` 로.
- **`Include.h`** — 공용 부분의 5개 헤더는 그대로 두고(포함 경로에 `../Engine`), Client 블록에 `InputDevice.h` 를 더한다.

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| MFC 앱이 비 MFC 정적 라이브러리를 링크 | 빌드 | 런타임 라이브러리가 같으면(기본 `/MD` 계열) 문제없다. 빌드·툴 실행으로 확인 |
| 툴의 그리기 결과(타일·몬스터·미니맵)는 스모크가 보지 않음 | 검증 범위 | 툴 일반 실행으로 창이 뜨는지만 확인, 그리기는 눈 확인 미실시로 기록 |
| 툴 해제 누수 수정은 동작 변화 | 동작 불변 | 종료 때만 영향 — 의도한 수정 |
