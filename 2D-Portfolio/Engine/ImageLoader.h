#pragma once

#include "Defines.h"

// 이미지 파일(png·bmp·jpg)을 WIC 로 읽어 32비트 BGRA 픽셀(한 줄 = 너비 × 4바이트, 위에서 아래로)로 돌려준다.
// 알파가 255 이고 RGB 가 ColorKey 와 같은 픽셀은 완전 투명(0,0,0,0)으로 바꾼다 — D3DXCreateTextureFromFileEx 의 컬러키와 같은 결과.
// D3D 를 쓰지 않는 CPU 함수라 D3D11 백엔드(DirectXTK 판·직접 구현 판)가 함께 쓴다(intent/render-dxtk).
bool LoadImagePixels(const TCHAR* pPath, const D3DCOLOR ColorKey,
	UINT& rWidth, UINT& rHeight, vector<BYTE>& rPixels);
