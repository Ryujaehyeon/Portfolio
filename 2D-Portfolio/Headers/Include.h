#pragma once
#include "Debug.h"
#include "Debug.hpp"


//===========Manager Header============//
#include "Device.h"
#include "TextureMgr.h"

//=============ETC Header==============//
#include "SingleTexture.h"
#include "MultiTexture.h"

#ifdef __CLIENT__EXEC__

//===========Manager Header============//
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "TimeMgr.h"
#include "AStar.h"
#include "CollisionMgr.h"

//===========Scene Header==============//
#include "Logo.h"
#include "MenuD2.h"
#include "Stage.h"
#include "Stage2.h"
#include "Stage3.h"

//===========Object Header=============//
#include "LogoBackGround.h"
#include "MenuBackground.h"
#include "StageBackground.h"
#include "MenuButton.h"
#include "UIObj.h"
#include "Ball.h"

#include "Player.h"
#include "Monster.h"
#include "NonPlayer.h"
#include "UIObj.h"

//==========Prototype Header============//
#include "LogoObjProto.h"
#include "MenuObjProto.h"
#include "StageObjProto.h"

//================Skill=================//
#include "Skill.h"
#include "SkillTree.h"
//================ETC===================//
#include "GameSound.hpp"
#include "FullModeMgr.h"
#include "KeyMgr.h"
#include "Value.h"

#endif