#include "StdAfx.h"
#include "StateRecorder.h"
#include "Include.h"

namespace
{
	// 이 거리(px) 이상 움직였을 때만 위치 줄을 쓴다 — 타일 크기 정도
	constexpr float POS_LOG_DISTANCE = 32.f;

	const TCHAR* TextOrEmpty(const TCHAR* const _pText)
	{
		return (_pText != nullptr) ? _pText : L"";
	}
}

CStateRecorder::CStateRecorder(void)
	: m_hFile(INVALID_HANDLE_VALUE)
	, m_ullStart(0)
	, m_dwFrame(0)
	, m_iNextId(1)
{
}

CStateRecorder::~CStateRecorder(void)
{
	if (IsOn())
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

bool CStateRecorder::Begin(const TCHAR* const _pLogPath)
{
	m_hFile = CreateFile(_pLogPath, GENERIC_WRITE, FILE_SHARE_READ, nullptr,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (!IsOn())
	{
		return false;
	}
	m_ullStart = GetTickCount64();
	WriteLine(L"state log begin");
	return true;
}

void CStateRecorder::OnScene(const SCENEID _eScene)
{
	if (!IsOn())
	{
		return;
	}
	WriteLine(L"scene %d (이전 장면 객체 %zu 개 기록 종료)", static_cast<int>(_eScene), m_mapPrev.size());
	m_mapPrev.clear();
}

void CStateRecorder::Record()
{
	if (!IsOn())
	{
		return;
	}
	++m_dwFrame;

	// 1. 이번 프레임 스냅숏 — 처음 본 객체는 id 를 붙이고 생성 줄을 쓴다
	MAPSTATE mapCur;
	vector<const CObj*> vecOrder;	// 줄 순서를 CObjMgr 순서로 맞춘다(주소 순서가 아니라)
	for (const auto& rPair : GET_SINGLE(CObjMgr)->GetObjectMap())
	{
		for (const CObj* const pObj : rPair.second)
		{
			if (pObj == nullptr)
			{
				continue;
			}
			OBJSTATE tCur = Capture(pObj, rPair.first);
			const MAPSTATE::const_iterator iterPrev = m_mapPrev.find(pObj);
			if (iterPrev != m_mapPrev.end() && iterPrev->second.strName == tCur.strName)
			{
				tCur.iId = iterPrev->second.iId;
				tCur.vLoggedPos = iterPrev->second.vLoggedPos;
			}
			else
			{
				// 같은 주소에 이름이 다른 객체 — 해제 뒤 재사용으로 보고 삭제 + 생성
				if (iterPrev != m_mapPrev.end())
				{
					WriteLine(L"- #%d %s", iterPrev->second.iId, iterPrev->second.strName.c_str());
				}
				tCur.iId = m_iNextId++;
				tCur.vLoggedPos = tCur.vPos;
				WriteLine(L"+ #%d %s (%s) @(%.0f,%.0f) hp %.0f motion %s", tCur.iId, tCur.strName.c_str(),
					tCur.strKey.c_str(), tCur.vPos.x, tCur.vPos.y, tCur.fHP, tCur.strMotion.c_str());
			}
			mapCur[pObj] = tCur;
			vecOrder.push_back(pObj);
		}
	}

	// 2. 이어진 객체의 변화
	for (const CObj* const pObj : vecOrder)
	{
		OBJSTATE& rCur = mapCur[pObj];
		const MAPSTATE::const_iterator iterPrev = m_mapPrev.find(pObj);
		if (iterPrev != m_mapPrev.end() && iterPrev->second.iId == rCur.iId)
		{
			WriteChanges(rCur, iterPrev->second, mapCur);
		}
	}

	// 3. 사라진 객체
	for (const auto& rPrev : m_mapPrev)
	{
		const MAPSTATE::const_iterator iterCur = mapCur.find(rPrev.first);
		if (iterCur == mapCur.end())
		{
			WriteLine(L"- #%d %s @(%.0f,%.0f)", rPrev.second.iId, rPrev.second.strName.c_str(),
				rPrev.second.vPos.x, rPrev.second.vPos.y);
		}
	}

	m_mapPrev.swap(mapCur);
}

void CStateRecorder::WriteChanges(OBJSTATE& _rCur, const OBJSTATE& _rPrev, const MAPSTATE& _mapCur)
{
	const int iId = _rCur.iId;
	const TCHAR* const pName = _rCur.strName.c_str();
	if (_rCur.strMotion != _rPrev.strMotion)
	{
		WriteLine(L"#%d %s motion %s → %s", iId, pName, _rPrev.strMotion.c_str(), _rCur.strMotion.c_str());
	}
	if (_rCur.fHP != _rPrev.fHP)
	{
		WriteLine(L"#%d %s hp %.1f → %.1f", iId, pName, _rPrev.fHP, _rCur.fHP);
	}
	if (_rCur.pTarget != _rPrev.pTarget)
	{
		WriteLine(L"#%d %s target #%d → #%d", iId, pName,
			FindId(_rPrev.pTarget, _mapCur), FindId(_rCur.pTarget, _mapCur));
	}
	if (_rCur.bCrash != _rPrev.bCrash)
	{
		WriteLine(L"#%d %s crash %d → %d", iId, pName, _rPrev.bCrash, _rCur.bCrash);
	}
	if (_rCur.bSelect != _rPrev.bSelect)
	{
		WriteLine(L"#%d %s select %d → %d", iId, pName, _rPrev.bSelect, _rCur.bSelect);
	}
	const D3DXVECTOR3 vMove = _rCur.vPos - _rCur.vLoggedPos;
	if (D3DXVec3Length(&vMove) >= POS_LOG_DISTANCE)
	{
		WriteLine(L"#%d %s pos (%.0f,%.0f)", iId, pName, _rCur.vPos.x, _rCur.vPos.y);
		_rCur.vLoggedPos = _rCur.vPos;
	}
}

// 대상 객체의 id — 없으면 0, 기록에 없는 객체면 -1
int CStateRecorder::FindId(const CObj* const _pObj, const MAPSTATE& _mapCur) const
{
	if (_pObj == nullptr)
	{
		return 0;
	}
	const MAPSTATE::const_iterator iterCur = _mapCur.find(_pObj);
	if (iterCur != _mapCur.end())
	{
		return iterCur->second.iId;
	}
	const MAPSTATE::const_iterator iterPrev = m_mapPrev.find(_pObj);
	if (iterPrev != m_mapPrev.end())
	{
		return iterPrev->second.iId;
	}
	return -1;
}

CStateRecorder::OBJSTATE CStateRecorder::Capture(const CObj* const _pObj, const wstring& _strKey)
{
	OBJSTATE tState;
	tState.strKey = _strKey;
	tState.strName = TextOrEmpty(_pObj->GetName());
	tState.strMotion = TextOrEmpty(_pObj->GetpMotion());
	tState.vPos = _pObj->GetInfo().vPos;
	tState.fHP = _pObj->GetStatas().fHealthPoint;
	tState.pTarget = _pObj->GetTagetObj();
	tState.bCrash = _pObj->GetCrash();
	tState.bSelect = _pObj->GetSelect();
	return tState;
}

// "[  경과 ms #프레임] 내용" 한 줄을 UTF-8 로 바로 쓴다(버퍼 없음 — 크래시해도 마지막 줄까지 남는다)
void CStateRecorder::WriteLine(const TCHAR* const _pFormat, ...)
{
	TCHAR szMsg[512];
	va_list ap;
	va_start(ap, _pFormat);
	_vsnwprintf_s(szMsg, _TRUNCATE, _pFormat, ap);
	va_end(ap);

	TCHAR szLine[600];
	_snwprintf_s(szLine, _TRUNCATE, L"[%7llu ms #%6lu] %s\n", GetTickCount64() - m_ullStart, m_dwFrame, szMsg);

	char szUtf8[1800];
	const int iLen = WideCharToMultiByte(CP_UTF8, 0, szLine, -1, szUtf8, sizeof(szUtf8), nullptr, nullptr);
	if (iLen > 1)
	{
		DWORD dwWritten = 0;
		WriteFile(m_hFile, szUtf8, static_cast<DWORD>(iLen - 1), &dwWritten, nullptr);
	}
}
