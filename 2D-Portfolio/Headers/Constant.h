#pragma once

const int WINSIZEX = 800;		 /*  창크기   X          */
const int WINSIZEY = 600;		 /*           Y          */
const int TILECNTX = 20;		 /*  맵의가로축타일갯수   */
const int TILECNTY = 40;		 /*  맵의세로축타일갯수   */
const int TILESIZEX = 130;		 /*  맵타일의 가로축크기  */
const int TILESIZEY = 68;		 /*  맵타일의 세로축크기  */

enum TEXTYPE					 /*  텍스트타입           */
{								 /*                      */
	TEXTYPE_SINGLE,				 /*                      */
	TEXTYPE_MULTI,				 /*                      */
};								 /*                      */

/*//const BYTE ITEM_GOLD			= 0x01;
//const BYTE ITEM_WEAPON		= 0x02;
//const BYTE ITEM_ARMOR			= 0x04;
//const BYTE ITEM_ACCESSORY		= 0x08;*/


enum WINMODE
{	// 윈도우 모드, 풀모드
	WINMODE_FULL,
	WINMODE_WIN,
};

enum SCENEID
{	// 장면번호
	SCENEID_END = -2,
	SCENEID_NONPASS = -1,
	SCENEID_LOGO,
	SCENEID_MENU,
	SCENEID_STAGE,
	SCENEID_STAGE2,
	SCENEID_STAGE3,
};

enum SORTLAYER
{	// 정렬
	LAYER_TERRAIN,
	LAYER_OBJECT,
	LAYER_UI,
	LAYER_END,
};

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

// 각도별 방향 16방향
const float Diract16    = 11.25f;
const float R			= 0.f;
const float R_START		= 348.75f;
const float R_END		= 11.25f;
const float RRU			= 22.5f;
const float RRU_START	= 11.25f;
const float RRU_END		= 33.75f;
const float RU			= 45.f;
const float RU_START	= 33.75f;
const float RU_END		= 56.25f;
const float RUU			= 67.5f;
const float RUU_START	= 56.25f;
const float RUU_END		= 78.75f;
const float U			= 90.f;
const float U_START		= 78.75f;
const float U_END		= 112.5f;
const float LUU			= 112.5f;
const float LUU_START	= 112.5f;
const float LUU_END		= 123.75f;
const float LU			= 135.f;
const float LU_START	= 123.75f;
const float LU_END		= 146.25f;
const float LLU			= 157.5f;
const float LLU_START	= 146.25f;
const float LLU_END		= 168.75f;
const float L			= 180.f;
const float L_START		= 168.75f;
const float L_END		= 191.25f;
const float LLD			= 202.5f;
const float LLD_START	= 191.25f;
const float LLD_END		= 213.75f;
const float LD			= 225.f;
const float LD_START	= 213.75f;
const float LD_END		= 236.25f;
const float LDD			= 247.5f;
const float LDD_START	= 236.25f;
const float LDD_END		= 258.75f;
const float D			= 270.f;
const float D_START		= 258.75f;
const float D_END		= 281.25f;
const float RDD			= 292.5f;
const float RDD_START	= 281.25f;
const float RDD_END		= 303.75f;
const float RD			= 315.f;
const float RD_START	= 303.75f;
const float RD_END		= 326.25f;
const float RRD			= 337.5f;
const float RRD_START	= 326.25f;
const float RRD_END		= 348.75f;

// 
#define PI 3.1415926535897932384626433832795f

// 타일 옵션 종류
const BYTE TILE_OPTION_NONE		 = 0x00; // 0000 0001
const BYTE TILE_OPTION_DONT_MOVE = 0x01; // 0000 0001


// 모션 프레임 매크로화
const float PLAYER_STAND				= 8.0f;
const float PLAYER_WALK					= 8.0f;
const float PLAYER_RUN					= 8.0f;
const float PLAYER_ATTACK				= 19.0f;
const float PLAYER_CAST					= 16.0f;

const float PLAYER_BoneSpear_Missile	= 6.0f;
const float PLAYER_BoneSpear_Tail		= 9.0f;
const float PLAYER_FireWall				= 7.0f;
const float PLAYER_Blizzard				= 8.0f;

const float SKELETON_STAND			= 8.0f;
const float SKELETON_RUN			= 8.0f;
const float SKELETON_ATTACK			= 16.0f;
const float SKELETON_DEATH			= 19.0f;

const float SUMMONER_STAND			= 8.0f;
const float SUMMONER_CAST			= 8.0f;
const float SUMMONER_RUN			= 8.0f;
const float SUMMONER_ATTACK			= 12.0f;
const float SUMMONER_DEATH			= 16.0f;

// 모션 키워드
TCHAR* const STAND					= L"FieldStand";
TCHAR* const WALK					= L"FieldWalk";
TCHAR* const RUN					= L"Run";
TCHAR* const ATTACK					= L"Attack";
TCHAR* const DEATH					= L"Death";
TCHAR* const ATTACKSKILL			= L"AttackSKill";
TCHAR* const CAST					= L"CAST";
TCHAR* const SUMMON					= L"SUMMON";


// 플레이어 키워드
TCHAR* const PLAYER					= L"Player";
TCHAR* const FELLOW_1ST				= L"Fellow_First";
TCHAR* const FELLOW_2ND				= L"Fellow_Second";

TCHAR* const MONSTER				= L"Monster";
TCHAR* const BUTTON					= L"Button";
TCHAR* const SKILL					= L"Skill";

TCHAR* const BONESPEAR				= L"BoneSpear";
TCHAR* const FIREWALL				= L"FireWall";
TCHAR* const BLIZZARD				= L"Blizzard";
TCHAR* const HEAL					= L"Heal";
TCHAR* const AURA					= L"Aura";
TCHAR* const RIVIVE					= L"Rivive";


TCHAR* const FONT0					= L"0";
TCHAR* const FONT1					= L"1";
TCHAR* const FONT2					= L"2";
TCHAR* const FONT3					= L"3";
TCHAR* const FONT4					= L"4";
TCHAR* const FONT5					= L"5";
TCHAR* const FONT6					= L"6";
TCHAR* const FONT7					= L"7";
TCHAR* const FONT8					= L"8";
TCHAR* const FONT9					= L"9";


TCHAR* const SKELETON	= L"Skeleton";
TCHAR* const SANDRAIDER = L"SandRaider";
TCHAR* const VIPER		= L"Viper";
TCHAR* const GRISWOLD	= L"Griswold";
TCHAR* const SUMMONER	= L"Summoner";
TCHAR* const ANDARIEL	= L"Andariel";
TCHAR* const IZUAL		= L"Izual";
TCHAR* const DIABLO		= L"Diablo";
