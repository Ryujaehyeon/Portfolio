# 7장 정답 — 배칭

> **막혔을 때만 여세요.** 본문: [07-batching](../07-batching.md). 이 파일은 6장 정답 상태에서 이 장을 마친 상태까지 **바뀐 줄만** 보여 줍니다(`+` 더한 줄, `-` 지운 줄).
> 경로는 `2D-Portfolio/` 기준. 실제로 빌드해 체크포인트를 확인한 코드입니다.

## `Engine/RendererD3D11Custom.h`

```diff
--- a/Engine/RendererD3D11Custom.h
+++ b/Engine/RendererD3D11Custom.h
@@ -39,5 +39,5 @@
 	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
 	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
-	// [3장] 사각형 하나의 정점(CPU 가 매번 고쳐 쓰는 동적 버퍼)·인덱스·투영 행렬 상수 버퍼
+	// [3장] 정점(CPU 가 매번 고쳐 쓰는 동적 버퍼)·인덱스·투영 행렬 상수 버퍼. [7장] 크기를 한 번에 그릴 최대 스프라이트 수만큼 키운다
 	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
 	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
@@ -49,5 +49,10 @@
 	// [6장] 뒤집힌(행렬식이 음수인) 스프라이트도 그리려고 컬링을 끈 래스터라이저 상태
 	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_pRasterizerState;
-	// TODO(7장): 장마다 필요한 멤버를 여기에 더한다
+	// [7장] 배칭: 같은 텍스처가 이어지는 동안 정점을 모았다가 한 번에 그린다
+	static constexpr size_t MAX_BATCH_SPRITES = 2048;	// 정점 4개씩 — 16비트 인덱스(최대 65535)에 들어간다
+	vector<SPRITEVERTEX> m_vecBatch;
+	ID3D11ShaderResourceView* m_pBatchSRV;	// 모으는 중인 텍스처(소유하지 않는다 — 텍스처는 그리는 동안 살아 있다)
+	int m_iDrawCalls;						// 이번 프레임의 그리기 호출 수(확인용)
+	int m_iLastDrawCalls;
 public:
 	virtual HRESULT Initialize(const HWND hWnd, const WINMODE Mode) override;
@@ -59,4 +64,6 @@
 		const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR Color) override;
 	virtual bool SaveBackBuffer(const TCHAR* pPath) override;
+private:
+	void FlushBatch(void);
 public:
 	CRendererD3D11Custom(void);
```

## `Engine/RendererD3D11Custom.cpp`

```diff
--- a/Engine/RendererD3D11Custom.cpp
+++ b/Engine/RendererD3D11Custom.cpp
@@ -2,4 +2,5 @@
 #include "RendererD3D11Custom.h"
 #include "ImageLoader.h"
+#include "Debug.hpp"
 
 #include <dxgi.h>
@@ -19,4 +20,7 @@
 
 CRendererD3D11Custom::CRendererD3D11Custom(void)
+	: m_pBatchSRV(nullptr)
+	, m_iDrawCalls(0)
+	, m_iLastDrawCalls(0)
 {
 }
@@ -98,7 +102,7 @@
 	}
 
-	// [3장] 사각형 하나 = 정점 4개. CPU 가 스프라이트마다 고쳐 쓰므로 DYNAMIC + CPU 쓰기
+	// [3장] 사각형 하나 = 정점 4개. CPU 가 고쳐 쓰므로 DYNAMIC + CPU 쓰기. [7장] 최대 스프라이트 수만큼
 	D3D11_BUFFER_DESC tVBDesc = {};
-	tVBDesc.ByteWidth = sizeof(SPRITEVERTEX) * 4;
+	tVBDesc.ByteWidth = static_cast<UINT>(sizeof(SPRITEVERTEX) * 4 * MAX_BATCH_SPRITES);
 	tVBDesc.Usage = D3D11_USAGE_DYNAMIC;
 	tVBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
@@ -111,10 +115,21 @@
 
 	// [3장] 정점 4개(0 왼쪽 위, 1 오른쪽 위, 2 왼쪽 아래, 3 오른쪽 아래)를 삼각형 2개로 잇는 순서 — 바뀌지 않는다
-	const WORD arrIndices[6] = { 0, 1, 2, 2, 1, 3 };
+	// [7장] 스프라이트 n 번째는 정점 4n ~ 4n+3 을 같은 순서로 잇는다
+	vector<WORD> vecIndices;
+	vecIndices.reserve(MAX_BATCH_SPRITES * 6);
+	for (size_t i = 0; i < MAX_BATCH_SPRITES; ++i)
+	{
+		const WORD wBase = static_cast<WORD>(i * 4);
+		const WORD arrQuadIndices[6] = { 0, 1, 2, 2, 1, 3 };
+		for (const WORD wIndex : arrQuadIndices)
+		{
+			vecIndices.push_back(static_cast<WORD>(wBase + wIndex));
+		}
+	}
 	D3D11_BUFFER_DESC tIBDesc = {};
-	tIBDesc.ByteWidth = sizeof(arrIndices);
+	tIBDesc.ByteWidth = static_cast<UINT>(sizeof(WORD) * vecIndices.size());
 	tIBDesc.Usage = D3D11_USAGE_IMMUTABLE;
 	tIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
-	const D3D11_SUBRESOURCE_DATA tIBData = { arrIndices, 0, 0 };
+	const D3D11_SUBRESOURCE_DATA tIBData = { vecIndices.data(), 0, 0 };
 	if (FAILED(m_pDevice->CreateBuffer(&tIBDesc, &tIBData, m_pIndexBuffer.GetAddressOf())))
 	{
@@ -179,4 +194,6 @@
 		return E_FAIL;
 	}
+
+	m_vecBatch.reserve(MAX_BATCH_SPRITES * 4);
 	return S_OK;
 }
@@ -207,9 +224,38 @@
 	m_pContext->PSSetSamplers(0, 1, m_pSampler.GetAddressOf());
 	m_pContext->OMSetBlendState(m_pBlendState.Get(), nullptr, 0xFFFFFFFF);
-}
-
-// TODO(7장): 모아 둔 스프라이트를 마저 그린다
+
+	m_vecBatch.clear();
+	m_pBatchSRV = nullptr;
+	m_iDrawCalls = 0;
+}
+
 void CRendererD3D11Custom::EndFrame(void)
 {
+	// [7장] 모아 둔 스프라이트를 마저 그린다
+	FlushBatch();
+	if (m_iDrawCalls != m_iLastDrawCalls)
+	{
+		DebugOut(L"[D3D11 custom] draw calls/frame: %d", m_iDrawCalls);	// VS 출력 창에서 배칭 효과를 본다
+		m_iLastDrawCalls = m_iDrawCalls;
+	}
+}
+
+// [7장] 모아 둔 정점을 동적 버퍼에 한 번에 올리고 그리기 호출 한 번으로 그린다
+void CRendererD3D11Custom::FlushBatch(void)
+{
+	if (m_vecBatch.empty())
+	{
+		return;
+	}
+	D3D11_MAPPED_SUBRESOURCE tMapped = {};
+	if (SUCCEEDED(m_pContext->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMapped)))
+	{
+		memcpy(tMapped.pData, m_vecBatch.data(), sizeof(SPRITEVERTEX) * m_vecBatch.size());
+		m_pContext->Unmap(m_pVertexBuffer.Get(), 0);
+		m_pContext->PSSetShaderResources(0, 1, &m_pBatchSRV);
+		m_pContext->DrawIndexed(static_cast<UINT>(m_vecBatch.size() / 4 * 6), 0, 0);
+		++m_iDrawCalls;
+	}
+	m_vecBatch.clear();
 }
 
@@ -260,5 +306,4 @@
 }
 
-// TODO(7장): 바로 그리지 않고 모았다가 한 번에 그린다
 void CRendererD3D11Custom::DrawSprite(const TEXINFO* pTexInfo, const RECT* pSrcRect,
 	const D3DXVECTOR3& vCenter, const D3DXMATRIX& matWorld, const D3DCOLOR Color)
@@ -268,7 +313,11 @@
 		return;
 	}
-	// [4장] 이 스프라이트의 텍스처를 픽셀 셰이더의 t0 에 연결한다
+	// [4장] 이 스프라이트의 텍스처. [7장] 모으는 중인 텍스처와 다르거나 버퍼가 차면 지금까지 모은 것을 먼저 그린다
 	ID3D11ShaderResourceView* const pSRV = static_cast<const CTextureD3D11Custom*>(pTexInfo->pTexture)->m_pSRV.Get();
-	m_pContext->PSSetShaderResources(0, 1, &pSRV);
+	if (pSRV != m_pBatchSRV || m_vecBatch.size() >= MAX_BATCH_SPRITES * 4)
+	{
+		FlushBatch();
+		m_pBatchSRV = pSRV;
+	}
 
 	// [6장] 그릴 범위: 원본 사각형(nullptr 이면 텍스처 전체), 텍셀 단위
@@ -292,25 +341,16 @@
 		(Color & 0xFF) / 255.f, ((Color >> 24) & 0xFF) / 255.f);
 
-	SPRITEVERTEX arrQuad[4];
 	for (int i = 0; i < 4; ++i)
 	{
 		const float fX = arrLocalX[i] - vCenter.x;
 		const float fY = arrLocalY[i] - vCenter.y;
-		arrQuad[i].vPos = XMFLOAT2(fX * matWorld._11 + fY * matWorld._21 + matWorld._41,
+		SPRITEVERTEX tVertex;
+		tVertex.vPos = XMFLOAT2(fX * matWorld._11 + fY * matWorld._21 + matWorld._41,
 			fX * matWorld._12 + fY * matWorld._22 + matWorld._42);
 		// [6장] UV = 텍셀 위치 ÷ 텍스처 크기
-		arrQuad[i].vUV = XMFLOAT2((rcSrc.left + arrLocalX[i]) / fTexWidth, (rcSrc.top + arrLocalY[i]) / fTexHeight);
-		arrQuad[i].vColor = vColor;
-	}
-
-	// [3장] 동적 버퍼 덮어쓰기 — WRITE_DISCARD: 이전 내용은 버리고 새 메모리를 받는다(GPU 가 아직 읽는 중이어도 기다리지 않는다)
-	D3D11_MAPPED_SUBRESOURCE tMapped = {};
-	if (FAILED(m_pContext->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMapped)))
-	{
-		return;
-	}
-	memcpy(tMapped.pData, arrQuad, sizeof(arrQuad));
-	m_pContext->Unmap(m_pVertexBuffer.Get(), 0);
-	m_pContext->DrawIndexed(6, 0, 0);
+		tVertex.vUV = XMFLOAT2((rcSrc.left + arrLocalX[i]) / fTexWidth, (rcSrc.top + arrLocalY[i]) / fTexHeight);
+		tVertex.vColor = vColor;
+		m_vecBatch.push_back(tVertex);	// [7장] 바로 그리지 않고 모은다
+	}
 }
 
```
