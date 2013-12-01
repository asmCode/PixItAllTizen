#ifndef LEADERBOARD_H_
#define LEADERBOARD_H_

#include "IHttpCommunicationObserver.h"
#include "PlayerStats.h"
#include <string>
#include <vector>

class HttpCommunication;
class ILeaderboardObserver;
class XMLNode;

class Leaderboard : public IHttpCommunicationObserver
{
public:
	static Leaderboard* m_instance;

	virtual ~Leaderboard();

	static Leaderboard* GetInstance();

	void AddObserver(ILeaderboardObserver* observer);

	void RefreshTopLadder();
	void RefreshSurrLadder(int playerPoints);
	void SendPlayerPoints(int id, const std::string& playerName, int points, int levels);

	const std::vector<PlayerStats>& GetTopLadder() const;
	const std::vector<PlayerStats>& GetPlayerLadder() const;

private:
	static const std::string HostAddress;
	static const std::string AddUserAddress;
	static const std::string TopAddress;
	static const std::string UserSurroundingAddress;

	static const int TopCount = 10;
	static const int SurrCount = 4;

	std::vector<ILeaderboardObserver*> m_observers;

	HttpCommunication* m_httpUser;
	HttpCommunication* m_httpTop;
	HttpCommunication* m_httpSurr;

	std::vector<PlayerStats> m_topStats;
	std::vector<PlayerStats> m_surrStats;

	Leaderboard();
	void Initialize();

	void FetchArrayFromRasult(std::vector<PlayerStats>& array, XMLNode* node);

	void Timeount();
	void Response(HttpCommunication* http, int httpCode, const std::string& data);

	void ProcessTopResponse(XMLNode* node);
	void ProcessSurrResponse(XMLNode* node);
	void ProcessUserResponse(XMLNode* node);
};

#endif /* LEADERBOARD_H_ */
