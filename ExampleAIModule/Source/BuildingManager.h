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
	buildingCommand(BWAPI::Unit buildingWorker, BWAPI::UnitType building, BWAPI::TilePosition buildingLocation) : m_buildingWorker(buildingWorker), m_building(building), m_buildingLocation(buildingLocation) {}
};

class BuildingManager
{
public:

	BuildingManager();
	void build(BWAPI::UnitType building);
	void buildingStarted(BWAPI::Unit building);
	void update();
	int getReservedMinerals();
	int getReservedGas();
	int getAvailableMinerals();
	int getAvailableGas();

	BWAPI::Unit getAvailableWorker();

protected:

	int reservedMinerals;
	int reservedGas;

	std::queue<BWAPI::UnitType> m_buildingsToBuild;
	std::vector<buildingCommand> m_currentlyBuilding;

};

