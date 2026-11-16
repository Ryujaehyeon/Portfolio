#include "StdAfx.h"
#include "ImageLoader.h"

#include <wincodec.h>
#include <wrl/client.h>

#pragma comment(lib, "windowscodecs.lib")

using Microsoft::WRL::ComPtr;

namespace
{
	// WIC 팩토리는 한 번만 만든다. COM 은 이 스레드에서 처음 쓸 때 초기화한다(이미 다른 방식으로 초기화돼 있어도 쓸 수 있다)
	IWICImagingFactory* GetWicFactory()
	{
		static ComPtr<IWICImagingFactory> s_pFactory;
		if (s_pFactory == nullptr)
		{
			const HRESULT hrCom = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
			if (FAILED(hrCom) && hrCom != RPC_E_CHANGED_MODE)
			{
				return nullptr;
			}
			if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(s_pFactory.GetAddressOf()))))
			{
				return nullptr;
			}
		}
		return s_pFactory.Get();
	}
}

bool LoadImagePixels(const TCHAR* pPath, const D3DCOLOR ColorKey,
	UINT& rWidth, UINT& rHeight, vector<BYTE>& rPixels)
{
	IWICImagingFactory* const pFactory = GetWicFactory();
	if (pFactory == nullptr)
	{
		return false;
	}

	// 파일 → 첫 프레임 → 32비트 BGRA 로 변환
	ComPtr<IWICBitmapDecoder> pDecoder;
	if (FAILED(pFactory->CreateDecoderFromFilename(pPath, nullptr, GENERIC_READ,
		WICDecodeMetadataCacheOnDemand, pDecoder.GetAddressOf())))
	{
		return false;
	}
	ComPtr<IWICBitmapFrameDecode> pFrame;
	if (FAILED(pDecoder->GetFrame(0, pFrame.GetAddressOf())))
	{
		return false;
	}
	ComPtr<IWICFormatConverter> pConverter;
	if (FAILED(pFactory->CreateFormatConverter(pConverter.GetAddressOf()))
		|| FAILED(pConverter->Initialize(pFrame.Get(), GUID_WICPixelFormat32bppBGRA,
			WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom)))
	{
		return false;
	}
	if (FAILED(pConverter->GetSize(&rWidth, &rHeight)) || rWidth == 0 || rHeight == 0)
	{
		return false;
	}
	const UINT uStride = rWidth * 4;
	rPixels.resize(static_cast<size_t>(uStride) * rHeight);
	if (FAILED(pConverter->CopyPixels(nullptr, uStride, static_cast<UINT>(rPixels.size()), rPixels.data())))
	{
		return false;
	}

	// 컬러키: D3DCOLOR 는 0xAARRGGBB, 픽셀은 메모리에 B·G·R·A 순서
	const BYTE byKeyB = static_cast<BYTE>(ColorKey & 0xFF);
	const BYTE byKeyG = static_cast<BYTE>((ColorKey >> 8) & 0xFF);
	const BYTE byKeyR = static_cast<BYTE>((ColorKey >> 16) & 0xFF);
	const BYTE byKeyA = static_cast<BYTE>((ColorKey >> 24) & 0xFF);
	for (size_t i = 0; i + 3 < rPixels.size(); i += 4)
	{
		if (rPixels[i] == byKeyB && rPixels[i + 1] == byKeyG && rPixels[i + 2] == byKeyR && rPixels[i + 3] == byKeyA)
		{
			rPixels[i] = 0;
			rPixels[i + 1] = 0;
			rPixels[i + 2] = 0;
			rPixels[i + 3] = 0;
		}
	}
	return true;
}
