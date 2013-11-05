#include "GameCommunityFactory.h"
#include "GameCenterCommunity.h"

IGameCommunity* GameCommunityFactory::CreateGameCenter(IGameCenterViewProxy *gameCenterViewProxy)
{
	return new GameCenterCommunity(gameCenterViewProxy);
}
