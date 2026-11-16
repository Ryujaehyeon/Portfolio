# 3장 정답 — 픽셀 좌표의 사각형

> **막혔을 때만 여세요.** 본문: [03-pixel-quad](../03-pixel-quad.md). 이 파일은 2장 정답 상태에서 이 장을 마친 상태까지 **바뀐 줄만** 보여 줍니다(`+` 더한 줄, `-` 지운 줄).
> 경로는 `2D-Portfolio/` 기준. 실제로 빌드해 체크포인트를 확인한 코드입니다.

## `Engine/RendererD3D11Custom.h`

```diff
--- a/Engine/RendererD3D11Custom.h
+++ b/Engine/RendererD3D11Custom.h
@@ -34,10 +34,13 @@
 	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
 	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRTV;
-	// [2장] 셰이더·입력 레이아웃·첫 삼각형 정점 버퍼
+	// [2장] 셰이더·입력 레이아웃
 	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
 	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
 	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
-	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pTriangleVB;
-	// TODO(3장~7장): 장마다 필요한 멤버를 여기에 더한다
+	// [3장] 사각형 하나의 정점(CPU 가 매번 고쳐 쓰는 동적 버퍼)·인덱스·투영 행렬 상수 버퍼
+	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
+	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
+	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pProjectionBuffer;
+	// TODO(4장~7장): 장마다 필요한 멤버를 여기에 더한다
 public:
 	virtual HRESULT Initialize(const HWND hWnd, const WINMODE Mode) override;
```

## `Engine/RendererD3D11Custom.cpp`

```diff
--- a/Engine/RendererD3D11Custom.cpp
+++ b/Engine/RendererD3D11Custom.cpp
@@ -98,19 +98,41 @@
 	}
 
-	// [2장] 첫 삼각형 — 클립 좌표(가운데 0,0 / 오른쪽 위 1,1). 화면에서 시계 방향으로 놓아야 앞면이다
-	const SPRITEVERTEX arrTriangle[3] =
-	{
-		{ XMFLOAT2(0.f, 0.5f), XMFLOAT2(0.f, 0.f), XMFLOAT4(1.f, 0.f, 0.f, 1.f) },
-		{ XMFLOAT2(0.5f, -0.5f), XMFLOAT2(0.f, 0.f), XMFLOAT4(0.f, 1.f, 0.f, 1.f) },
-		{ XMFLOAT2(-0.5f, -0.5f), XMFLOAT2(0.f, 0.f), XMFLOAT4(0.f, 0.f, 1.f, 1.f) },
-	};
+	// [3장] 사각형 하나 = 정점 4개. CPU 가 스프라이트마다 고쳐 쓰므로 DYNAMIC + CPU 쓰기
 	D3D11_BUFFER_DESC tVBDesc = {};
-	tVBDesc.ByteWidth = sizeof(arrTriangle);
-	tVBDesc.Usage = D3D11_USAGE_IMMUTABLE;
+	tVBDesc.ByteWidth = sizeof(SPRITEVERTEX) * 4;
+	tVBDesc.Usage = D3D11_USAGE_DYNAMIC;
 	tVBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
-	const D3D11_SUBRESOURCE_DATA tVBData = { arrTriangle, 0, 0 };
-	if (FAILED(m_pDevice->CreateBuffer(&tVBDesc, &tVBData, m_pTriangleVB.GetAddressOf())))
+	tVBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
+	if (FAILED(m_pDevice->CreateBuffer(&tVBDesc, nullptr, m_pVertexBuffer.GetAddressOf())))
 	{
 		ERR_MSG(hWnd, L"D3D11 정점 버퍼 생성 실패");
+		return E_FAIL;
+	}
+
+	// [3장] 정점 4개(0 왼쪽 위, 1 오른쪽 위, 2 왼쪽 아래, 3 오른쪽 아래)를 삼각형 2개로 잇는 순서 — 바뀌지 않는다
+	const WORD arrIndices[6] = { 0, 1, 2, 2, 1, 3 };
+	D3D11_BUFFER_DESC tIBDesc = {};
+	tIBDesc.ByteWidth = sizeof(arrIndices);
+	tIBDesc.Usage = D3D11_USAGE_IMMUTABLE;
+	tIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
+	const D3D11_SUBRESOURCE_DATA tIBData = { arrIndices, 0, 0 };
+	if (FAILED(m_pDevice->CreateBuffer(&tIBDesc, &tIBData, m_pIndexBuffer.GetAddressOf())))
+	{
+		ERR_MSG(hWnd, L"D3D11 인덱스 버퍼 생성 실패");
+		return E_FAIL;
+	}
+
+	// [3장] 직교 투영: 화면 픽셀(왼쪽 위 0,0 / 오른쪽 아래 WINSIZEX,WINSIZEY) → 클립 좌표(-1~1, 위가 +)
+	XMFLOAT4X4 matProjection;
+	XMStoreFloat4x4(&matProjection, XMMatrixOrthographicOffCenterLH(
+		0.f, static_cast<float>(WINSIZEX), static_cast<float>(WINSIZEY), 0.f, 0.f, 1.f));
+	D3D11_BUFFER_DESC tCBDesc = {};
+	tCBDesc.ByteWidth = sizeof(matProjection);
+	tCBDesc.Usage = D3D11_USAGE_IMMUTABLE;
+	tCBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
+	const D3D11_SUBRESOURCE_DATA tCBData = { &matProjection, 0, 0 };
+	if (FAILED(m_pDevice->CreateBuffer(&tCBDesc, &tCBData, m_pProjectionBuffer.GetAddressOf())))
+	{
+		ERR_MSG(hWnd, L"D3D11 상수 버퍼 생성 실패");
 		return E_FAIL;
 	}
@@ -129,18 +151,20 @@
 		(ClearColor & 0xFF) / 255.f, ((ClearColor >> 24) & 0xFF) / 255.f };
 	m_pContext->ClearRenderTargetView(m_pRTV.Get(), arrColor);
-}
-
-// TODO(7장): 모아 둔 스프라이트를 마저 그린다
-void CRendererD3D11Custom::EndFrame(void)
-{
-	// [2장] 첫 삼각형 — 파이프라인 단계마다 무엇을 쓸지 정하고 그린다
+
+	// [3장] 이번 프레임 동안 바뀌지 않는 파이프라인 설정은 한 번만 건다
 	const UINT uStride = sizeof(SPRITEVERTEX);
 	const UINT uOffset = 0;
 	m_pContext->IASetInputLayout(m_pInputLayout.Get());
-	m_pContext->IASetVertexBuffers(0, 1, m_pTriangleVB.GetAddressOf(), &uStride, &uOffset);
+	m_pContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &uStride, &uOffset);
+	m_pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
 	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
 	m_pContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
+	m_pContext->VSSetConstantBuffers(0, 1, m_pProjectionBuffer.GetAddressOf());
 	m_pContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
-	m_pContext->Draw(3, 0);
+}
+
+// TODO(7장): 모아 둔 스프라이트를 마저 그린다
+void CRendererD3D11Custom::EndFrame(void)
+{
 }
 
@@ -168,10 +192,36 @@
 }
 
-// TODO(3장): 스프라이트마다 사각형 하나 — 이동만 반영
 // TODO(4장~6장): 텍스처·색·월드 행렬·원본 사각형을 반영
 // TODO(7장): 바로 그리지 않고 모았다가 한 번에 그린다
-void CRendererD3D11Custom::DrawSprite(const TEXINFO* /*pTexInfo*/, const RECT* /*pSrcRect*/,
-	const D3DXVECTOR3& /*vCenter*/, const D3DXMATRIX& /*matWorld*/, const D3DCOLOR /*Color*/)
-{
+void CRendererD3D11Custom::DrawSprite(const TEXINFO* pTexInfo, const RECT* /*pSrcRect*/,
+	const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR /*Color*/)
+{
+	if (pTexInfo == nullptr)
+	{
+		return;
+	}
+	// [3장] 크기·회전은 아직 무시하고 이동만 — 왼쪽 위 = 월드 행렬의 이동 − 중심점
+	const float fLeft = matWorld._41 - vCenter.x;
+	const float fTop = matWorld._42 - vCenter.y;
+	const float fRight = fLeft + static_cast<float>(pTexInfo->ImgInfo.Width);
+	const float fBottom = fTop + static_cast<float>(pTexInfo->ImgInfo.Height);
+	const XMFLOAT4 vWhite(1.f, 1.f, 1.f, 1.f);
+	const SPRITEVERTEX arrQuad[4] =
+	{
+		{ XMFLOAT2(fLeft, fTop), XMFLOAT2(0.f, 0.f), vWhite },
+		{ XMFLOAT2(fRight, fTop), XMFLOAT2(1.f, 0.f), vWhite },
+		{ XMFLOAT2(fLeft, fBottom), XMFLOAT2(0.f, 1.f), vWhite },
+		{ XMFLOAT2(fRight, fBottom), XMFLOAT2(1.f, 1.f), vWhite },
+	};
+
+	// [3장] 동적 버퍼 덮어쓰기 — WRITE_DISCARD: 이전 내용은 버리고 새 메모리를 받는다(GPU 가 아직 읽는 중이어도 기다리지 않는다)
+	D3D11_MAPPED_SUBRESOURCE tMapped = {};
+	if (FAILED(m_pContext->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMapped)))
+	{
+		return;
+	}
+	memcpy(tMapped.pData, arrQuad, sizeof(arrQuad));
+	m_pContext->Unmap(m_pVertexBuffer.Get(), 0);
+	m_pContext->DrawIndexed(6, 0, 0);
 }
 
```

## `Engine/Shaders/SpriteVS.hlsl`

```diff
--- a/Engine/Shaders/SpriteVS.hlsl
+++ b/Engine/Shaders/SpriteVS.hlsl
@@ -1,3 +1,9 @@
 // 스프라이트 정점 셰이더(intent/render-guide). 이 파일은 UTF-8, BOM 없이 저장한다 — fxc 는 BOM 을 읽지 못한다.
+
+// [3장] 화면 픽셀 좌표 → 클립 좌표로 바꾸는 직교 투영 행렬. C++ 의 XMMATRIX 처럼 행 우선으로 읽는다
+cbuffer ProjectionBuffer : register(b0)
+{
+	row_major float4x4 g_matProjection;
+};
 
 // [2장] 정점 하나에 담기는 것 — C++ 의 정점 구조체·입력 레이아웃과 순서·형식이 같아야 한다
@@ -20,5 +26,5 @@
 {
 	VS_OUTPUT tOut;
-	tOut.vPos = float4(tIn.vPos, 0.f, 1.f);	// [2장] 아직 위치를 클립 좌표 그대로 쓴다
+	tOut.vPos = mul(float4(tIn.vPos, 0.f, 1.f), g_matProjection);	// [3장] 위치는 화면 픽셀 좌표
 	tOut.vUV = tIn.vUV;
 	tOut.vColor = tIn.vColor;
```

## `Engine/Shaders/SpritePS.hlsl`

```diff
--- a/Engine/Shaders/SpritePS.hlsl
+++ b/Engine/Shaders/SpritePS.hlsl
@@ -11,5 +11,5 @@
 float4 main(PS_INPUT tIn) : SV_TARGET
 {
-	return tIn.vColor;	// [2장] 정점 색(세 꼭짓점 사이는 래스터라이저가 보간한다)
+	return float4(tIn.vUV, 0.f, 1.f) * tIn.vColor;	// [3장] 텍스처가 아직 없다 — UV 를 색으로 보인다(왼쪽 위 검정, 오른쪽 위 빨강, 왼쪽 아래 초록)
 }
 
```
