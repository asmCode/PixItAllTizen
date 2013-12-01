#pragma once

#include <string>

class IGameCommunityObserver
{
public:
	virtual ~IGameCommunityObserver() {}

	virtual void PlayerAuthenticated(const std::string &playerId) {};
	virtual void PlayerDeauthenticated() {};
	virtual void PointsLoaded(unsigned points) {};
};
