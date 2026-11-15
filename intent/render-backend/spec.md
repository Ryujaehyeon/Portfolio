# Spec: render-backend

Author: Ryu Jahyeon
Status: draft

<!-- 입력: intent.md. 경로는 2D-Portfolio/ 기준. -->

## 한 줄 요약

Engine 에 렌더러 인터페이스 `IRenderer` 와 이를 감싸는 싱글턴 `CRenderer` 를 두고, 지금의 D3D9 코드를 `CRendererD3D9` 로 옮긴다.
게임 코드 78곳은 `GET_SINGLE(CRenderer)->DrawSprite(...)` 한 가지 형태로 바뀐다.

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | **인터페이스** `IRenderer`(Engine). 함수는 다음 여섯 개다 — `Initialize(창, 창 모드)`, `BeginFrame(지우는 색)`, `EndFrame(창)`, `CreateTexture(경로, 컬러키, TEXINFO&)`, `DrawSprite(텍스처, 원본 사각형, 중심점, 월드 행렬, 색)`, `SaveBackBuffer(경로)` | 코드 검토 |
| R2 | **텍스처 핸들** `IRenderTexture`(Engine). 백엔드가 만든 텍스처를 감추는 추상 클래스로, 가상 소멸자만 가진다. `TEXINFO` 는 `IRenderTexture* pTexture` 와 크기 `ImgInfo`(`Width`·`Height` 만 가진 구조체)를 담는다 | 코드 검토 |
| R3 | **필드 이름 유지.** `TEXINFO::ImgInfo.Width/Height` 라는 이름을 그대로 둔다 — 크기를 읽는 62곳은 고치지 않는다 | `git diff` |
| R4 | **싱글턴** `CRenderer`(Engine). 앱이 만든 백엔드를 `Initialize(IRenderer*, 창, 창 모드)` 로 받아 소유하고, 나머지 함수를 그대로 전달한다. 캡처 요청(`RequestCapture`)도 여기서 받아 `EndFrame` 때 `SaveBackBuffer` 를 부른다 | 코드 검토 |
| R5 | **D3D9 구현** `CRendererD3D9`. 지금의 `CDevice` 코드(장치·`ID3DXSprite`·`Present(hWnd)`)와 텍스처 적재(`D3DXGetImageInfoFromFile` + `D3DXCreateTextureFromFileEx`)를 옮긴다. Client 의 깊이 테스트 설정은 생성자 인자(`bDepthTest`)로 받는다 | 코드 검토 |
| R6 | **텍스처 클래스.** `CSingleTexture`·`CMultiTexture` 가 D3DX 대신 `CRenderer::CreateTexture` 를 부른다. 컬러키 규칙(단장 흰색, 여러 장은 `SetMultiColorKey` 값)은 그대로다 | 코드 검토 |
| R7 | **호출부.** Client·툴의 `GetSprite()->SetTransform` + `GetSprite()->Draw` 를 모두 `DrawSprite` 로 바꾼다. `GetSprite()`·`GetDevice()` 를 부르는 곳이 Engine 밖에 남지 않는다 | `git grep` |
| R8 | **백엔드 생성은 앱이 한다.** Client·툴이 각자 `new CRendererD3D9(...)` 를 만들어 넘긴다. Engine 은 어떤 백엔드가 있는지 모른다 | 코드 검토 |
| R9 | **화면 동일.** 변경 전 빌드와 변경 후 빌드의 캡처(`-smoke -shot`)를 허용 오차 0 으로 비교해 모두 같다. Win32 Debug·x64 Release 두 구성 | 비교 스크립트 |
| R10 | 4개 구성 빌드 경고 0, Client·툴 스모크 PASS, 툴 일반 실행에서 타일·미니맵이 그려짐(눈으로 확인) | CLAUDE.md |
| R11 | 문서 — CLAUDE.md 의 싱글턴 목록·「공용 렌더링」 규칙, ARCHITECTURE.md 의 구조 그림 | 문서 검토 |

## 설계

### 인터페이스의 모양

```cpp
class IRenderTexture
{
public:
	virtual ~IRenderTexture() {}
};

class IRenderer
{
public:
	virtual ~IRenderer() {}
	virtual HRESULT Initialize(const HWND hWnd, const WINMODE Mode) = 0;
	virtual void BeginFrame(const D3DCOLOR ClearColor) = 0;
	virtual void EndFrame(const HWND hWnd) = 0;
	virtual HRESULT CreateTexture(const TCHAR* pPath, const D3DCOLOR ColorKey, TEXINFO& rOut) = 0;
	virtual void DrawSprite(const TEXINFO* pTex, const RECT* pSrc,
		const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR Color) = 0;
	virtual bool SaveBackBuffer(const TCHAR* pPath) = 0;
};
```

이렇게 정한 이유:

- **지금 쓰는 형태 그대로다.** 모든 호출부가 「행렬 설정 → 텍스처·원본 사각형·중심점·색으로 그리기」다. `DrawSprite` 는 이 두 호출을 하나로 합친 것뿐이라
  호출부를 기계적으로 바꿀 수 있고, 실수할 여지가 작다. `Draw` 의 네 번째 인자(위치)는 모든 곳에서 NULL 이라 뺐다.
- **텍스처는 `TEXINFO` 로 넘긴다.** 호출부는 이미 `TEXINFO*` 를 들고 있다. 백엔드는 그 안의 `IRenderTexture*` 를 자기 타입으로 바꿔 쓴다.
- **D3D12 를 막지 않는다.** 프레임 시작·끝과 스프라이트 제출만 있으므로, 명령 목록·동기화 같은 세부는 구현 안에 숨길 수 있다.
- **화면 크기·전체 화면 같은 기능은 넣지 않는다.** 지금 쓰지 않는다(YAGNI).

### 싱글턴과 소유권

- 게임 코드는 지금처럼 싱글턴으로 접근한다(`GET_SINGLE(CRenderer)`). 기존 관례(`SINGLETON` 매크로)를 따른다.
- `CRenderer` 는 앱이 넘긴 백엔드를 소유하고 `Release` 때 지운다. 앱은 `new` 만 하고 지우지 않는다.
- 기존 `CDevice` 는 없어진다. 이름이 「D3D 장치」를 뜻해 인터페이스의 역할과 맞지 않는다.

### 이름과 파일

| 클래스 | 파일 | 역할 |
|---|---|---|
| `IRenderer`, `IRenderTexture` | `Engine/Renderer.h` | 인터페이스 |
| `CRenderer` | `Engine/RendererMgr.h/.cpp` | 싱글턴. 백엔드 소유·전달·캡처 요청 |
| `CRendererD3D9` | `Engine/RendererD3D9.h/.cpp` | D3D9 구현(기존 `Device.h/.cpp` 를 `git mv`) |

인터페이스 두 개를 한 헤더에 두는 것은 「클래스 1개 = 파일 1쌍」 관례의 예외다. 둘 다 몸통이 없는 짧은 선언이고 항상 함께 쓰이기 때문이다.

### 깊이 테스트

Client 는 지금 `D3DRS_ZENABLE` 을 켠다. 스프라이트 Z 값이 모두 같아 실제 효과는 없어 보이지만, 「픽셀 하나 다르지 않다」가 목표이므로 이번에는 그대로 옮긴다.
이 설정은 D3D9 에만 있는 개념이라 인터페이스가 아니라 `CRendererD3D9` 생성자 인자로 받는다. 필요한지는 3단계에서 캡처로 판단한다.

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| `TEXINFO` 를 바꾸면 저장 데이터가 깨지나 | 데이터 호환 | `TEXINFO` 는 `TILE` 과 달리 파일에 쓰지 않는다. plan 1번에서 `WriteFile`/`ReadFile` 대상 구조체를 다시 확인한다 |
| 한 번의 `SetTransform` 뒤에 `Draw` 를 여러 번 하는 곳 | 기계적 변환의 실수 | 그런 곳은 같은 행렬을 `DrawSprite` 마다 넘긴다. 파일마다 바꾼 뒤 캡처로 확인한다 |
| 툴은 캡처 비교 대상이 아니다 | 툴 회귀 | 툴 스모크(맵 파일 읽기)와 일반 실행 화면을 눈으로 확인한다. shared-render-lib 때와 같은 방법 |
