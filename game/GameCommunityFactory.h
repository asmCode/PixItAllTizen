#pragma once

#include "IGameCommunity.h"
#include <string>

class IGameCenterViewProxy;

class GameCommunityFactory
{
public:
	static IGameCommunity* CreateGameCenter(IGameCenterViewProxy *gameCenterViewProxy);
};
