#pragma once
typedef struct AStarNode
{
	// 이동 비용
	float fCost;
	// 좌표값
	int iIndex;
	// 
	AStarNode* pParent;

}NODE;
typedef struct tagObjInfo
{
	// 오브젝트 좌표
	D3DXVECTOR3 vPos;
	// 방향
	D3DXVECTOR3 vDir;
	// 
	D3DXVECTOR3 vLook;
	// 중심점
	D3DXVECTOR3 vCenter;
	// 그릴 X좌표
	float fX;
	// 그릴 Y좌표
	float fY;
	// X 크기
	float fCX;
	// Y 크기
	float fCY;
	// 오브젝트 크기
	D3DXMATRIX  matScale;
	// 오브젝트 자전
	D3DXMATRIX  matRotz;
	// 오브젝트 이동
	D3DXMATRIX  matTrans;
	// 오브젝트가 존재할 월드 좌표
	D3DXMATRIX  matWorld;
	// 오브젝트가 선택됬는지의 여부
	bool		unitSelect;
	// 어떤 텍스쳐를 그릴지
	BYTE        byDrawID;
	// 오브젝트의 옵션
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
	// 오브젝트 이름
	TCHAR szName[128];
	// 힘(Might): 10점 초과시 1점당 데미지(근접, 투사, 마법(!))와 회복수치(포션, 마법, 어빌리티) +3%, 인내(Fortitude) 방어수치 +2
	float fMight;
	// 체질(Constitution): 10점 초과시 1점당 지구력(Endurance)과 체력(Health) +5%, 인내 방어수치 +2
	float fConstitution;
	// 민첩(Dexterity): 10점 초과시 1점당 행동 속도(Action Speed) +3%, 반사(Reflex) 방어수치 +2
	float fDexterity;
	// 통찰(Perception): 10점 초과시 1점당 방해(Interrupt) 수치 +3, 명중률 +1 , 반사 방어수치 +2
	float fPerception;
	// 지능(Intellect): 10점 초과시 1점당 마법/어빌리티의 적용범위(AOE) +6%, 마법/어빌리티의 유효시간 +5%, 의지 (Will) 방어수치 +2
	float fIntellect;
	// 결의(Resolve): 10점 초과시 1점당 집중(Concentration) +3, 회피 방어수치 +1, 의지 방어수치 +2
	float fResolve;
	// 공격력
	float fAttack;
	// 방어력
	float fDefence;
	// 체력
	float fHealthPoint;
	// 마력
	float fMagikaPoint;
	int  iSKillPoint;
	int  iGold;
	int  iCnt;
	int  iType;
}ITEM;
typedef struct tagItem* PITEM;

typedef struct tagInfo
{
	// 오브젝트 이름
	TCHAR szName[128];
	// 레벨
	int iLevel;
	// 경험치
	float fExp;
	float fMaxExp;
	// 힘(Might): 10점 초과시 1점당 데미지(근접, 투사, 마법(!))와 회복수치(포션, 마법, 어빌리티) +3%, 인내(Fortitude) 방어수치 +2
	float fMight;
	// 체질(Constitution): 10점 초과시 1점당 지구력(Endurance)과 체력(Health) +5%, 인내 방어수치 +2
	float fConstitution;
	// 민첩(Dexterity): 10점 초과시 1점당 행동 속도(Action Speed) +3%, 반사(Reflex) 방어수치 +2
	float fDexterity;
	// 통찰(Perception): 10점 초과시 1점당 방해(Interrupt) 수치 +3, 명중률 +1 , 반사 방어수치 +2
	float fPerception;
	// 지능(Intellect): 10점 초과시 1점당 마법/어빌리티의 적용범위(AOE) +6%, 마법/어빌리티의 유효시간 +5%, 의지 (Will) 방어수치 +2
	float fIntellect;
	// 결의(Resolve): 10점 초과시 1점당 집중(Concentration) +3, 회피 방어수치 +1, 의지 방어수치 +2
	float fResolve;
	// 공격력
	float fAttack;
	// 방어력
	float fDefence;
	// 체력
	float fHealthPoint;
	// 최대 체력
	float fHealthPointMAX;
	// 마력
	float fMagikaPoint;
	// 최대 마력
	float fMagikaPointMAX;
	// 스킬 포인트
	int  iSKillPoint;
	// 스텟 포인트
	int  iStatPoint;
	// 성별
	int iGenderIdx;
	// 소지금
	int iGold;
	float fSpeed;
	// 아이템
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
	// 텍스처를 담는 포인터
	LPDIRECT3DTEXTURE9 pTexture;
	// 비슷한 이미지의 그룹명을 담는 변수
	D3DXIMAGE_INFO     ImgInfo;
}TEXINFO;

typedef struct tagTile
{
	D3DXVECTOR3 vPos;
	// 여러가지 타일 중 몇번째 모양의 타일을 그릴지에 대한 변수
	BYTE        byDrawID;
	// 어떤 효과를 가진 타일인지에 대한 변수 
	BYTE        byOption;
	// 타일 선택 여부
	bool		TileSelect;
}TILE;

