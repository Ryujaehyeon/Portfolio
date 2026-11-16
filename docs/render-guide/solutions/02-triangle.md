# 2장 정답 — 첫 삼각형

> **막혔을 때만 여세요.** 본문: [02-triangle](../02-triangle.md). 이 파일은 1장 정답 상태에서 이 장을 마친 상태까지 **바뀐 줄만** 보여 줍니다(`+` 더한 줄, `-` 지운 줄).
> 경로는 `2D-Portfolio/` 기준. 실제로 빌드해 체크포인트를 확인한 코드입니다.

## `Engine/RendererD3D11Custom.h`

```diff
--- a/Engine/RendererD3D11Custom.h
+++ b/Engine/RendererD3D11Custom.h
@@ -5,4 +5,5 @@
 #include <d3d11.h>
 #include <wrl/client.h>
+#include <DirectXMath.h>
 
 // Direct3D 11 스프라이트 렌더러 — 직접 구현 판(A). -d3d11custom 으로 선택한다(intent/render-guide).
@@ -12,4 +13,12 @@
 	: public IRenderer
 {
+private:
+	// [2장] 정점 하나 — 셰이더의 VS_INPUT 과 같은 순서·형식(위치는 2장에서 클립 좌표, 3장부터 화면 픽셀)
+	struct SPRITEVERTEX
+	{
+		DirectX::XMFLOAT2 vPos;
+		DirectX::XMFLOAT2 vUV;
+		DirectX::XMFLOAT4 vColor;
+	};
 private:
 	// 이 백엔드가 만든 텍스처 — TODO(4장): 셰이더가 읽을 SRV 를 담는다
@@ -25,5 +34,10 @@
 	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
 	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRTV;
-	// TODO(2장~7장): 장마다 필요한 멤버를 여기에 더한다
+	// [2장] 셰이더·입력 레이아웃·첫 삼각형 정점 버퍼
+	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
+	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
+	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
+	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pTriangleVB;
+	// TODO(3장~7장): 장마다 필요한 멤버를 여기에 더한다
 public:
 	virtual HRESULT Initialize(const HWND hWnd, const WINMODE Mode) override;
```

## `Engine/RendererD3D11Custom.cpp`

```diff
--- a/Engine/RendererD3D11Custom.cpp
+++ b/Engine/RendererD3D11Custom.cpp
@@ -16,4 +16,5 @@
 
 using Microsoft::WRL::ComPtr;
+using namespace DirectX;
 
 CRendererD3D11Custom::CRendererD3D11Custom(void)
@@ -25,5 +26,4 @@
 }
 
-// TODO(2장~7장): 셰이더·입력 레이아웃·버퍼·상태 객체를 만든다
 HRESULT CRendererD3D11Custom::Initialize(const HWND hWnd, const WINMODE Mode)
 {
@@ -75,4 +75,44 @@
 		return E_FAIL;
 	}
+
+	// [2장] 셰이더 — 빌드 때 컴파일된 바이트코드(g_SpriteVS·g_SpritePS)로 만든다
+	if (FAILED(m_pDevice->CreateVertexShader(g_SpriteVS, sizeof(g_SpriteVS), nullptr, m_pVertexShader.GetAddressOf()))
+		|| FAILED(m_pDevice->CreatePixelShader(g_SpritePS, sizeof(g_SpritePS), nullptr, m_pPixelShader.GetAddressOf())))
+	{
+		ERR_MSG(hWnd, L"D3D11 셰이더 생성 실패");
+		return E_FAIL;
+	}
+
+	// [2장] 입력 레이아웃: 정점 버퍼의 바이트를 셰이더의 VS_INPUT 으로 읽는 방법
+	const D3D11_INPUT_ELEMENT_DESC arrElements[] =
+	{
+		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
+		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
+		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
+	};
+	if (FAILED(m_pDevice->CreateInputLayout(arrElements, _countof(arrElements), g_SpriteVS, sizeof(g_SpriteVS),
+		m_pInputLayout.GetAddressOf())))
+	{
+		ERR_MSG(hWnd, L"D3D11 입력 레이아웃 생성 실패");
+		return E_FAIL;
+	}
+
+	// [2장] 첫 삼각형 — 클립 좌표(가운데 0,0 / 오른쪽 위 1,1). 화면에서 시계 방향으로 놓아야 앞면이다
+	const SPRITEVERTEX arrTriangle[3] =
+	{
+		{ XMFLOAT2(0.f, 0.5f), XMFLOAT2(0.f, 0.f), XMFLOAT4(1.f, 0.f, 0.f, 1.f) },
+		{ XMFLOAT2(0.5f, -0.5f), XMFLOAT2(0.f, 0.f), XMFLOAT4(0.f, 1.f, 0.f, 1.f) },
+		{ XMFLOAT2(-0.5f, -0.5f), XMFLOAT2(0.f, 0.f), XMFLOAT4(0.f, 0.f, 1.f, 1.f) },
+	};
+	D3D11_BUFFER_DESC tVBDesc = {};
+	tVBDesc.ByteWidth = sizeof(arrTriangle);
+	tVBDesc.Usage = D3D11_USAGE_IMMUTABLE;
+	tVBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
+	const D3D11_SUBRESOURCE_DATA tVBData = { arrTriangle, 0, 0 };
+	if (FAILED(m_pDevice->CreateBuffer(&tVBDesc, &tVBData, m_pTriangleVB.GetAddressOf())))
+	{
+		ERR_MSG(hWnd, L"D3D11 정점 버퍼 생성 실패");
+		return E_FAIL;
+	}
 	return S_OK;
 }
@@ -91,8 +131,16 @@
 }
 
-// TODO(2장): 첫 삼각형을 여기서 그려 본다(장 끝에서 지운다)
 // TODO(7장): 모아 둔 스프라이트를 마저 그린다
 void CRendererD3D11Custom::EndFrame(void)
 {
+	// [2장] 첫 삼각형 — 파이프라인 단계마다 무엇을 쓸지 정하고 그린다
+	const UINT uStride = sizeof(SPRITEVERTEX);
+	const UINT uOffset = 0;
+	m_pContext->IASetInputLayout(m_pInputLayout.Get());
+	m_pContext->IASetVertexBuffers(0, 1, m_pTriangleVB.GetAddressOf(), &uStride, &uOffset);
+	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
+	m_pContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
+	m_pContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
+	m_pContext->Draw(3, 0);
 }
 
```

## `Engine/Shaders/SpriteVS.hlsl`

```diff
--- a/Engine/Shaders/SpriteVS.hlsl
+++ b/Engine/Shaders/SpriteVS.hlsl
@@ -1,7 +1,27 @@
 // 스프라이트 정점 셰이더(intent/render-guide). 이 파일은 UTF-8, BOM 없이 저장한다 — fxc 는 BOM 을 읽지 못한다.
-// 뼈대: 컴파일만 되는 최소 내용. TODO(2장): 입력·출력 구조체를 정하고 위치를 그대로 넘긴다. TODO(3장): 직교 투영. TODO(4장): UV
-float4 main(float2 vPos : POSITION) : SV_POSITION
+
+// [2장] 정점 하나에 담기는 것 — C++ 의 정점 구조체·입력 레이아웃과 순서·형식이 같아야 한다
+struct VS_INPUT
 {
-	return float4(vPos, 0.f, 1.f);
+	float2 vPos : POSITION;
+	float2 vUV : TEXCOORD0;
+	float4 vColor : COLOR0;
+};
+
+// [2장] 래스터라이저와 픽셀 셰이더로 넘기는 것. SV_POSITION 은 클립 좌표(x·y 가 -1~1 인 화면)
+struct VS_OUTPUT
+{
+	float4 vPos : SV_POSITION;
+	float2 vUV : TEXCOORD0;
+	float4 vColor : COLOR0;
+};
+
+VS_OUTPUT main(VS_INPUT tIn)
+{
+	VS_OUTPUT tOut;
+	tOut.vPos = float4(tIn.vPos, 0.f, 1.f);	// [2장] 아직 위치를 클립 좌표 그대로 쓴다
+	tOut.vUV = tIn.vUV;
+	tOut.vColor = tIn.vColor;
+	return tOut;
 }
 
```

## `Engine/Shaders/SpritePS.hlsl`

```diff
--- a/Engine/Shaders/SpritePS.hlsl
+++ b/Engine/Shaders/SpritePS.hlsl
@@ -1,7 +1,15 @@
 // 스프라이트 픽셀 셰이더(intent/render-guide). 이 파일은 UTF-8, BOM 없이 저장한다 — fxc 는 BOM 을 읽지 못한다.
-// 뼈대: 컴파일만 되는 최소 내용. TODO(2장): 정점 색. TODO(4장): 텍스처 샘플링. TODO(5장): 색 곱
-float4 main() : SV_TARGET
+
+// [2장] 정점 셰이더의 출력과 같은 모양
+struct PS_INPUT
 {
-	return float4(1.f, 1.f, 1.f, 1.f);
+	float4 vPos : SV_POSITION;
+	float2 vUV : TEXCOORD0;
+	float4 vColor : COLOR0;
+};
+
+float4 main(PS_INPUT tIn) : SV_TARGET
+{
+	return tIn.vColor;	// [2장] 정점 색(세 꼭짓점 사이는 래스터라이저가 보간한다)
 }
 
```
