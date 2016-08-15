#include "StdAfx.h"
#include "KeyMgr.h"
#include "Include.h"

CKeyMgr::CKeyMgr(void)
{
}


CKeyMgr::~CKeyMgr(void)
{
}

void CKeyMgr::CheckKey()
{
		m_dwKey = 0;
		if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
			m_dwKey |= KEY_RBUTTON;
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			m_dwKey |= KEY_LBUTTON;
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			m_dwKey |= KEY_LEFT;
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			m_dwKey |= KEY_RIGHT;
		if (GetAsyncKeyState(VK_UP) & 0x8000)
			m_dwKey |= KEY_UP;
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			m_dwKey |= KEY_DOWN;
		if (GetAsyncKeyState(VK_F1) & 0x8000)
			m_dwKey |= KEY_F1;
		if (GetAsyncKeyState(VK_F2) & 0x8000)
			m_dwKey |= KEY_F2;
		if (GetAsyncKeyState(VK_F3) & 0x8000)
			m_dwKey |= KEY_F3;
		if (GetAsyncKeyState(VK_F4) & 0x8000)
			m_dwKey |= KEY_F4;
		if (GetAsyncKeyState('I') & 0x8000)
			m_dwKey |= KEY_I;
		if (GetAsyncKeyState('B') & 0x8000)
			m_dwKey |= KEY_B;
		if (GetAsyncKeyState('C') & 0x8000)
			m_dwKey |= KEY_C;
		if (GetAsyncKeyState('K') & 0x8000)
			m_dwKey |= KEY_K;
		if (GetAsyncKeyState(VK_TAB) & 0x8000)
			m_dwKey |= KEY_TAB;
}

DWORD CKeyMgr::PushKey(DWORD dwKey)
{
	return m_dwKey & dwKey;
}
const DWORD CKeyMgr::GetKey() const
{
	return m_dwKey;
}