# 5장 정답 — 투명과 색

> **막혔을 때만 여세요.** 본문: [05-blend-color](../05-blend-color.md). 이 파일은 4장 정답 상태에서 이 장을 마친 상태까지 **바뀐 줄만** 보여 줍니다(`+` 더한 줄, `-` 지운 줄).
> 경로는 `2D-Portfolio/` 기준. 실제로 빌드해 체크포인트를 확인한 코드입니다.

## `Engine/RendererD3D11Custom.h`

```diff
--- a/Engine/RendererD3D11Custom.h
+++ b/Engine/RendererD3D11Custom.h
@@ -45,5 +45,7 @@
 	// [4장] 텍스처를 읽는 방법(필터·가장자리 처리)
 	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;
-	// TODO(5장~7장): 장마다 필요한 멤버를 여기에 더한다
+	// [5장] 그린 색과 이미 있는 색을 섞는 방법(알파 블렌드)
+	Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendState;
+	// TODO(6장~7장): 장마다 필요한 멤버를 여기에 더한다
 public:
 	virtual HRESULT Initialize(const HWND hWnd, const WINMODE Mode) override;
```

## `Engine/RendererD3D11Custom.cpp`

```diff
--- a/Engine/RendererD3D11Custom.cpp
+++ b/Engine/RendererD3D11Custom.cpp
@@ -151,4 +151,21 @@
 		return E_FAIL;
 	}
+
+	// [5장] 알파 블렌드: 결과 = 그린 색 × 알파 + 이미 있는 색 × (1 − 알파). D3DXSPRITE_ALPHABLEND 와 같다(알파 채널도 같은 식)
+	D3D11_BLEND_DESC tBlendDesc = {};
+	D3D11_RENDER_TARGET_BLEND_DESC& rTarget = tBlendDesc.RenderTarget[0];
+	rTarget.BlendEnable = TRUE;
+	rTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
+	rTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
+	rTarget.BlendOp = D3D11_BLEND_OP_ADD;
+	rTarget.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
+	rTarget.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
+	rTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
+	rTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
+	if (FAILED(m_pDevice->CreateBlendState(&tBlendDesc, m_pBlendState.GetAddressOf())))
+	{
+		ERR_MSG(hWnd, L"D3D11 블렌드 상태 생성 실패");
+		return E_FAIL;
+	}
 	return S_OK;
 }
@@ -177,4 +194,5 @@
 	m_pContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
 	m_pContext->PSSetSamplers(0, 1, m_pSampler.GetAddressOf());
+	m_pContext->OMSetBlendState(m_pBlendState.Get(), nullptr, 0xFFFFFFFF);
 }
 
@@ -230,8 +248,8 @@
 }
 
-// TODO(5장~6장): 색·월드 행렬·원본 사각형을 반영
+// TODO(6장): 월드 행렬·원본 사각형을 반영
 // TODO(7장): 바로 그리지 않고 모았다가 한 번에 그린다
 void CRendererD3D11Custom::DrawSprite(const TEXINFO* pTexInfo, const RECT* /*pSrcRect*/,
-	const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR /*Color*/)
+	const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR Color)
 {
 	if (pTexInfo == nullptr || pTexInfo->pTexture == nullptr)
@@ -248,11 +266,13 @@
 	const float fRight = fLeft + static_cast<float>(pTexInfo->ImgInfo.Width);
 	const float fBottom = fTop + static_cast<float>(pTexInfo->ImgInfo.Height);
-	const XMFLOAT4 vWhite(1.f, 1.f, 1.f, 1.f);
+	// [5장] D3DCOLOR(0xAARRGGBB) → 0~1 실수 RGBA. 네 꼭짓점이 같은 색이다
+	const XMFLOAT4 vColor(((Color >> 16) & 0xFF) / 255.f, ((Color >> 8) & 0xFF) / 255.f,
+		(Color & 0xFF) / 255.f, ((Color >> 24) & 0xFF) / 255.f);
 	const SPRITEVERTEX arrQuad[4] =
 	{
-		{ XMFLOAT2(fLeft, fTop), XMFLOAT2(0.f, 0.f), vWhite },
-		{ XMFLOAT2(fRight, fTop), XMFLOAT2(1.f, 0.f), vWhite },
-		{ XMFLOAT2(fLeft, fBottom), XMFLOAT2(0.f, 1.f), vWhite },
-		{ XMFLOAT2(fRight, fBottom), XMFLOAT2(1.f, 1.f), vWhite },
+		{ XMFLOAT2(fLeft, fTop), XMFLOAT2(0.f, 0.f), vColor },
+		{ XMFLOAT2(fRight, fTop), XMFLOAT2(1.f, 0.f), vColor },
+		{ XMFLOAT2(fLeft, fBottom), XMFLOAT2(0.f, 1.f), vColor },
+		{ XMFLOAT2(fRight, fBottom), XMFLOAT2(1.f, 1.f), vColor },
 	};
 
```

## `Engine/Shaders/SpritePS.hlsl`

```diff
--- a/Engine/Shaders/SpritePS.hlsl
+++ b/Engine/Shaders/SpritePS.hlsl
@@ -15,5 +15,5 @@
 float4 main(PS_INPUT tIn) : SV_TARGET
 {
-	return g_Texture.Sample(g_Sampler, tIn.vUV);	// [4장] UV 위치의 텍스처 색
+	return g_Texture.Sample(g_Sampler, tIn.vUV) * tIn.vColor;	// [4장] UV 위치의 텍스처 색 × [5장] 스프라이트 색(반투명 등)
 }
 
```
