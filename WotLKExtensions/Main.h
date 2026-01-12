#pragma once

#include "CustomLua.h"
#include "CustomGlueLua.h"
#include "CustomPacket.h"
#include "Misc.h"
#include "Player.h"
#include "World.h"
#include "CDBCMgr/CDBCMgr.h"

class Main
{
public:
	static void OnAttach();
	static void Init();
};
