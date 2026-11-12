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
constexpr int PLAYER_STAND				= 8;
constexpr int PLAYER_WALK					= 8;
constexpr int PLAYER_RUN					= 8;
constexpr int PLAYER_ATTACK				= 19;
constexpr int PLAYER_CAST					= 16;

constexpr int PLAYER_BoneSpear_Missile	= 6;
constexpr int PLAYER_BoneSpear_Tail		= 9;
constexpr int PLAYER_FireWall				= 7;
constexpr int PLAYER_Blizzard				= 8;

constexpr int SKELETON_STAND			= 8;
constexpr int SKELETON_RUN			= 8;
constexpr int SKELETON_ATTACK			= 16;
constexpr int SKELETON_DEATH			= 19;

constexpr int SUMMONER_STAND			= 8;
constexpr int SUMMONER_CAST			= 8;
constexpr int SUMMONER_RUN			= 8;
constexpr int SUMMONER_ATTACK			= 12;
constexpr int SUMMONER_DEATH			= 16;

// 모션 키워드
const TCHAR* const STAND					= L"Stand";
const TCHAR* const WALK					= L"Walk";
const TCHAR* const RUN					= L"Run";
const TCHAR* const ATTACK					= L"Attack";
const TCHAR* const DEATH					= L"Death";
const TCHAR* const ATTACKSKILL			= L"AttackSKill";
const TCHAR* const CAST					= L"CAST";
const TCHAR* const SUMMON					= L"SUMMON";


// 플레이어 키워드
const TCHAR* const PLAYER					= L"Player";
const TCHAR* const FELLOW_1ST				= L"Fellow_First";
const TCHAR* const FELLOW_2ND				= L"Fellow_Second";

const TCHAR* const MONSTER				= L"Monster";
const TCHAR* const BUTTON					= L"Button";
const TCHAR* const SKILL					= L"Skill";

const TCHAR* const BONESPEAR				= L"BoneSpear";
const TCHAR* const FIREWALL				= L"FireWall";
const TCHAR* const BLIZZARD				= L"Blizzard";
const TCHAR* const HEAL					= L"Heal";
const TCHAR* const AURA					= L"Aura";
const TCHAR* const RIVIVE					= L"Rivive";


const TCHAR* const FONT0					= L"0";
const TCHAR* const FONT1					= L"1";
const TCHAR* const FONT2					= L"2";
const TCHAR* const FONT3					= L"3";
const TCHAR* const FONT4					= L"4";
const TCHAR* const FONT5					= L"5";
const TCHAR* const FONT6					= L"6";
const TCHAR* const FONT7					= L"7";
const TCHAR* const FONT8					= L"8";
const TCHAR* const FONT9					= L"9";


const TCHAR* const SKELETON	= L"Skeleton";
const TCHAR* const SANDRAIDER = L"SandRaider";
const TCHAR* const VIPER		= L"Viper";
const TCHAR* const GRISWOLD	= L"Griswold";
const TCHAR* const SUMMONER	= L"Summoner";
const TCHAR* const ANDARIEL	= L"Andariel";
const TCHAR* const IZUAL		= L"Izual";
const TCHAR* const DIABLO		= L"Diablo";
