#pragma once
typedef struct AStarNode
{
	// 이동 비용
	float fCost;
	// 좌표값
	int iIndex;
	// 
	AStarNode* pParent;
	AStarNode()
		: fCost(0.f)
		, iIndex(0)
		, pParent(nullptr)
	{
	}
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
	tagObjInfo()
		: vPos(0.f, 0.f, 0.f)
		, vDir(0.f, 0.f, 0.f)
		, vLook(0.f, 0.f, 0.f)
		, vCenter(0.f, 0.f, 0.f)
		, fX(0.f)
		, fY(0.f)
		, fCX(0.f)
		, fCY(0.f)
		, matScale(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f)
		, matRotz(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f)
		, matTrans(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f)
		, matWorld(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f)
		, unitSelect(false)
		, byDrawID(0)
		, byOption(0)
	{
	}
	tagObjInfo(const tagObjInfo& Info)
		: vPos(Info.vPos)
		, vDir(Info.vDir)
		, vLook(Info.vLook)
		, vCenter(Info.vCenter)
		, fX(Info.fX)
		, fY(Info.fY)
		, fCX(Info.fCX)
		, fCY(Info.fCY)
		, matScale(Info.matScale)
		, matRotz(Info.matRotz)
		, matTrans(Info.matTrans)
		, matWorld(Info.matWorld)
		, unitSelect(Info.unitSelect)
		, byDrawID(Info.byDrawID)
		, byOption(Info.byOption)
	{
	}
}OBJINFO;

typedef struct tagFrame
{
	float fStart;
	float fLast;
	tagFrame()
		: fStart(0.f)
		, fLast(0.f)
	{
	}
	tagFrame(float _fStart
		, float _fLast)
		: fStart(_fStart)
		, fLast(_fLast)
	{
	}
}FRAME;

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
	tagItem()
		: szName()
		, fMight(0.f)
		, fConstitution(0.f)
		, fDexterity(0.f)
		, fPerception(0.f)
		, fIntellect(0.f)
		, fResolve(0.f)
		, fAttack(0.f)
		, fDefence(0.f)
		, fHealthPoint(0.f)
		, fMagikaPoint(0.f)
		, iSKillPoint(0)
		, iGold(0)
		, iCnt(0)
		, iType(0)
	{
	}
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
	tagInfo()
		: szName()
		, iLevel(0)
		, fExp(0.f)
		, fMaxExp(0.f)
		, fMight(0.f)
		, fConstitution(0.f)
		, fDexterity(0.f)
		, fPerception(0.f)
		, fIntellect(0.f)
		, fResolve(0.f)
		, fAttack(0.f)
		, fDefence(0.f)
		, fHealthPoint(0.f)
		, fHealthPointMAX(0.f)
		, fMagikaPoint(0.f)
		, fMagikaPointMAX(0.f)
		, iSKillPoint(0)
		, iStatPoint(0)
		, iGenderIdx(0)
		, iGold(0)
		, fSpeed(0.f)
		, tItem()
		, tInventory()
	{
	}
}INFO;

// 
typedef struct tagBoneSpear
{
	// 스킬레벨
	int iLevel;
	// 요구레벨
	int iMinLevel;
	// 최대레벨
	int iLimitLevel;
	// 시전간격
	float fCastTime;
	// 소비 HP
	float fHpCost;
	// 소비 MP
	float fMpCost;
	tagBoneSpear()
		: iLevel(0)
		, iMinLevel(0)
		, iLimitLevel(0)
		, fCastTime(0.f)
		, fHpCost(0.f)
		, fMpCost(0.f)
	{
	}
}BoneSpearSkill;

typedef struct tagFireWall
{
	// 스킬레벨
	int iLevel;
	// 요구레벨
	int iMinLevel;
	// 최대레벨
	int iLimitLevel;
	// 시전간격
	float fCastTime;
	// 소비 HP
	float fHpCost;
	// 소비 MP
	float fMpCost;
	tagFireWall()
		: iLevel(0)
		, iMinLevel(0)
		, iLimitLevel(0)
		, fCastTime(0.f)
		, fHpCost(0.f)
		, fMpCost(0.f)
	{
	}
}FireWallSkill;

typedef struct tagBlizzard
{
	// 스킬레벨
	int iLevel;
	// 요구레벨
	int iMinLevel;
	// 최대레벨
	int iLimitLevel;
	// 시전간격
	float fCastTime;
	// 소비 HP
	float fHpCost;
	// 소비 MP
	float fMpCost;
	tagBlizzard()
		: iLevel(0)
		, iMinLevel(0)
		, iLimitLevel(0)
		, fCastTime(0.f)
		, fHpCost(0.f)
		, fMpCost(0.f)
	{
	}
}BlizzardSkill;

typedef struct tagSkill
{
	BoneSpearSkill sBoneSpear;
	FireWallSkill sFireWall;
	BlizzardSkill sBlizzard;
	tagSkill()
		: sBoneSpear()
		, sFireWall()
		, sBlizzard()
	{
	}
}SkillTree;




typedef struct tagTexture
{
	// 텍스처를 담는 포인터
	LPDIRECT3DTEXTURE9 pTexture;
	// 비슷한 이미지의 그룹명을 담는 변수
	D3DXIMAGE_INFO     ImgInfo;
	tagTexture()
		: pTexture(nullptr)
		, ImgInfo()
	{
	}
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
	tagTile()
		: vPos(0.f, 0.f, 0.f)
		, byDrawID(0)
		, byOption(0)
		, TileSelect(false)
	{
	}
}TILE;

