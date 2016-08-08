#pragma once

#define SINGLETON(type) public:\
	static type** GetInstance()\
	{\
		static type* pInstance = new type;\
		if(pInstance == 0)\
			pInstance = new type;\
		return &pInstance;\
	}\
	static void DestroyInstance()\
	{\
		type** ppInstance = GetInstance();\
		if(*ppInstance != 0)\
		{\
			delete *ppInstance;\
			*ppInstance = 0;\
		}\
	}

#define GET_SINGLE(type) (*type::GetInstance())
#define ERR_MSG(hwnd, sz) MessageBox(hwnd, sz, L"system err", MB_OK)
#define DESTROY_SINGLE(type) (*type::GetInstance())->DestroyInstance()

