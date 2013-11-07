#include "GameCommunityFactory.h"
#include "GameCenterCommunity.h"
#include "DummyCommunity.h"

IGameCommunity* GameCommunityFactory::CreateGameCenter(IGameCenterViewProxy *gameCenterViewProxy)
{
	//return new GameCenterCommunity(gameCenterViewProxy);
	return new DummyCommunity();
}
