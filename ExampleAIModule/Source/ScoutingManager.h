#pragma once

#include <BWAPI.h>
#include <BWTA.h>
#include <queue>

class ScoutingManager
{
public:
	ScoutingManager();
	void setScout(BWAPI::Unit scout);
	void removeScout();
	void setEnemyMain(BWTA::Region *enemyLocation);
	void update();
	bool hasScout();
	bool foundEnemyMain();

	BWAPI::Position getEnemyLocation();
protected:
	BWAPI::Unit m_scout;
	BWTA::Region *m_startLocation;
	BWTA::Region *m_enemyLocation;
	std::set<BWTA::Region*> m_enemyLocations;
	std::queue<BWAPI::UnitType> m_buildQueue;
	std::set<BWTA::BaseLocation*> m_baseLocations;
};

