// Engine — Client·툴(MFC_000) 공용 렌더링 정적 라이브러리의 미리 컴파일된 헤더.
// 그리기에 필요한 것만 둔다(앱 전용 헤더·MFC·DirectInput 은 넣지 않는다, intent/shared-render-lib).
#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#include <cmath>
#include <cstdlib>
#include <vector>
#include <list>
#include <map>
#include <string>

#include <d3d9.h>
#include <d3dx9.h>

using namespace std;
