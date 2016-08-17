#include "StdAfx.h"
#include "Stage.h"
#include "Include.h"



CStage::CStage(void)
{
	GameSound Sound;
	Sound->Stop(L"Wilderness");
	Sound->LoadSound(L"Tristram", L"Tristram.mp3", true);
	Sound->SetVolume(10);
	Sound->Play(L"Tristram");       //����
}

CStage::~CStage(void)
{
	Release();
}

HRESULT CStage::Initialize()
{
	int PlayerCount = 1;
	int MonsterCount = 5;
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
	m_pSkillPrototype = new CSkillProto;

	if(FAILED(m_pPrototype->InitProtoInstance()))
	{
		ERR_MSG(g_hWnd, L"���� ��ü ���� ����");
		return E_FAIL;
	}


#pragma region StageProto
#pragma region AddObj


	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"StageBack")))
	{
		ERR_MSG(g_hWnd, L"��ü ���� ����");
		return E_FAIL;
	}
	// ĳ����
	for ( int i= 0 ; i < PlayerCount; ++i)
	{
		if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
			, Charter[i])))
		{
			ERR_MSG(g_hWnd, L"%s ĳ���� ��ü ���� ����", Charter[i]);
			return E_FAIL;
		}
	}

	// ����
	for(int i = 0; i < MonsterCount; ++i)
	{
		if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
			, Monster[0])))
		{
			ERR_MSG(g_hWnd, L"%s ���� ��ü ���� ����", Monster[0]);
			return E_FAIL;
		}
	}



	///////////////////////////////////////////////////////////////
	// UI ��ü ����

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"StatusBar")))
	{
		ERR_MSG(g_hWnd, L"StatusBar ��ü ���� ����");
		return E_FAIL;
	}
	// HP ��ü ����
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"HP")))
	{
		ERR_MSG(g_hWnd, L"HP ��ü ���� ����");
		return E_FAIL;
	}

	// HPBarBack ��ü ����
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"HPBarBack")))
	{
		ERR_MSG(g_hWnd, L"HPBarBack ��ü ���� ����");
		return E_FAIL;
	}

	// HPBar ��ü ����
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"HPBar")))
	{
		ERR_MSG(g_hWnd, L"HPBar ��ü ���� ����");
		return E_FAIL;
	}
	// MP ��ü ����
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"MP")))
	{
		ERR_MSG(g_hWnd, L"MP ��ü ���� ����");
		return E_FAIL;
	}

	// Stamina ��ü ����
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"Stamina")))
	{
		ERR_MSG(g_hWnd, L"Stamina ��ü ���� ����");
		return E_FAIL;
	}
	// Run ��ü ����
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"Run")))
	{
		ERR_MSG(g_hWnd, L"Run��ü ���� ����");
		return E_FAIL;
	}
	// SkillPoint ��ü ����
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"SkillPointTrue")))
	{
		ERR_MSG(g_hWnd, L"SkillPointTrue ��ü ���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"SkillPointFalse")))
	{
		ERR_MSG(g_hWnd, L"SkillPointFalse ��ü ���� ����");
		return E_FAIL;
	}

	// SkillPoint ��ü ����
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"StatPointTrue")))
	{
		ERR_MSG(g_hWnd, L"StatPointTrue ��ü ���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"StatPointFalse")))
	{
		ERR_MSG(g_hWnd, L"StatPointFalse ��ü ���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"ExpBar")))
	{
		ERR_MSG(g_hWnd, L"ExpBar ��ü ���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"Inven")))
	{
		ERR_MSG(g_hWnd, L"Inven ��ü ���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"SkillTree")))
	{
		ERR_MSG(g_hWnd, L"SkillTree ��ü ���� ����");
		return E_FAIL;
	}
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"Bank")))
	{
		ERR_MSG(g_hWnd, L"Bank ��ü ���� ����");
		return E_FAIL;
	}
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"Character")))
	{
		ERR_MSG(g_hWnd, L"Character ��ü ���� ����");
		return E_FAIL;
	}
	// ��ų ����Ʈ ��ư
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"BoneSpearButton")))
	{
		ERR_MSG(g_hWnd, L"BoneSpearButton ��ü ���� ����");
		return E_FAIL;
	}
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"FireWallButton")))
	{
		ERR_MSG(g_hWnd, L"FireWallButton ��ü ���� ����");
		return E_FAIL;
	}
	if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pPrototype
		, L"BlizzardButton")))
	{
		ERR_MSG(g_hWnd, L"BlizzardButton ��ü ���� ����");
		return E_FAIL;
	}
#pragma endregion AddObj
#pragma region AddTexture

	/////////////////////////////////////////////////////////
	// UI �̹���
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/StatusBar.png"
		, L"StatusBar", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"StatusBar �ؽ��� �о���� ����");
		return E_FAIL;
	}

	// HP �̹���
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/HP.png"
		, L"HP", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"HP �ؽ��� �о���� ����");
		return E_FAIL;
	}
	// HPBar �̹���
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/HPBar/HPBar.png"
		, L"HPBar", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"HPBar �ؽ��� �о���� ����");
		return E_FAIL;
	}
	// HPBarBack �̹���
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/HPbar/HPBarBack.png"
		, L"HPBarBack", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"HPBarBack �ؽ��� �о���� ����");
		return E_FAIL;
	}

	// MP �̹���
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/MP.png"
		, L"MP", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"MP �ؽ��� �о���� ����");
		return E_FAIL;
	}
	// Stamina �̹���
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/Stamina.png"
		, L"Stamina", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Stamina �ؽ��� �о���� ����");
		return E_FAIL;
	}
	// RUN �̹���
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/Run.png"
		, L"Run", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Run �ؽ��� �о���� ����");
		return E_FAIL;
	}
	// SkillPoint �̹���
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/PointFalse.png"
		, L"SkillPointFalse", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"SkillPointFalse �ؽ��� �о���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/PointTrue.png"
		, L"SkillPointTrue", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"SkillPointTrue �ؽ��� �о���� ����");
		return E_FAIL;
	}
	// StatPoint �̹���
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/PointFalse.png"
		, L"StatPointFalse", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"StatPointFalse �ؽ��� �о���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/PointTrue.png"
		, L"StatPointTrue", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"StatPointTrue �ؽ��� �о���� ����");
		return E_FAIL;
	}

	//ExpBar
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/ExpBar.png"
		, L"ExpBar", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"ExpBar �ؽ��� �о���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/Inven/Inven.png"
		, L"Inven", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Inven �ؽ��� �о���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/SkillTree/PoisonBorn/SkillTree.png"
		, L"SkillTree", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"SkillTree �ؽ��� �о���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/Inven/TownStorage.png"
		, L"Bank", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Bank �ؽ��� �о���� ����");
		return E_FAIL;
	}

	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/Stat/StatBoard.png"
		, L"Character", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"Character �ؽ��� �о���� ����");
		return E_FAIL;
	}


	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/SkillTree/PoisonBorn/BoneSpear.png"
		, L"BoneSpearButton", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"BoneSpearButton �ؽ��� �о���� ����");
		return E_FAIL;
	}
	
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/SkillTree/PoisonBorn/FireWall.png"
		, L"FireWallButton", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"FireWallButton �ؽ��� �о���� ����");
		return E_FAIL;
	}
	
	if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
		L"../Resource/Texture/UI/SkillTree/PoisonBorn/Blizzard.png"
		, L"BlizzardButton", TEXTYPE_SINGLE)))
	{
		ERR_MSG(g_hWnd, L"BlizzardButton �ؽ��� �о���� ����");
		return E_FAIL;
	}

	//-------------------------------------------------------------//
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

	for (int i = 0 ; i < PlayerCount ; ++i)
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
#pragma region Cast
		//////////////////////////////////////////////////////////////////////
		//								Cast								//
		//////////////////////////////////////////////////////////////////////
		// �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Cast/D/Cast_D_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Cast_D", PLAYER_CAST)))
		{
			ERR_MSG(g_hWnd, L"%d Cast D �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ���� �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Cast/LD/Cast_LD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Cast_LD", PLAYER_CAST)))
		{
			ERR_MSG(g_hWnd, L"%d Walk LD �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ������ �Ʒ�
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Cast/RD/Cast_RD_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Cast_RD", PLAYER_CAST)))
		{
			ERR_MSG(g_hWnd, L"%d Walk RD �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Cast/U/Cast_U_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Cast_U", PLAYER_CAST)))
		{
			ERR_MSG(g_hWnd, L"%d Walk U �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ���� ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Cast/LU/Cast_LU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Cast_LU", PLAYER_CAST)))
		{
			ERR_MSG(g_hWnd, L"%d Walk LU �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ������ ��
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Cast/RU/Cast_RU_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Cast_RU", PLAYER_CAST)))
		{
			ERR_MSG(g_hWnd, L"%d Walk RU �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ����
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Cast/L/Cast_L_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Cast_L", PLAYER_CAST)))
		{
			ERR_MSG(g_hWnd, L"%d Walk L �ε� ����" , Charter[i]);
			return E_FAIL;
		}

		// ������
		if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
			L"../Resource/Texture/Stage/Player/Cast/R/Cast_R_%02d.png",
			Charter[i], TEXTYPE_MULTI, L"Cast_R", PLAYER_CAST)))
		{
			ERR_MSG(g_hWnd, L"%d Walk R �ε� ����" , Charter[i]);
			return E_FAIL;
		}
#pragma endregion Cast
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

#pragma endregion AddTexture

#pragma endregion StageProto


#pragma region SkillProto
	{
/*
#pragma region AddObj
		{
			if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pSkillPrototype
				, BONESPEAR)))
			{
				ERR_MSG(g_hWnd, L"BoneSpear ��ü ���� ����");
				return E_FAIL;
			}
			if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pSkillPrototype
				, FIREWALL)))
			{
				ERR_MSG(g_hWnd, L"FireWall ��ü ���� ����");
				return E_FAIL;
			}
			if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pSkillPrototype
				, BLIZZARD)))
			{
				ERR_MSG(g_hWnd, L"Blizzard ��ü ���� ����");
				return E_FAIL;
			}
			if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pSkillPrototype
				, HEAL)))
			{
				ERR_MSG(g_hWnd, L"Heal ��ü ���� ����");
				return E_FAIL;
			}
			if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pSkillPrototype
				, AURA)))
			{
				ERR_MSG(g_hWnd, L"Aura ��ü ���� ����");
				return E_FAIL;
			}
			if(FAILED(GET_SINGLE(CObjMgr)->AddObject(m_pSkillPrototype
				, RIVIVE)))
			{
				ERR_MSG(g_hWnd, L"Rivive ��ü ���� ����");
				return E_FAIL;
			}
		}
#pragma endregion AddObj
*/
#pragma region AddTexture
		{	
#pragma region BoneSpear
			{
				//------------------------Skill--------------------------------//

				if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
					L"../Resource/Texture/Skill/BoneSpear/Missile/D/Missile_D_%02d.png",
					SKILL, TEXTYPE_MULTI, L"BoneSpear_D", PLAYER_BoneSpear_Missile)))
				{
					ERR_MSG(g_hWnd, L"%d BoneSpear D �ε� ����" , BONESPEAR);
					return E_FAIL;
				}

				// ���� �Ʒ�
				if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
					L"../Resource/Texture/Skill/BoneSpear/Missile/LD/Missile_LD_%02d.png",
					SKILL, TEXTYPE_MULTI, L"BoneSpear_LD", PLAYER_BoneSpear_Missile)))
				{
					ERR_MSG(g_hWnd, L"%d BoneSpear LD �ε� ����" , BONESPEAR);
					return E_FAIL;
				}

				// ������ �Ʒ�
				if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
					L"../Resource/Texture/Skill/BoneSpear/Missile/RD/Missile_RD_%02d.png",
					SKILL, TEXTYPE_MULTI, L"BoneSpear_RD", PLAYER_BoneSpear_Missile)))
				{
					ERR_MSG(g_hWnd, L"%d BoneSpear RD �ε� ����" , BONESPEAR);
					return E_FAIL;
				}

				// ��
				if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
					L"../Resource/Texture/Skill/BoneSpear/Missile/U/Missile_U_%02d.png",
					SKILL, TEXTYPE_MULTI, L"BoneSpear_U", PLAYER_BoneSpear_Missile)))
				{
					ERR_MSG(g_hWnd, L"%d BoneSpear U �ε� ����" , BONESPEAR);
					return E_FAIL;
				}

				// ���� ��
				if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
					L"../Resource/Texture/Skill/BoneSpear/Missile/LU/Missile_LU_%02d.png",
					SKILL, TEXTYPE_MULTI, L"BoneSpear_LU", PLAYER_BoneSpear_Missile)))
				{
					ERR_MSG(g_hWnd, L"%d BoneSpear LU �ε� ����" , BONESPEAR);
					return E_FAIL;
				}

				// ������ ��
				if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
					L"../Resource/Texture/Skill/BoneSpear/Missile/RU/Missile_RU_%02d.png",
					SKILL, TEXTYPE_MULTI, L"BoneSpear_RU", PLAYER_BoneSpear_Missile)))
				{
					ERR_MSG(g_hWnd, L"%d BoneSpear RU �ε� ����" , BONESPEAR);
					return E_FAIL;
				}

				// ����
				if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
					L"../Resource/Texture/Skill/BoneSpear/Missile/L/Missile_L_%02d.png",
					SKILL, TEXTYPE_MULTI, L"BoneSpear_L", PLAYER_BoneSpear_Missile)))
				{
					ERR_MSG(g_hWnd, L"%d BoneSpear L �ε� ����" , BONESPEAR);
					return E_FAIL;
				}

				// ������
				if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
					L"../Resource/Texture/Skill/BoneSpear/Missile/R/Missile_R_%02d.png",
					SKILL, TEXTYPE_MULTI, L"BoneSpear_R", PLAYER_BoneSpear_Missile)))
				{
					ERR_MSG(g_hWnd, L"%d BoneSpear R �ε� ����" , BONESPEAR);
					return E_FAIL;
				}

				//-------------------------------------------------------------//
			}
#pragma endregion BoneSpear
#pragma region FireWall
			{
				// FireWall
				if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
					L"../Resource/Texture/Skill/FireWall/FireWall_%02d.png",
					SKILL, TEXTYPE_MULTI, FIREWALL, PLAYER_FireWall)))
				{
					ERR_MSG(g_hWnd, L"%d �ε� ����" , FIREWALL);
					return E_FAIL;
				}

			}
#pragma endregion FireWall
#pragma region Blizzard
			{
				// Blizzard
				if(FAILED(GET_SINGLE(CTextureMgr)->InsertTexture(
					L"../Resource/Texture/Skill/Blizzard/Blizzard_%02d.png",
					SKILL, TEXTYPE_MULTI, BLIZZARD, PLAYER_Blizzard)))
				{
					ERR_MSG(g_hWnd, L"%d �ε� ����" , BLIZZARD);
					return E_FAIL;
				}
			}
#pragma endregion Blizzard
		}
#pragma endregion AddTexture
	}
#pragma endregion SkillProto
	
	return S_OK;
}

SCENEID CStage::Progress()
{
	SCENEID iScene = GET_SINGLE(CObjMgr)->Progress();

	// ��ID�� NONPASS�� �ƴϸ� ���Ŵ������� �ش�ID�� ���� ���� �� �ʱ�ȭ
	if(iScene > SCENEID_NONPASS)
		GET_SINGLE(CSceneMgr)->InitScene(iScene);

	if (iScene == SCENEID_END)
		return SCENEID_END;
	return iScene;
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
	SAFE_DELETE<CPrototype>(&m_pSkillPrototype);
}
