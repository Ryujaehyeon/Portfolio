# 1장 정답 — 장치와 스왑체인

> **막혔을 때만 여세요.** 본문: [01-device](../01-device.md). 이 파일은 뼈대 상태에서 이 장을 마친 상태까지 **바뀐 줄만** 보여 줍니다(`+` 더한 줄, `-` 지운 줄).
> 경로는 `2D-Portfolio/` 기준. 실제로 빌드해 체크포인트를 확인한 코드입니다.

## `Engine/RendererD3D11Custom.h`

```diff
--- a/Engine/RendererD3D11Custom.h
+++ b/Engine/RendererD3D11Custom.h
@@ -20,5 +20,5 @@
 	};
 private:
-	// TODO(1장): 장치·컨텍스트·스왑체인·렌더 타깃 뷰를 만든다
+	// [1장] 장치·컨텍스트·스왑체인·렌더 타깃 뷰
 	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
 	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
```

## `Engine/RendererD3D11Custom.cpp`

```diff
--- a/Engine/RendererD3D11Custom.cpp
+++ b/Engine/RendererD3D11Custom.cpp
@@ -25,14 +25,68 @@
 }
 
-// TODO(1장): 플립 모델 스왑체인과 장치를 만들고, 백버퍼로 렌더 타깃 뷰를 만든다
 // TODO(2장~7장): 셰이더·입력 레이아웃·버퍼·상태 객체를 만든다
-HRESULT CRendererD3D11Custom::Initialize(const HWND /*hWnd*/, const WINMODE /*Mode*/)
+HRESULT CRendererD3D11Custom::Initialize(const HWND hWnd, const WINMODE Mode)
 {
+	if (Mode != WINMODE_WIN)
+	{
+		ERR_MSG(hWnd, L"D3D11 백엔드는 창 모드만 지원합니다");
+		return E_FAIL;
+	}
+
+	// [1장] 스왑체인: 화면에 내보낼 버퍼 2개(플립 모델), 백버퍼는 B8G8R8A8
+	DXGI_SWAP_CHAIN_DESC tSwapDesc = {};
+	tSwapDesc.BufferDesc.Width = WINSIZEX;
+	tSwapDesc.BufferDesc.Height = WINSIZEY;
+	tSwapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
+	tSwapDesc.SampleDesc.Count = 1;
+	tSwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
+	tSwapDesc.BufferCount = 2;
+	tSwapDesc.OutputWindow = hWnd;
+	tSwapDesc.Windowed = TRUE;
+	tSwapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
+
+	// [1장] 장치·컨텍스트·스왑체인을 한 번에 만든다. Debug 는 디버그 레이어를 켠다(없으면 끄고 다시)
+	const D3D_FEATURE_LEVEL arrLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
+	UINT uFlags = 0;
+#ifdef _DEBUG
+	uFlags |= D3D11_CREATE_DEVICE_DEBUG;
+#endif
+	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, uFlags,
+		arrLevels, _countof(arrLevels), D3D11_SDK_VERSION, &tSwapDesc,
+		m_pSwapChain.GetAddressOf(), m_pDevice.GetAddressOf(), nullptr, m_pContext.GetAddressOf());
+	if (FAILED(hr) && (uFlags & D3D11_CREATE_DEVICE_DEBUG) != 0)
+	{
+		hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
+			arrLevels, _countof(arrLevels), D3D11_SDK_VERSION, &tSwapDesc,
+			m_pSwapChain.GetAddressOf(), m_pDevice.GetAddressOf(), nullptr, m_pContext.GetAddressOf());
+	}
+	if (FAILED(hr))
+	{
+		ERR_MSG(hWnd, L"D3D11 장치 생성 실패(0x%08X)", static_cast<unsigned int>(hr));
+		return E_FAIL;
+	}
+
+	// [1장] 백버퍼에 그리려면 「렌더 타깃 뷰」가 필요하다
+	ComPtr<ID3D11Texture2D> pBackBuffer;
+	if (FAILED(m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.GetAddressOf())))
+		|| FAILED(m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, m_pRTV.GetAddressOf())))
+	{
+		ERR_MSG(hWnd, L"D3D11 렌더 타깃 생성 실패");
+		return E_FAIL;
+	}
 	return S_OK;
 }
 
-// TODO(1장): 렌더 타깃을 연결하고 뷰포트를 정한 뒤 ClearColor 로 지운다
-void CRendererD3D11Custom::BeginFrame(const D3DCOLOR /*ClearColor*/)
+void CRendererD3D11Custom::BeginFrame(const D3DCOLOR ClearColor)
 {
+	// [1장] 플립 모델은 Present 뒤에 렌더 타깃 연결이 풀린다 — 프레임마다 다시 연결한다
+	m_pContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), nullptr);
+	const D3D11_VIEWPORT tViewport = { 0.f, 0.f, static_cast<float>(WINSIZEX), static_cast<float>(WINSIZEY), 0.f, 1.f };
+	m_pContext->RSSetViewports(1, &tViewport);
+
+	// [1장] D3DCOLOR(0xAARRGGBB) → 0~1 실수 RGBA
+	const float arrColor[4] = { ((ClearColor >> 16) & 0xFF) / 255.f, ((ClearColor >> 8) & 0xFF) / 255.f,
+		(ClearColor & 0xFF) / 255.f, ((ClearColor >> 24) & 0xFF) / 255.f };
+	m_pContext->ClearRenderTargetView(m_pRTV.Get(), arrColor);
 }
 
@@ -43,7 +97,8 @@
 }
 
-// TODO(1장): 스왑체인으로 화면에 내보낸다
 void CRendererD3D11Custom::Present(const HWND /*hWnd*/)
 {
+	// [1장] 스왑체인이 그릴 창을 이미 안다. 수직 동기 없이 내보낸다(D3D9 판과 같게)
+	m_pSwapChain->Present(0, 0);
 }
 
```
