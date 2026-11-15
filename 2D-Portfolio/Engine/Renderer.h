#pragma once

#include "Defines.h"

// 렌더러 백엔드가 만든 텍스처를 감춘다. 게임 코드는 TEXINFO::pTexture 로 들고 다니기만 하고,
// 백엔드가 자기 타입으로 바꿔 쓴다(intent/render-backend).
class IRenderTexture
{
public:
	virtual ~IRenderTexture(void) {}
};

// 그래픽 API 별 렌더러가 구현하는 인터페이스. 게임 코드는 CRenderer 싱글턴을 통해서만 부른다.
// 함수 모양은 지금 게임이 쓰는 그리기 그대로다 — 「행렬 설정 + 스프라이트 그리기」를 DrawSprite 하나로 합쳤다.
class IRenderer
{
public:
	virtual ~IRenderer(void) {}
public:
	// 장치를 만든다. 실패하면 오류를 알리고 E_FAIL
	virtual HRESULT Initialize(const HWND hWnd, const WINMODE Mode) PURE;
	// 화면을 ClearColor 로 지우고 그리기를 시작한다
	virtual void BeginFrame(const D3DCOLOR ClearColor) PURE;
	// 그리기를 끝낸다 — 아직 화면에 내보내지 않는다(그 사이에 백버퍼를 저장할 수 있게)
	virtual void EndFrame(void) PURE;
	// 그린 화면을 hWnd 창에 내보낸다
	virtual void Present(const HWND hWnd) PURE;
	// 이미지 파일로 텍스처를 만든다. ColorKey 와 같은 색은 투명. 성공하면 rTexInfo 의 텍스처·크기를 채운다
	virtual HRESULT CreateTexture(const TCHAR* pPath, const D3DCOLOR ColorKey, TEXINFO& rTexInfo) PURE;
	// 원본 사각형(nullptr 이면 전체)을 중심점 기준으로 월드 행렬만큼 옮겨 그린다. Color 는 곱할 색(알파 포함)
	virtual void DrawSprite(const TEXINFO* pTexInfo, const RECT* pSrcRect,
		const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR Color) PURE;
	// EndFrame 뒤·Present 전에 백버퍼를 PNG 로 저장한다
	virtual bool SaveBackBuffer(const TCHAR* pPath) PURE;
};
