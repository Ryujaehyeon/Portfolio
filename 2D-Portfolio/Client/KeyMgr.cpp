#include "StdAfx.h"
#include "KeyMgr.h"
#include "Include.h"

CKeyMgr::CKeyMgr(void)
	: m_dwKey(0)
{
}


CKeyMgr::~CKeyMgr(void)
{
}

void CKeyMgr::CheckKey()
{
		m_dwKey = 0;
		if (GameAsyncKeyState(VK_RBUTTON) & 0x8000)
			m_dwKey |= KEY_RBUTTON;
		if (GameAsyncKeyState(VK_LBUTTON) & 0x8000)
			m_dwKey |= KEY_LBUTTON;
		if (GameAsyncKeyState(VK_LEFT) & 0x8000)
			m_dwKey |= KEY_LEFT;
		if (GameAsyncKeyState(VK_RIGHT) & 0x8000)
			m_dwKey |= KEY_RIGHT;
		if (GameAsyncKeyState(VK_UP) & 0x8000)
			m_dwKey |= KEY_UP;
		if (GameAsyncKeyState(VK_DOWN) & 0x8000)
			m_dwKey |= KEY_DOWN;
		if (GameAsyncKeyState(VK_F1) & 0x8000)
			m_dwKey |= KEY_F1;
		if (GameAsyncKeyState(VK_F2) & 0x8000)
			m_dwKey |= KEY_F2;
		if (GameAsyncKeyState(VK_F3) & 0x8000)
			m_dwKey |= KEY_F3;
		if (GameAsyncKeyState(VK_F4) & 0x8000)
			m_dwKey |= KEY_F4;
		if (GameAsyncKeyState('I') & 0x8000)
			m_dwKey |= KEY_I;
		if (GameAsyncKeyState('B') & 0x8000)
			m_dwKey |= KEY_B;
		if (GameAsyncKeyState('C') & 0x8000)
			m_dwKey |= KEY_C;
		if (GameAsyncKeyState('K') & 0x8000)
			m_dwKey |= KEY_K;
		if (GameAsyncKeyState(VK_TAB) & 0x8000)
			m_dwKey |= KEY_TAB;
		if (GameAsyncKeyState(VK_SPACE) & 0x8000)
			m_dwKey |= KEY_SPACE;
		if (GameAsyncKeyState('1') & 0x8000)
			m_dwKey |= KEY_1;
		if (GameAsyncKeyState('2') & 0x8000)
			m_dwKey |= KEY_2;
		if (GameAsyncKeyState('3') & 0x8000)
			m_dwKey |= KEY_3;
		if (GameAsyncKeyState(VK_ESCAPE) & 0x8000)
			m_dwKey |= KEY_ESC;
}

DWORD CKeyMgr::PushKey(DWORD dwKey)
{
	return m_dwKey & dwKey;
}
const DWORD CKeyMgr::GetKey() const
{
	return m_dwKey;
}