#include "Leaderboard.h"
#include "HttpCommunication.h"
#include "ILeaderboardObserver.h"
#include <Utils/Log.h>
#include <Utils/Xml/XMLLoader.h>
#include <Utils/Xml/XMLNode.h>
#include <stdio.h>

Leaderboard* Leaderboard::m_instance;
const std::string Leaderboard::HostAddress = "http://pixitall.semiseriousgames.com";
const std::string Leaderboard::AddUserAddress = "http://pixitall.semiseriousgames.com/server/ladder/Points.php?id=%d&n=%s&p=%d&l=%d";
const std::string Leaderboard::TopAddress = "http://pixitall.semiseriousgames.com/server/ladder/LadderTop.php?l=%d";
const std::string Leaderboard::UserSurroundingAddress = "http://pixitall.semiseriousgames.com/server/ladder/LadderSurr.php?p=%d&l=%d";

Leaderboard::Leaderboard()
{
	m_httpUser = new HttpCommunication(HostAddress);
	m_httpTop = new HttpCommunication(HostAddress);
	m_httpSurr = new HttpCommunication(HostAddress);

	m_httpUser->AddObserver(this);
	m_httpTop->AddObserver(this);
	m_httpSurr->AddObserver(this);
}

Leaderboard::~Leaderboard()
{
}

Leaderboard* Leaderboard::GetInstance()
{
	if (m_instance == NULL)
		m_instance = new Leaderboard();

	return m_instance;
}

void Leaderboard::AddObserver(ILeaderboardObserver* observer)
{
	m_observers.push_back(observer);
}

void Leaderboard::RefreshTopLadder()
{
	if (m_httpTop->IsBusy())
		return;

	char request[2048];
	sprintf(request, TopAddress.c_str(), TopCount);

	m_httpTop->SendRequest(request);
}

void Leaderboard::RefreshSurrLadder(int playerPoints)
{
	if (m_httpSurr->IsBusy())
		return;
}

void Leaderboard::SendPlayerPoints(int id, const std::string& playerName, int points, int levels)
{
	if (m_httpUser->IsBusy())
		return;

	char request[2048];
	sprintf(request, AddUserAddress.c_str(), id, playerName.c_str(), points, levels);

	m_httpUser->SendRequest(request);
}

const std::vector<PlayerStats>& Leaderboard::GetTopLadder() const
{
	return m_topStats;
}

const std::vector<PlayerStats>& Leaderboard::GetPlayerLadder() const
{
	return m_surrStats;
}

void Leaderboard::Timeount()
{

}

void Leaderboard::Response(HttpCommunication* http, int httpCode, const std::string& data)
{
	if (httpCode != 200)
	{
		Log::LogT("Error http response %d", httpCode);
		return;
	}

	XMLNode *node = XMLLoader::LoadFromString(data);
	if (node == NULL)
	{
		Log::LogT("Couldn't parse http response");
		return;
	}

	if (!node->HasAttrib("result"))
	{
		Log::LogT("Wrong http response format");
		return;
	}

	if (node->GetAttribAsInt32("result") != 0)
	{
		Log::LogT("Http response error");
		return;
	}

	if (http == m_httpTop)
		ProcessTopResponse(node);
	else if (http == m_httpSurr)
		ProcessSurrResponse(node);
	else if (http == m_httpUser)
		ProcessUserResponse(node);
}

void Leaderboard::ProcessTopResponse(XMLNode* node)
{
	m_topStats.clear();

	FetchArrayFromRasult(m_topStats, node);

	for (unsigned int i = 0; i < m_observers.size(); i++)
		m_observers[i]->LeaderTopLoaded();
}

void Leaderboard::ProcessSurrResponse(XMLNode* node)
{

}

void Leaderboard::ProcessUserResponse(XMLNode* node)
{
	int userId = 0;

	if (node->GetChildrenCount() == 1)
	{
		XMLNode* child = node->GetChild(0);
		if (child->GetName() == "user" && child->HasAttrib("id"))
			userId = child->GetAttribAsInt32("id");
	}

	for (unsigned int i = 0; i < m_observers.size(); i++)
		m_observers[i]->PointsUpdated(userId);
}

void Leaderboard::FetchArrayFromRasult(std::vector<PlayerStats>& array, XMLNode* node)
{
	for (unsigned int i = 0; i < node->GetChildrenCount(); i++)
	{
		XMLNode* child = node->GetChild(i);
		if (child == NULL)
			continue;

		PlayerStats stats;

		if (child->HasAttrib("n"))
			stats.m_name = child->GetAttribAsString("n");
		if (child->HasAttrib("p"))
			stats.m_points = child->GetAttribAsInt32("p");
		if (child->HasAttrib("l"))
			stats.m_levels = child->GetAttribAsInt32("l");

		array.push_back(stats);
	}
}
