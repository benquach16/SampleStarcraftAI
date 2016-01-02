#pragma once

#include <BWAPI.h>
#include <queue>
#include <vector>
#include <BWTA.h>

struct buildingCommand
{
	BWAPI::Unit m_buildingWorker;
	BWAPI::UnitType m_building;
	BWAPI::TilePosition m_buildingLocation;

	buildingCommand(BWAPI::Unit buildingWorker, BWAPI::UnitType building) : m_buildingWorker(buildingWorker), m_building(building) {}
};

class BuildingManager
{
public:

	BuildingManager();
	void build(BWAPI::UnitType building);
	void update();

	BWAPI::Unit getAvailableWorker();

protected:
	std::queue<BWAPI::UnitType> m_buildingsToBuild;
	std::vector<buildingCommand> m_currentlyBuilding;

};

