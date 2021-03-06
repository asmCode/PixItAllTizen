#include "Leaderboard.h"
#include "HttpCommunication.h"
#include "ILeaderboardObserver.h"
#include <Utils/StringUtils.h>
#include <Utils/Log.h>
#include <Utils/Xml/XMLLoader.h>
#include <Utils/Xml/XMLNode.h>
#include <math.h>
#include <FBase.h>
#include <stdio.h>

using namespace Tizen::Base;
using namespace Tizen::Base::Utility;

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

	char request[2048];
	sprintf(request, UserSurroundingAddress.c_str(), playerPoints, SurrCount);

	m_httpSurr->SendRequest(request);
}

void Leaderboard::SendPlayerPoints(int id, const std::string& playerName, int points, int levels)
{
	if (m_httpUser->IsBusy())
		return;

	ByteBuffer buffer;
	buffer.Construct(reinterpret_cast<const byte*>(playerName.c_str()), 0, playerName.size(), playerName.size());
	String base64String;
	StringUtil::EncodeToBase64String(buffer, base64String);

	char request[2048];
	sprintf(request, AddUserAddress.c_str(), id, StringUtils::ToNarrow(base64String.GetPointer()).c_str(), points, levels);

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
	for (unsigned int i = 0; i < m_observers.size(); i++)
		m_observers[i]->LeaderOffline();
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

	FetchArrayFromRasult(m_topStats, node, 1);

	for (unsigned int i = 0; i < m_observers.size(); i++)
		m_observers[i]->LeaderTopLoaded();
}

void Leaderboard::ProcessSurrResponse(XMLNode* node)
{
	m_surrStats.clear();

	int rank = 0;
	if (node->HasAttrib("rank"))
		rank = node->GetAttribAsInt32("rank");

	FetchArrayFromRasult(m_surrStats, node, rank);

	for (unsigned int i = 0; i < m_observers.size(); i++)
		m_observers[i]->LeaderPlayerLoaded();
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

void Leaderboard::FetchArrayFromRasult(std::vector<PlayerStats>& array, XMLNode* node, int rank)
{
	for (unsigned int i = 0; i < fmin(node->GetChildrenCount(), 10); i++)
	{
		XMLNode* child = node->GetChild(i);
		if (child == NULL)
			continue;

		PlayerStats stats;

		if (child->HasAttrib("i"))
			stats.m_id = child->GetAttribAsInt32("i");
		if (child->HasAttrib("n"))
		{
			std::string name = child->GetAttribAsString("n");

			ByteBuffer *buffer = StringUtil::DecodeBase64StringN(name.c_str());
			if (buffer != NULL)
				name.assign(reinterpret_cast<const char*>(buffer->GetPointer()), buffer->GetCapacity());

			stats.m_name = name;
		}
		if (child->HasAttrib("p"))
			stats.m_points = child->GetAttribAsInt32("p");
		if (child->HasAttrib("l"))
			stats.m_levels = child->GetAttribAsInt32("l");

		stats.m_place = rank++;

		array.push_back(stats);
	}
}
