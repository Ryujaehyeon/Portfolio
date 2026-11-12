// Page2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFC_000.h"
#include "Page2.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MFC_000View.h"

// CPage2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPage2, CPropertyPage)

CPage2::CPage2()
	: CPropertyPage(CPage2::IDD)
{

}

CPage2::~CPage2()
{
}

void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPage2, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage2::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage2::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPage2::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPage2::OnBnClickedButton4)
END_MESSAGE_MAP()


// CPage2 메시지 처리기입니다.


void CPage2::OnBnClickedButton1()
{
	// 타일 저장하기
	CFileDialog Dlg(FALSE, L"dat", L"*.dat"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	HANDLE hFile;
	DWORD dwByte;

	hFile = CreateFile(Dlg.GetPathName(),
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	vector<TILE*>* pTile = 
		&((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_vecTile;
	int size = static_cast<int>(pTile->size());
	for(int i = 0; i < size; ++i)
	{
		WriteFile(hFile, (*pTile)[i]
		, sizeof(TILE), &dwByte, 0);
	}
	CloseHandle(hFile);
}


int ReadTileFile(const TCHAR* const _pPath, vector<TILE*>& _rTiles)
{
	const HANDLE hFile = CreateFile(_pPath,
		GENERIC_READ, 0, nullptr, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	int iCount = 0;
	DWORD dwByte;
	while(true)
	{
		TILE* pInfo = new TILE;
		ZeroMemory(pInfo, sizeof(TILE));
		ReadFile(hFile, pInfo, sizeof(TILE)
			, &dwByte, nullptr);

		if(dwByte == 0)
		{
			delete pInfo;
			break;
		}

		_rTiles.push_back(pInfo);
		++iCount;
	}

	CloseHandle(hFile);
	return iCount;
}

void CPage2::OnBnClickedButton2()
{
	// 타일 불러오기
	CFileDialog Dlg(TRUE, L"dat", L"*.dat"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;
	vector<TILE*>* pTile = 
		&((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_vecTile;
	int size = static_cast<int>(pTile->size());
	for(int i = 0; i < size; ++i)
	{
		SAFE_DELETE<TILE>(&(*pTile)[i]);
	}
	pTile->clear();

	ReadTileFile(Dlg.GetPathName(), *pTile);
	((CMainFrame*)AfxGetMainWnd()
		)->m_pMainView->Invalidate(FALSE);
}


void CPage2::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 몬스터 저장하기
	CFileDialog Dlg(FALSE, L"dat", L"*.dat"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	HANDLE hFile;
	DWORD dwByte;

	hFile = CreateFile(Dlg.GetPathName(),
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	vector<OBJINFO*>* pMonster = 
		&((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_vecMonster;

	int size = static_cast<int>(pMonster->size());
	for(int i = 0; i < size; ++i)
	{
		if((*pMonster)[i] != NULL)
			WriteFile(hFile, (*pMonster)[i]	, sizeof(OBJINFO), &dwByte, 0);
	}
	CloseHandle(hFile);
}


void CPage2::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 몬스터 불러오기
	CFileDialog Dlg(TRUE, L"dat", L"*.dat"
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;
	vector<OBJINFO*>* pMonster = 
		&((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_vecMonster;
	int size = static_cast<int>(pMonster->size());
	for(int i = 0; i < size; ++i)
	{
		SAFE_DELETE<OBJINFO>(&(*pMonster)[i]);
	}
	pMonster->clear();

	HANDLE hFile;
	DWORD dwByte;

	hFile = CreateFile(Dlg.GetPathName(),
		GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	pMonster->resize(TILECNTX * TILECNTY);

	for(int i = 0; i < TILECNTY; ++i)
	{
		for(int j = 0; j < TILECNTX; ++j)
		{
			int iIdx = i * TILECNTX + j;

			OBJINFO* pInfo = new OBJINFO;
			ZeroMemory(pInfo, sizeof(OBJINFO));
			ReadFile(hFile, pInfo, sizeof(OBJINFO)
				, &dwByte, NULL);

			if(dwByte == 0)
			{
				delete pInfo;
				pInfo = NULL;
				break;
			}
			int vecNum = static_cast<int>((pInfo->vPos.x / TILESIZEX) + (pInfo->vPos.y / (TILESIZEY * 0.5f)) * TILECNTX);
			memcpy(&(*pMonster)[vecNum], &pInfo, sizeof(pInfo));
		}
	}

	CloseHandle(hFile);
	((CMainFrame*)AfxGetMainWnd()
		)->m_pMainView->Invalidate(FALSE);
}
