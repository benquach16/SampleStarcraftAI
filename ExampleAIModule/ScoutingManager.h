#pragma once

#include <BWAPI.h>
#include <BWTA.h>

class ScoutingManager
{
public:
	void setScout(BWAPI::Unit *scout);

protected:
	BWAPI::Unit *m_scout;
	BWTA::Region *m_startLocation;
	std::queue<BWAPI::UnitType> m_buildQueue;
	std::set<BWTA::BaseLocation*> m_baseLocations;
};

