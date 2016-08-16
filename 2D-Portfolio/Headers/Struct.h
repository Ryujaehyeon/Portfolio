#pragma once
typedef struct AStarNode
{
	// �̵� ���
	float fCost;
	// ��ǥ��
	int iIndex;
	// 
	AStarNode* pParent;

}NODE;
typedef struct tagObjInfo
{
	// ������Ʈ ��ǥ
	D3DXVECTOR3 vPos;
	// ����
	D3DXVECTOR3 vDir;
	// 
	D3DXVECTOR3 vLook;
	// �߽���
	D3DXVECTOR3 vCenter;
	// �׸� X��ǥ
	float fX;
	// �׸� Y��ǥ
	float fY;
	// X ũ��
	float fCX;
	// Y ũ��
	float fCY;
	// ������Ʈ ũ��
	D3DXMATRIX  matScale;
	// ������Ʈ ����
	D3DXMATRIX  matRotz;
	// ������Ʈ �̵�
	D3DXMATRIX  matTrans;
	// ������Ʈ�� ������ ���� ��ǥ
	D3DXMATRIX  matWorld;
	// ������Ʈ�� ���É������ ����
	bool		unitSelect;
	// � �ؽ��ĸ� �׸���
	BYTE        byDrawID;
	// ������Ʈ�� �ɼ�
	BYTE        byOption;
	tagObjInfo(){}
	tagObjInfo(const tagObjInfo& Info)
		:vPos(Info.vPos), vDir(Info.vDir), vLook(Info.vLook)
		,vCenter(Info.vCenter), matWorld(Info.matWorld)
	{}
}OBJINFO;

typedef struct tagFrame
{
	float fStart;
	float fLast;
	//float fScene;
	//DWORD dwTime;
	tagFrame(){}
	tagFrame(float _fStart
		, float _fLast
		/*, float _fScene
		, DWORD _dwTime*/)
	{
		fStart = _fStart;
		fLast  = _fLast;
		//fScene = _fScene;
		//dwTime = _dwTime;
	}
}FRAME;

enum ITEM_TYPE
{
	ITEM_POTION = -1,
	ITEM_HEAD,
	ITEM_ACCESSORY_AMULET,
	ITEM_ACCESSORY_RING,
	ITEM_ARMOR,
	ITEM_GLOVES,
	ITEM_BELT,
	ITEM_SHOES,
	ITEM_SHIELD,
	ITEM_WEAPON,
	ITEM_END,
};

typedef struct tagItem
{
	// ������Ʈ �̸�
	TCHAR szName[128];
	// ��(Might): 10�� �ʰ��� 1���� ������(����, ����, ����(!))�� ȸ����ġ(����, ����, �����Ƽ) +3%, �γ�(Fortitude) ����ġ +2
	float fMight;
	// ü��(Constitution): 10�� �ʰ��� 1���� ������(Endurance)�� ü��(Health) +5%, �γ� ����ġ +2
	float fConstitution;
	// ��ø(Dexterity): 10�� �ʰ��� 1���� �ൿ �ӵ�(Action Speed) +3%, �ݻ�(Reflex) ����ġ +2
	float fDexterity;
	// ����(Perception): 10�� �ʰ��� 1���� ����(Interrupt) ��ġ +3, ���߷� +1 , �ݻ� ����ġ +2
	float fPerception;
	// ����(Intellect): 10�� �ʰ��� 1���� ����/�����Ƽ�� �������(AOE) +6%, ����/�����Ƽ�� ��ȿ�ð� +5%, ���� (Will) ����ġ +2
	float fIntellect;
	// ����(Resolve): 10�� �ʰ��� 1���� ����(Concentration) +3, ȸ�� ����ġ +1, ���� ����ġ +2
	float fResolve;
	// ���ݷ�
	float fAttack;
	// ����
	float fDefence;
	// ü��
	float fHealthPoint;
	// ����
	float fMagikaPoint;
	int  iSKillPoint;
	int  iGold;
	int  iCnt;
	int  iType;
}ITEM;
typedef struct tagItem* PITEM;

typedef struct tagInfo
{
	// ������Ʈ �̸�
	TCHAR szName[128];
	// ����
	int iLevel;
	// ����ġ
	float fExp;
	float fMaxExp;
	// ��(Might): 10�� �ʰ��� 1���� ������(����, ����, ����(!))�� ȸ����ġ(����, ����, �����Ƽ) +3%, �γ�(Fortitude) ����ġ +2
	float fMight;
	// ü��(Constitution): 10�� �ʰ��� 1���� ������(Endurance)�� ü��(Health) +5%, �γ� ����ġ +2
	float fConstitution;
	// ��ø(Dexterity): 10�� �ʰ��� 1���� �ൿ �ӵ�(Action Speed) +3%, �ݻ�(Reflex) ����ġ +2
	float fDexterity;
	// ����(Perception): 10�� �ʰ��� 1���� ����(Interrupt) ��ġ +3, ���߷� +1 , �ݻ� ����ġ +2
	float fPerception;
	// ����(Intellect): 10�� �ʰ��� 1���� ����/�����Ƽ�� �������(AOE) +6%, ����/�����Ƽ�� ��ȿ�ð� +5%, ���� (Will) ����ġ +2
	float fIntellect;
	// ����(Resolve): 10�� �ʰ��� 1���� ����(Concentration) +3, ȸ�� ����ġ +1, ���� ����ġ +2
	float fResolve;
	// ���ݷ�
	float fAttack;
	// ����
	float fDefence;
	// ü��
	float fHealthPoint;
	// �ִ� ü��
	float fHealthPointMAX;
	// ����
	float fMagikaPoint;
	// �ִ� ����
	float fMagikaPointMAX;
	// ��ų ����Ʈ
	int  iSKillPoint;
	// ���� ����Ʈ
	int  iStatPoint;
	// ����
	int iGenderIdx;
	// ������
	int iGold;
	float fSpeed;
	// ������
	PITEM tItem[ITEM_END];
	PITEM tInventory[50];
}INFO;



enum OBJ_TYPE
{
	OBJ_PLAYER,
	OBJ_SKILL,
	OBJ_MONSTER,
	OBJ_NONPLAYER,
	OBJ_BROKENBUILDING,
	OBJ_BUILDING,
	OBJ_BUTTON,
	OBJ_UI
};


typedef struct tagTexture
{
	// �ؽ�ó�� ��� ������
	LPDIRECT3DTEXTURE9 pTexture;
	// ����� �̹����� �׷���� ��� ����
	D3DXIMAGE_INFO     ImgInfo;
}TEXINFO;

typedef struct tagTile
{
	D3DXVECTOR3 vPos;
	// �������� Ÿ�� �� ���° ����� Ÿ���� �׸����� ���� ����
	BYTE        byDrawID;
	// � ȿ���� ���� Ÿ�������� ���� ���� 
	BYTE        byOption;
	// Ÿ�� ���� ����
	bool		TileSelect;
}TILE;

