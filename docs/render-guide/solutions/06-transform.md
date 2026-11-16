# 6장 정답 — 스프라이트 변환

> **막혔을 때만 여세요.** 본문: [06-transform](../06-transform.md). 이 파일은 5장 정답 상태에서 이 장을 마친 상태까지 **바뀐 줄만** 보여 줍니다(`+` 더한 줄, `-` 지운 줄).
> 경로는 `2D-Portfolio/` 기준. 실제로 빌드해 체크포인트를 확인한 코드입니다.

## `Engine/RendererD3D11Custom.h`

```diff
--- a/Engine/RendererD3D11Custom.h
+++ b/Engine/RendererD3D11Custom.h
@@ -47,5 +47,7 @@
 	// [5장] 그린 색과 이미 있는 색을 섞는 방법(알파 블렌드)
 	Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendState;
-	// TODO(6장~7장): 장마다 필요한 멤버를 여기에 더한다
+	// [6장] 뒤집힌(행렬식이 음수인) 스프라이트도 그리려고 컬링을 끈 래스터라이저 상태
+	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterizerState;
+	// TODO(7장): 장마다 필요한 멤버를 여기에 더한다
 public:
 	virtual HRESULT Initialize(const HWND hWnd, const WINMODE Mode) override;
```

## `Engine/RendererD3D11Custom.cpp`

```diff
--- a/Engine/RendererD3D11Custom.cpp
+++ b/Engine/RendererD3D11Custom.cpp
@@ -168,4 +168,15 @@
 		return E_FAIL;
 	}
+
+	// [6장] 기본 래스터라이저는 뒷면(화면에서 반시계 방향)을 버린다. 좌우로 뒤집는 행렬이면 사각형이 뒷면이 되므로 컬링을 끈다
+	D3D11_RASTERIZER_DESC tRasterDesc = {};
+	tRasterDesc.FillMode = D3D11_FILL_SOLID;
+	tRasterDesc.CullMode = D3D11_CULL_NONE;
+	tRasterDesc.DepthClipEnable = TRUE;
+	if (FAILED(m_pDevice->CreateRasterizerState(&tRasterDesc, m_pRasterizerState.GetAddressOf())))
+	{
+		ERR_MSG(hWnd, L"D3D11 래스터라이저 상태 생성 실패");
+		return E_FAIL;
+	}
 	return S_OK;
 }
@@ -177,4 +188,5 @@
 	const D3D11_VIEWPORT tViewport = { 0.f, 0.f, static_cast<float>(WINSIZEX), static_cast<float>(WINSIZEY), 0.f, 1.f };
 	m_pContext->RSSetViewports(1, &tViewport);
+	m_pContext->RSSetState(m_pRasterizerState.Get());
 
 	// [1장] D3DCOLOR(0xAARRGGBB) → 0~1 실수 RGBA
@@ -248,7 +260,6 @@
 }
 
-// TODO(6장): 월드 행렬·원본 사각형을 반영
 // TODO(7장): 바로 그리지 않고 모았다가 한 번에 그린다
-void CRendererD3D11Custom::DrawSprite(const TEXINFO* pTexInfo, const RECT* /*pSrcRect*/,
+void CRendererD3D11Custom::DrawSprite(const TEXINFO* pTexInfo, const RECT* pSrcRect,
 	const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR Color)
 {
@@ -261,19 +272,35 @@
 	m_pContext->PSSetShaderResources(0, 1, &pSRV);
 
-	// [3장] 크기·회전은 아직 무시하고 이동만 — 왼쪽 위 = 월드 행렬의 이동 − 중심점
-	const float fLeft = matWorld._41 - vCenter.x;
-	const float fTop = matWorld._42 - vCenter.y;
-	const float fRight = fLeft + static_cast<float>(pTexInfo->ImgInfo.Width);
-	const float fBottom = fTop + static_cast<float>(pTexInfo->ImgInfo.Height);
+	// [6장] 그릴 범위: 원본 사각형(nullptr 이면 텍스처 전체), 텍셀 단위
+	const float fTexWidth = static_cast<float>(pTexInfo->ImgInfo.Width);
+	const float fTexHeight = static_cast<float>(pTexInfo->ImgInfo.Height);
+	RECT rcSrc = { 0, 0, static_cast<LONG>(pTexInfo->ImgInfo.Width), static_cast<LONG>(pTexInfo->ImgInfo.Height) };
+	if (pSrcRect != nullptr)
+	{
+		rcSrc = *pSrcRect;
+	}
+	const float fWidth = static_cast<float>(rcSrc.right - rcSrc.left);
+	const float fHeight = static_cast<float>(rcSrc.bottom - rcSrc.top);
+
+	// [6장] ID3DXSprite 와 같은 식: 화면 위치 = (꼭짓점 − 중심점) × 월드 행렬.
+	// 중심점은 원본 사각형의 왼쪽 위 기준. D3DX 행렬은 「행 벡터 × 행렬」이라 x' = x·_11 + y·_21 + _41
+	const float arrLocalX[4] = { 0.f, fWidth, 0.f, fWidth };
+	const float arrLocalY[4] = { 0.f, 0.f, fHeight, fHeight };
+
 	// [5장] D3DCOLOR(0xAARRGGBB) → 0~1 실수 RGBA. 네 꼭짓점이 같은 색이다
 	const XMFLOAT4 vColor(((Color >> 16) & 0xFF) / 255.f, ((Color >> 8) & 0xFF) / 255.f,
 		(Color & 0xFF) / 255.f, ((Color >> 24) & 0xFF) / 255.f);
-	const SPRITEVERTEX arrQuad[4] =
-	{
-		{ XMFLOAT2(fLeft, fTop), XMFLOAT2(0.f, 0.f), vColor },
-		{ XMFLOAT2(fRight, fTop), XMFLOAT2(1.f, 0.f), vColor },
-		{ XMFLOAT2(fLeft, fBottom), XMFLOAT2(0.f, 1.f), vColor },
-		{ XMFLOAT2(fRight, fBottom), XMFLOAT2(1.f, 1.f), vColor },
-	};
+
+	SPRITEVERTEX arrQuad[4];
+	for (int i = 0; i < 4; ++i)
+	{
+		const float fX = arrLocalX[i] - vCenter.x;
+		const float fY = arrLocalY[i] - vCenter.y;
+		arrQuad[i].vPos = XMFLOAT2(fX * matWorld._11 + fY * matWorld._21 + matWorld._41,
+			fX * matWorld._12 + fY * matWorld._22 + matWorld._42);
+		// [6장] UV = 텍셀 위치 ÷ 텍스처 크기
+		arrQuad[i].vUV = XMFLOAT2((rcSrc.left + arrLocalX[i]) / fTexWidth, (rcSrc.top + arrLocalY[i]) / fTexHeight);
+		arrQuad[i].vColor = vColor;
+	}
 
 	// [3장] 동적 버퍼 덮어쓰기 — WRITE_DISCARD: 이전 내용은 버리고 새 메모리를 받는다(GPU 가 아직 읽는 중이어도 기다리지 않는다)
```
