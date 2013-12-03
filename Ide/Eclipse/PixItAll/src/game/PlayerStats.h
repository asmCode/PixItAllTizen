#ifndef PLAYERSTATS_H_
#define PLAYERSTATS_H_

#include <string>

class PlayerStats
{
public:
	PlayerStats() :
		m_id(0),
		m_place(0),
		m_points(0),
		m_levels(0)
	{

	}

	int m_id;
	int m_place;
	std::string m_name;
	int m_points;
	int m_levels;
};


#endif /* PLAYERSTATS_H_ */
