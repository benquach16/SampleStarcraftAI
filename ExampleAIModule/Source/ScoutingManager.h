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


	BWAPI::TilePosition getNextExpansionLocation(BWAPI::TilePosition currentExpansion);
protected:
	BWAPI::Unit m_scout;
	BWTA::Region *m_startLocation;
	BWTA::Region *m_enemyLocation;
	std::set<BWTA::Region*> m_enemyLocations;
	std::queue<BWAPI::UnitType> m_buildQueue;
	//this is for scouting
	std::set<BWTA::BaseLocation*> m_baseLocations;
	//dont modify this
	std::set<BWTA::BaseLocation*> m_allBases;
};

