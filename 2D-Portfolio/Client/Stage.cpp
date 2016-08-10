#include "StdAfx.h"
#include "Stage.h"
#include "Include.h"

D3DXVECTOR3 CStage::g_tScroll = D3DXVECTOR3(0.f, 0.f, 0.f);

CStage::CStage(void)
{}

CStage::~CStage(void)
{
	Release();
}

HRESULT CStage::Initialize()
{
	TCHAR* Charter[] = {L"Player", 
		L"Fellow_First",
		L"Fellow_Second"};
	TCHAR* Monster[] = {L"Skeleton", 
		L"SandRaider", 
		L"Viper",
		L"Griswold",
		L"Summoner",
		L"Andariel",
		L"Izual",
		L"Diablo"
	};
	m_pPrototype = new CStageObjProto;
	if(FAILED(m_pPrototype->InitProtoInstance()))
	{
		ERR_MSG(g_hWnd, L"원형 객체 생성 실패");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"StageBack")))
	{
		ERR_MSG(g_hWnd, L"객체 생성 실패");
		return E_FAIL;
	}
	// 캐릭터
	for ( int i= 0 ; i < 3 ; ++i)
	{
		if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
			, Charter[i])))
		{
			ERR_MSG(g_hWnd, L"%d 객체 생성 실패", Charter[i]);
			return E_FAIL;
		}
	}

	// 몬스터
	for(int i = 0; i < 5; ++i)
	{
		if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
			, L"Skeleton")))
		{
			ERR_MSG(g_hWnd, L"%d 객체 생성 실패", L"Skeleton");
			return E_FAIL;
		}
	}

	// UI 객체 생성
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"0StatusBar")))
	{
		ERR_MSG(g_hWnd, L"객체 생성 실패");
		return E_FAIL;
	}
	// UI 이미지
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/0StatusBar.png"
		, L"0StatusBar", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"텍스쳐 읽어오기 실패");
		return E_FAIL;
	}

	// 타일 불러오기
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Tile/Tile%d.png",
		L"StageBack", TEXTYPE_MULTI, L"Tile", 38)))
	{
		ERR_MSG(g_hWnd, L"TileTexture 로드 실패");
		return E_FAIL;
	}



	// Player
#pragma region Player

	for (int i = 0 ; i < 3 ; ++i)
	{
#pragma region Stand

		//////////////////////////////////////////////////////////////////////
		//						 		Stand								//
		//////////////////////////////////////////////////////////////////////
		// 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/D/FieldStand_D_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_D", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand D 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 왼쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/LD/FieldStand_LD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_LD", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand LD 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 오른쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/RD/FieldStand_RD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_RD", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand RD 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/U/FieldStand_U_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_U", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand U 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 왼쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/LU/FieldStand_LU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_LU", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand LU 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 오른쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/RU/FieldStand_RU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_RU", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand RU 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 왼쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/L/FieldStand_L_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_L", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand L 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 오른쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/R/FieldStand_R_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_R", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand R 로드 실패" , Charter[i]);
			return E_FAIL;
		}
#pragma endregion Stand
#pragma region Walk
		//////////////////////////////////////////////////////////////////////
		//								Walk								//
		//////////////////////////////////////////////////////////////////////
		// 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/D/FieldWalk_D_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_D", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk D 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 왼쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/LD/FieldWalk_LD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_LD", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk LD 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 오른쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/RD/FieldWalk_RD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_RD", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk RD 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/U/FieldWalk_U_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_U", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk U 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 왼쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/LU/FieldWalk_LU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_LU", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk LU 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 오른쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/RU/FieldWalk_RU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_RU", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk RU 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 왼쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/L/FieldWalk_L_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_L", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk L 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 오른쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/R/FieldWalk_R_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_R", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk R 로드 실패" , Charter[i]);
			return E_FAIL;
		}
#pragma endregion Walk
#pragma region Run
		//////////////////////////////////////////////////////////////////////
		//								Run									//
		//////////////////////////////////////////////////////////////////////
		// 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/D/Run_D_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_D", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk D 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 왼쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/LD/Run_LD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_LD", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk LD 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 오른쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/RD/Run_RD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_RD", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk RD 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/U/Run_U_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_U", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk U 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 왼쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/LU/Run_LU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_LU", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk LU 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 오른쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/RU/Run_RU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_RU", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk RU 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 왼쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/L/Run_L_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_L", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk L 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 오른쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/R/Run_R_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_R", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk R 로드 실패" , Charter[i]);
			return E_FAIL;
		}
#pragma endregion Run
#pragma region Attack
		//////////////////////////////////////////////////////////////////////
		//								Attack								//
		//////////////////////////////////////////////////////////////////////
		// 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/D/Attack_D_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_D", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack D 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 왼쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/LD/Attack_LD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_LD", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk LD 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 오른쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/RD/Attack_RD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_RD", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk RD 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/U/Attack_U_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_U", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk U 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 왼쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/LU/Attack_LU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_LU", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk LU 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 오른쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/RU/Attack_RU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_RU", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk RU 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 왼쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/L/Attack_L_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_L", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk L 로드 실패" , Charter[i]);
			return E_FAIL;
		}

		// 오른쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/R/Attack_R_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_R", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk R 로드 실패" , Charter[i]);
			return E_FAIL;
		}

#pragma endregion Attack
	}

#pragma endregion Player
#pragma region Monster

	for (int i = 0 ; i < 1 ; ++i)
	{
#pragma region Stand
		//////////////////////////////////////////////////////////////////////
		//						 		Stand								//
		//////////////////////////////////////////////////////////////////////
		// 아래

		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/D/FieldStand_D_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_D", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand D 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 왼쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/LD/FieldStand_LD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_LD", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand LD 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 오른쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/RD/FieldStand_RD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_RD", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand RD 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/U/FieldStand_U_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_U", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand U 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 왼쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/LU/FieldStand_LU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_LU", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand LU 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 오른쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/RU/FieldStand_RU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_RU", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand RU 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 왼쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/L/FieldStand_L_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_L", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand L 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 오른쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/R/FieldStand_R_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_R", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand R 로드 실패" , Monster[i]);
			return E_FAIL;
		}
#pragma endregion Stand
#pragma region Run
		//////////////////////////////////////////////////////////////////////
		//								Run									//
		//////////////////////////////////////////////////////////////////////
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Run/D/Run_D_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Run_D", SKELETON_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Run D 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 왼쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Run/LD/Run_LD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Run_LD", SKELETON_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Run LD 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 오른쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Run/RD/Run_RD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Run_RD", SKELETON_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Run RD 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Run/U/Run_U_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Run_U", SKELETON_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Run U 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 왼쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Run/LU/Run_LU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Run_LU", SKELETON_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Run LU 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 오른쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Run/RU/Run_RU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Run_RU", SKELETON_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Run RU 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 왼쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Run/L/Run_L_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Run_L", SKELETON_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Run L 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 오른쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Run/R/Run_R_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Run_R", SKELETON_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Run R 로드 실패" , Monster[i]);
			return E_FAIL;
		}
#pragma endregion Run
#pragma region Attack
		//////////////////////////////////////////////////////////////////////
		//								Attack								//
		//////////////////////////////////////////////////////////////////////
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Attack/D/Attack_D_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Attack_D", SKELETON_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack D 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 왼쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Attack/LD/Attack_LD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Attack_LD", SKELETON_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack LD 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 오른쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Attack/RD/Attack_RD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Attack_RD", SKELETON_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack RD 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Attack/U/Attack_U_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Attack_U", SKELETON_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack U 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 왼쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Attack/LU/Attack_LU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Attack_LU", SKELETON_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack LU 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 오른쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Attack/RU/Attack_RU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Attack_RU", SKELETON_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack RU 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 왼쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Attack/L/Attack_L_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Attack_L", SKELETON_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack L 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 오른쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Attack/R/Attack_R_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Attack_R", SKELETON_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack R 로드 실패" , Monster[i]);
			return E_FAIL;
		}
#pragma endregion Attack
#pragma region Death
		//////////////////////////////////////////////////////////////////////
		//								Death								//
		//////////////////////////////////////////////////////////////////////
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Death/D/Death_D_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Death_D", SKELETON_DEATH)))
		{
			ERR_MSG(g_hWnd, L"%d Death D 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 왼쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Death/LD/Death_LD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Death_LD", SKELETON_DEATH)))
		{
			ERR_MSG(g_hWnd, L"%d Death LD 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 오른쪽 아래
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Death/RD/Death_RD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Death_RD", SKELETON_DEATH)))
		{
			ERR_MSG(g_hWnd, L"%d Death RD 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Death/U/Death_U_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Death_U", SKELETON_DEATH)))
		{
			ERR_MSG(g_hWnd, L"%d Death U 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 왼쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Death/LU/Death_LU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Death_LU", SKELETON_DEATH)))
		{
			ERR_MSG(g_hWnd, L"%d Death LU 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 오른쪽 위
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Death/RU/Death_RU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Death_RU", SKELETON_DEATH)))
		{
			ERR_MSG(g_hWnd, L"%d Death RU 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 왼쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Death/L/Death_L_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Death_L", SKELETON_DEATH)))
		{
			ERR_MSG(g_hWnd, L"%d Death L 로드 실패" , Monster[i]);
			return E_FAIL;
		}

		// 오른쪽
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Death/R/Death_R_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Death_R", SKELETON_DEATH)))
		{
			ERR_MSG(g_hWnd, L"%d Death R 로드 실패" , Monster[i]);
			return E_FAIL;
		}
#pragma endregion Death
	}
#pragma endregion Monster
	return S_OK;
}

SCENEID CStage::Progress()
{
	TCHAR* Charter[3] = {L"Player", 
		L"Fellow_First",
		L"Fellow_Second"};

	SCENEID iScene = GET_SINGLE(CObjMgr)->Progress();


	// 씬ID가 NONPASS가 아니면 씬매니져에서 해당ID의 씬을 생성 및 초기화
	if(iScene > SCENEID_NONPASS)
		GET_SINGLE(CSceneMgr)->InitScene((SCENEID)iScene);
	if (iScene == SCENEID_END)
		return SCENEID_END;
}

void CStage::Render()
{
	GET_SINGLE(CObjMgr)->Render();
}

void CStage::Release()
{
	DESTROY_SINGLE(CTextureMgr);
	GET_SINGLE(CObjMgr)->DestroyInstance();
	SAFE_DELETE<CPrototype>(&m_pPrototype);
}
