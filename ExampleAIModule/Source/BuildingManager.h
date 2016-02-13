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
	int m_lastCommand;
	//forgive this line
	
	buildingCommand(BWAPI::Unit buildingWorker, BWAPI::UnitType building, BWAPI::TilePosition buildingLocation) : m_buildingWorker(buildingWorker), m_building(building), m_buildingLocation(buildingLocation), m_lastCommand(0) {}
};
 
//not like building command
//used to store information about where we WANT to place the building
struct buildingQueue
{
	BWAPI::UnitType m_building;
	BWAPI::TilePosition m_desiredPosition; 

	buildingQueue(BWAPI::UnitType building, BWAPI::TilePosition desiredPosition) : m_building(building), m_desiredPosition(desiredPosition) {}
};

class BuildingManager
{
public:
	BuildingManager();

	//for when we want to queue a building
	void buildQueue(BWAPI::UnitType building, BWAPI::TilePosition place = BWAPI::TilePositions::None);
	//for when we want to build something now, fuck build orders
	void buildAsync(BWAPI::UnitType building, BWAPI::TilePosition place = BWAPI::TilePositions::None);
	//for when we want to build something now, but not fuck our build order
	void buildAfterQueue(BWAPI::UnitType building, BWAPI::TilePosition place = BWAPI::TilePositions::None);

	//this is used to REMOVE from the currently building list
	void buildingStarted(BWAPI::Unit building);
	void update();
	int getReservedMinerals();
	int getReservedGas();
	int getAvailableMinerals();
	int getAvailableGas();

	
	void setNextExpansionLocation(BWAPI::TilePosition expand);

	bool isAreaExplored(BWAPI::TilePosition area);

protected:

	BWAPI::TilePosition m_nextExpandLocation;
	void beginConstructingBuilding(buildingQueue newBldg);

	int reservedMinerals;
	int reservedGas;

	std::queue<buildingQueue> m_buildingQueue;
	std::vector<buildingQueue> m_buildingsToBuild;

	//convert this into a map or something eventually - associative array
	std::vector<buildingCommand> m_currentlyBuilding;

};



