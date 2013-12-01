#ifndef LEADERBOARDOBSERVER_H_
#define LEADERBOARDOBSERVER_H_

class ILeaderboardObserver
{
public:
	virtual ~ILeaderboardObserver() {}

	virtual void LeaderTopLoaded() = 0;
	virtual void LeaderPlayerLoaded() = 0;
	virtual void PointsUpdated(int playerId) = 0;
};

#endif /* LEADERBOARDOBSERVER_H_ */
