# 4장 정답 — 텍스처

> **막혔을 때만 여세요.** 본문: [04-texture](../04-texture.md). 이 파일은 3장 정답 상태에서 이 장을 마친 상태까지 **바뀐 줄만** 보여 줍니다(`+` 더한 줄, `-` 지운 줄).
> 경로는 `2D-Portfolio/` 기준. 실제로 빌드해 체크포인트를 확인한 코드입니다.

## `Engine/RendererD3D11Custom.h`

```diff
--- a/Engine/RendererD3D11Custom.h
+++ b/Engine/RendererD3D11Custom.h
@@ -22,9 +22,10 @@
 	};
 private:
-	// 이 백엔드가 만든 텍스처 — TODO(4장): 셰이더가 읽을 SRV 를 담는다
+	// 이 백엔드가 만든 텍스처 — [4장] 셰이더가 읽는 SRV 하나
 	class CTextureD3D11Custom
 		: public IRenderTexture
 	{
 	public:
+		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV;
 	};
 private:
@@ -42,5 +43,7 @@
 	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
 	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pProjectionBuffer;
-	// TODO(4장~7장): 장마다 필요한 멤버를 여기에 더한다
+	// [4장] 텍스처를 읽는 방법(필터·가장자리 처리)
+	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;
+	// TODO(5장~7장): 장마다 필요한 멤버를 여기에 더한다
 public:
 	virtual HRESULT Initialize(const HWND hWnd, const WINMODE Mode) override;
```

## `Engine/RendererD3D11Custom.cpp`

```diff
--- a/Engine/RendererD3D11Custom.cpp
+++ b/Engine/RendererD3D11Custom.cpp
@@ -137,4 +137,18 @@
 		return E_FAIL;
 	}
+
+	// [4장] 샘플러: 텍셀 사이는 선형 보간, UV 가 0~1 을 벗어나면 가장자리 색(ID3DXSprite 와 같은 설정)
+	D3D11_SAMPLER_DESC tSamplerDesc = {};
+	tSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
+	tSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
+	tSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
+	tSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
+	tSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
+	tSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
+	if (FAILED(m_pDevice->CreateSamplerState(&tSamplerDesc, m_pSampler.GetAddressOf())))
+	{
+		ERR_MSG(hWnd, L"D3D11 샘플러 생성 실패");
+		return E_FAIL;
+	}
 	return S_OK;
 }
@@ -162,4 +176,5 @@
 	m_pContext->VSSetConstantBuffers(0, 1, m_pProjectionBuffer.GetAddressOf());
 	m_pContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
+	m_pContext->PSSetSamplers(0, 1, m_pSampler.GetAddressOf());
 }
 
@@ -175,6 +190,4 @@
 }
 
-// 뼈대는 크기만 채운다 — 게임 코드가 텍스처 크기로 위치를 계산하므로 이것이 있어야 로직이 돈다.
-// TODO(4장): 픽셀로 Texture2D 와 SRV 를 만들어 CTextureD3D11Custom 에 담는다
 HRESULT CRendererD3D11Custom::CreateTexture(const TCHAR* pPath, const D3DCOLOR ColorKey, TEXINFO& rTexInfo)
 {
@@ -186,5 +199,30 @@
 		return E_FAIL;
 	}
-	rTexInfo.pTexture = new CTextureD3D11Custom;
+
+	// [4장] 픽셀(BGRA, 컬러키 적용됨)로 텍스처를 만든다 — 원본 크기, 밉맵 1단계, 만든 뒤 바뀌지 않음
+	D3D11_TEXTURE2D_DESC tDesc = {};
+	tDesc.Width = uWidth;
+	tDesc.Height = uHeight;
+	tDesc.MipLevels = 1;
+	tDesc.ArraySize = 1;
+	tDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
+	tDesc.SampleDesc.Count = 1;
+	tDesc.Usage = D3D11_USAGE_IMMUTABLE;
+	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
+	const D3D11_SUBRESOURCE_DATA tData = { vecPixels.data(), uWidth * 4, 0 };	// 한 줄의 바이트 수 = 너비 × 4
+	ComPtr<ID3D11Texture2D> pTexture;
+	if (FAILED(m_pDevice->CreateTexture2D(&tDesc, &tData, pTexture.GetAddressOf())))
+	{
+		return E_FAIL;
+	}
+
+	// [4장] 셰이더는 텍스처를 직접 읽지 않고 SRV 를 통해 읽는다
+	CTextureD3D11Custom* const pRenderTexture = new CTextureD3D11Custom;
+	if (FAILED(m_pDevice->CreateShaderResourceView(pTexture.Get(), nullptr, pRenderTexture->m_pSRV.GetAddressOf())))
+	{
+		delete pRenderTexture;
+		return E_FAIL;
+	}
+	rTexInfo.pTexture = pRenderTexture;
 	rTexInfo.ImgInfo.Width = uWidth;
 	rTexInfo.ImgInfo.Height = uHeight;
@@ -192,13 +230,17 @@
 }
 
-// TODO(4장~6장): 텍스처·색·월드 행렬·원본 사각형을 반영
+// TODO(5장~6장): 색·월드 행렬·원본 사각형을 반영
 // TODO(7장): 바로 그리지 않고 모았다가 한 번에 그린다
 void CRendererD3D11Custom::DrawSprite(const TEXINFO* pTexInfo, const RECT* /*pSrcRect*/,
 	const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR /*Color*/)
 {
-	if (pTexInfo == nullptr)
+	if (pTexInfo == nullptr || pTexInfo->pTexture == nullptr)
 	{
 		return;
 	}
+	// [4장] 이 스프라이트의 텍스처를 픽셀 셰이더의 t0 에 연결한다
+	ID3D11ShaderResourceView* const pSRV = static_cast<const CTextureD3D11Custom*>(pTexInfo->pTexture)->m_pSRV.Get();
+	m_pContext->PSSetShaderResources(0, 1, &pSRV);
+
 	// [3장] 크기·회전은 아직 무시하고 이동만 — 왼쪽 위 = 월드 행렬의 이동 − 중심점
 	const float fLeft = matWorld._41 - vCenter.x;
```

## `Engine/Shaders/SpritePS.hlsl`

```diff
--- a/Engine/Shaders/SpritePS.hlsl
+++ b/Engine/Shaders/SpritePS.hlsl
@@ -1,3 +1,7 @@
 // 스프라이트 픽셀 셰이더(intent/render-guide). 이 파일은 UTF-8, BOM 없이 저장한다 — fxc 는 BOM 을 읽지 못한다.
+
+// [4장] 그릴 텍스처(t0)와 읽는 방법(s0) — C++ 에서 PSSetShaderResources·PSSetSamplers 로 연결한다
+Texture2D g_Texture : register(t0);
+SamplerState g_Sampler : register(s0);
 
 // [2장] 정점 셰이더의 출력과 같은 모양
@@ -11,5 +15,5 @@
 float4 main(PS_INPUT tIn) : SV_TARGET
 {
-	return float4(tIn.vUV, 0.f, 1.f) * tIn.vColor;	// [3장] 텍스처가 아직 없다 — UV 를 색으로 보인다(왼쪽 위 검정, 오른쪽 위 빨강, 왼쪽 아래 초록)
+	return g_Texture.Sample(g_Sampler, tIn.vUV);	// [4장] UV 위치의 텍스처 색
 }
 
```
