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
		ERR_MSG(g_hWnd, L"���� ��ü ���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"StageBack")))
	{
		ERR_MSG(g_hWnd, L"��ü ���� ����");
		return E_FAIL;
	}
	// ĳ����
	for ( int i= 0 ; i < 3 ; ++i)
	{
		if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
			, Charter[i])))
		{
			ERR_MSG(g_hWnd, L"%d ��ü ���� ����", Charter[i]);
			return E_FAIL;
		}
	}

	// ����
	for(int i = 0; i < 5; ++i)
	{
		if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
			, L"Skeleton")))
		{
			ERR_MSG(g_hWnd, L"%d ��ü ���� ����", L"Skeleton");
			return E_FAIL;
		}
	}

	// UI ��ü ����
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"0StatusBar")))
	{
		ERR_MSG(g_hWnd, L"��ü ���� ����");
		return E_FAIL;
	}
	// UI �̹���
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/0StatusBar.png"
		, L"0StatusBar", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"�ؽ��� �о���� ����");
		return E_FAIL;
	}

	// Ÿ�� �ҷ�����
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/Tile/Tile%d.png",
		L"StageBack", TEXTYPE_MULTI, L"Tile", 38)))
	{
		ERR_MSG(g_hWnd, L"TileTexture �ε� ����");
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
		// �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/D/FieldStand_D_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_D", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand D �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ���� �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/LD/FieldStand_LD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_LD", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand LD �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ������ �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/RD/FieldStand_RD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_RD", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand RD �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/U/FieldStand_U_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_U", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand U �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ���� ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/LU/FieldStand_LU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_LU", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand LU �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ������ ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/RU/FieldStand_RU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_RU", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand RU �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ����
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/L/FieldStand_L_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_L", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand L �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ������
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldStand/R/FieldStand_R_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldStand_R", PLAYER_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand R �ε� ����" , Charter[i]);
			return E_FAIL;
		}
#pragma endregion Stand
#pragma region Walk
		//////////////////////////////////////////////////////////////////////
		//								Walk								//
		//////////////////////////////////////////////////////////////////////
		// �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/D/FieldWalk_D_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_D", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk D �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ���� �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/LD/FieldWalk_LD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_LD", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk LD �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ������ �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/RD/FieldWalk_RD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_RD", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk RD �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/U/FieldWalk_U_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_U", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk U �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ���� ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/LU/FieldWalk_LU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_LU", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk LU �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ������ ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/RU/FieldWalk_RU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_RU", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk RU �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ����
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/L/FieldWalk_L_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_L", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk L �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ������
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/FieldWalk/R/FieldWalk_R_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"FieldWalk_R", PLAYER_WALK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk R �ε� ����" , Charter[i]);
			return E_FAIL;
		}
#pragma endregion Walk
#pragma region Run
		//////////////////////////////////////////////////////////////////////
		//								Run									//
		//////////////////////////////////////////////////////////////////////
		// �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/D/Run_D_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_D", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk D �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ���� �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/LD/Run_LD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_LD", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk LD �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ������ �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/RD/Run_RD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_RD", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk RD �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/U/Run_U_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_U", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk U �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ���� ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/LU/Run_LU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_LU", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk LU �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ������ ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/RU/Run_RU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_RU", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk RU �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ����
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/L/Run_L_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_L", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk L �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ������
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Run/R/Run_R_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Run_R", PLAYER_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Walk R �ε� ����" , Charter[i]);
			return E_FAIL;
		}
#pragma endregion Run
#pragma region Attack
		//////////////////////////////////////////////////////////////////////
		//								Attack								//
		//////////////////////////////////////////////////////////////////////
		// �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/D/Attack_D_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_D", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack D �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ���� �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/LD/Attack_LD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_LD", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk LD �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ������ �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/RD/Attack_RD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_RD", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk RD �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/U/Attack_U_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_U", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk U �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ���� ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/LU/Attack_LU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_LU", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk LU �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ������ ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/RU/Attack_RU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_RU", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk RU �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ����
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/L/Attack_L_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_L", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk L �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ������
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Attack/R/Attack_R_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Attack_R", PLAYER_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Walk R �ε� ����" , Charter[i]);
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
		// �Ʒ�

		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/D/FieldStand_D_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_D", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand D �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ���� �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/LD/FieldStand_LD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_LD", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand LD �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ������ �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/RD/FieldStand_RD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_RD", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand RD �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/U/FieldStand_U_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_U", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand U �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ���� ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/LU/FieldStand_LU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_LU", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand LU �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ������ ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/RU/FieldStand_RU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_RU", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand RU �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ����
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/L/FieldStand_L_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_L", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand L �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ������
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/FieldStand/R/FieldStand_R_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"FieldStand_R", SKELETON_STAND)))
		{
			ERR_MSG(g_hWnd, L"%d Stand R �ε� ����" , Monster[i]);
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
			ERR_MSG(g_hWnd, L"%d Run D �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ���� �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Run/LD/Run_LD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Run_LD", SKELETON_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Run LD �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ������ �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Run/RD/Run_RD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Run_RD", SKELETON_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Run RD �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Run/U/Run_U_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Run_U", SKELETON_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Run U �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ���� ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Run/LU/Run_LU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Run_LU", SKELETON_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Run LU �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ������ ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Run/RU/Run_RU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Run_RU", SKELETON_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Run RU �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ����
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Run/L/Run_L_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Run_L", SKELETON_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Run L �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ������
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Run/R/Run_R_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Run_R", SKELETON_RUN)))
		{
			ERR_MSG(g_hWnd, L"%d Run R �ε� ����" , Monster[i]);
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
			ERR_MSG(g_hWnd, L"%d Attack D �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ���� �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Attack/LD/Attack_LD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Attack_LD", SKELETON_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack LD �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ������ �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Attack/RD/Attack_RD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Attack_RD", SKELETON_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack RD �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Attack/U/Attack_U_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Attack_U", SKELETON_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack U �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ���� ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Attack/LU/Attack_LU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Attack_LU", SKELETON_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack LU �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ������ ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Attack/RU/Attack_RU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Attack_RU", SKELETON_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack RU �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ����
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Attack/L/Attack_L_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Attack_L", SKELETON_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack L �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ������
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Attack/R/Attack_R_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Attack_R", SKELETON_ATTACK)))
		{
			ERR_MSG(g_hWnd, L"%d Attack R �ε� ����" , Monster[i]);
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
			ERR_MSG(g_hWnd, L"%d Death D �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ���� �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Death/LD/Death_LD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Death_LD", SKELETON_DEATH)))
		{
			ERR_MSG(g_hWnd, L"%d Death LD �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ������ �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Death/RD/Death_RD_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Death_RD", SKELETON_DEATH)))
		{
			ERR_MSG(g_hWnd, L"%d Death RD �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Death/U/Death_U_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Death_U", SKELETON_DEATH)))
		{
			ERR_MSG(g_hWnd, L"%d Death U �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ���� ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Death/LU/Death_LU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Death_LU", SKELETON_DEATH)))
		{
			ERR_MSG(g_hWnd, L"%d Death LU �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ������ ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Death/RU/Death_RU_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Death_RU", SKELETON_DEATH)))
		{
			ERR_MSG(g_hWnd, L"%d Death RU �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ����
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Death/L/Death_L_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Death_L", SKELETON_DEATH)))
		{
			ERR_MSG(g_hWnd, L"%d Death L �ε� ����" , Monster[i]);
			return E_FAIL;
		}

		// ������
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Monster/Skeleton/Death/R/Death_R_%02d.png",
			Monster[i], TEXTYPE_MULTI, L"Death_R", SKELETON_DEATH)))
		{
			ERR_MSG(g_hWnd, L"%d Death R �ε� ����" , Monster[i]);
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


	// ��ID�� NONPASS�� �ƴϸ� ���Ŵ������� �ش�ID�� ���� ���� �� �ʱ�ȭ
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
