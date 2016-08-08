#pragma once

template <typename T>
void SAFE_DELETE(T** pObj)
{
	if(*pObj != 0)
	{
		delete *pObj;
		*pObj = 0;
	}
}