#pragma once

template <typename T>
void SAFE_DELETE(T** pObj)
{
	if(*pObj != nullptr)
	{
		delete *pObj;
		*pObj = nullptr;
	}
}