#include "BuildingManager.h"

using namespace BWAPI;

BuildingManager::BuildingManager() : reservedMinerals(0)
{

}


void BuildingManager::build(UnitType building)
{
	m_buildingsToBuild.push(building);
}


void BuildingManager::update()
{
	if (m_buildingsToBuild.size())
	{
		UnitType building = m_buildingsToBuild.front();
		if (getAvailableMinerals() > building.mineralPrice())
		{
			//have enough minerals - for now
			//make sure we keep track of the worker so that we dont 'lose' the building if we run out of minerals
			Unit builder = getAvailableWorker();
			TilePosition targetBuildLocation = Broodwar->getBuildLocation(building, builder->getTilePosition());
			builder->build(building, targetBuildLocation);
			reservedMinerals += building.mineralPrice();
			reservedGas += building.gasPrice();
			buildingCommand cmd(builder, building, targetBuildLocation);
			m_currentlyBuilding.push_back(cmd);
			m_buildingsToBuild.pop();
		}
	}

	for (unsigned i = 0; i < m_currentlyBuilding.size(); i++)
	{
		//find out if the worker got interruptted or killed
		
	}
}

void BuildingManager::buildingStarted(Unit building)
{
	//find the building that just got started and then check it off
	for (int i = 0; i < m_currentlyBuilding.size(); i++)
	{
		if (m_currentlyBuilding[i].m_building == building->getType() && m_currentlyBuilding[i].m_buildingLocation == building->getTilePosition())
		{
			Broodwar->sendText("Building built!!");
			reservedMinerals -= building->getType().mineralPrice();
			reservedGas -= building->getType().gasPrice();
			m_currentlyBuilding.erase(m_currentlyBuilding.begin() + i);
		}
	}
}

int BuildingManager::getReservedMinerals()
{
	return reservedMinerals;
}

int BuildingManager::getReservedGas()
{
	return reservedGas;
}

int BuildingManager::getAvailableMinerals()
{
	return (Broodwar->self()->minerals() - reservedMinerals);
}

int BuildingManager::getAvailableGas()
{
	return (Broodwar->self()->gas() - reservedGas);
}

Unit BuildingManager::getAvailableWorker()
{
	//TEMPORARY
	for (auto &u : Broodwar->self()->getUnits())
	{
		if (!u->exists())
			continue;
		if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
			continue;
		if (u->isLoaded() || !u->isPowered() || u->isStuck())
			continue;
		if (!u->isCompleted() || u->isConstructing())
			continue;

		if (u->getType().isWorker() && (u->isCarryingGas() || u->isCarryingMinerals()))
		{
			return u;
		}
	}
}
