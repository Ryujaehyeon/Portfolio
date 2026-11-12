#pragma once

#include "Defines.h"

class CObj;

// -log 로 실행했을 때 객체 상태의 변화를 state.log 에 남긴다(intent/state-log).
// 프레임마다 CObjMgr 의 모든 객체를 공개 접근자로 스냅숏하고 직전 스냅숏과 비교해 바뀐 것만 쓴다.
// 꺼져 있으면 모든 함수가 즉시 반환한다. 저장한 객체 포인터는 비교 키로만 쓰고 역참조하지 않는다.
class CStateRecorder
{
	SINGLETON(CStateRecorder);
private:
	// 한 객체의 한 프레임 상태(값 복사 — 원본 포인터 수명에 기대지 않는다)
	struct OBJSTATE
	{
		int iId;
		wstring strKey;
		wstring strName;
		wstring strMotion;
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vLoggedPos;
		float fHP;
		const CObj* pTarget;
		bool bCrash;
		bool bSelect;

		OBJSTATE()
			: iId(0)
			, vPos(0.f, 0.f, 0.f)
			, vLoggedPos(0.f, 0.f, 0.f)
			, fHP(0.f)
			, pTarget(nullptr)
			, bCrash(false)
			, bSelect(false)
		{
		}
	};
	typedef map<const CObj*, OBJSTATE> MAPSTATE;
private:
	HANDLE m_hFile;
	ULONGLONG m_ullStart;
	DWORD m_dwFrame;
	int m_iNextId;
	MAPSTATE m_mapPrev;
public:
	// 기록을 켜고 로그 파일을 새로 만든다. 실패하면 false(꺼진 채).
	bool Begin(const TCHAR* const _pLogPath);
	// 프레임마다 장면 갱신 직후 1번 — 바뀐 상태만 쓴다
	void Record();
	// 장면 교체 — 장면 줄을 쓰고 직전 스냅숏을 비운다
	void OnScene(const SCENEID _eScene);
private:
	bool IsOn() const { return m_hFile != INVALID_HANDLE_VALUE; }
	void WriteLine(const TCHAR* const _pFormat, ...);
	void WriteChanges(OBJSTATE& _rCur, const OBJSTATE& _rPrev, const MAPSTATE& _mapCur);
	int FindId(const CObj* const _pObj, const MAPSTATE& _mapCur) const;
	static OBJSTATE Capture(const CObj* const _pObj, const wstring& _strKey);
private:
	CStateRecorder(void);
public:
	~CStateRecorder(void);
};
