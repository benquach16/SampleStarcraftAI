#include "BuildingManager.h"

using namespace BWAPI;

BuildingManager::BuildingManager() : reservedMinerals(0), reservedGas(0)
{

}


void BuildingManager::buildQueue(UnitType building)
{
	m_buildingQueue.push(building);
}

void BuildingManager::buildAsync(UnitType building)
{
	m_buildingsToBuild.push_back(building);
}


void BuildingManager::update()
{
	//check if a refinery is done
	for (auto &u : Broodwar->self()->getUnits())
	{
		if (!u->getType().isBuilding() || !u->isBeingConstructed())
		{
			continue;
		}
		buildingStarted(u);
		
	}


	if (m_buildingQueue.size())
	{
		UnitType building = m_buildingQueue.front();
		if (getAvailableMinerals() >= building.mineralPrice() && getAvailableGas() >= building.gasPrice())
		{
			std::string str("building " + building.getName());
			Broodwar->sendText(str.c_str());
			beginConstructingBuilding(building);
			m_buildingQueue.pop();
		}
	}

	//iterate through async building queue
	for (unsigned i = 0; i < m_buildingsToBuild.size();)
	{
		UnitType building = m_buildingsToBuild[i];
		if (getAvailableMinerals() >= building.mineralPrice() && getAvailableGas() >= building.gasPrice())
		{
			beginConstructingBuilding(building);
			//remove from vector
			m_buildingsToBuild.erase(m_buildingsToBuild.begin() + i);
		}
		else
		{
			i++;
		}
	}

	for (unsigned i = 0; i < m_currentlyBuilding.size(); i++)
	{
		
		//find out if the worker got interrupted or killed on the way
		if (!m_currentlyBuilding[i].m_buildingWorker->exists())
		{
			//worker died before building started :(
			//find another worker and restart
			m_currentlyBuilding[i].m_buildingWorker = getAvailableWorker();
			m_currentlyBuilding[i].m_buildingWorker->build(m_currentlyBuilding[i].m_building, m_currentlyBuilding[i].m_buildingLocation);
			
		}
		else if (m_currentlyBuilding[i].m_buildingWorker->isGatheringMinerals() || m_currentlyBuilding[i].m_buildingWorker->isGatheringGas())
		{
			//THIS SHOULD NOT HAPPEN


			//that bitch went back to mining
			//must have somehow hit a race condition with minerals again or for some reason just did not start mining
			//so restart
			Broodwar->sendText("wtf?");
			Broodwar->sendText(m_currentlyBuilding[i].m_building.getName().c_str());

			//dont think this line is needed
			//m_currentlyBuilding[i].m_buildingWorker = getAvailableWorker();
			m_currentlyBuilding[i].m_buildingWorker->stop();
			m_currentlyBuilding[i].m_buildingWorker->build(m_currentlyBuilding[i].m_building, m_currentlyBuilding[i].m_buildingLocation);
		}
		else
		{
			//uncomment this if you have a bug where a building is stuck in the queue or if you just like being spammed
			//Broodwar->sendText("Something still in build queue!");
		}
		
	}
}

void BuildingManager::buildingStarted(Unit building)
{
	//find the building that just got started and then check it off
	for (unsigned i = 0; i < m_currentlyBuilding.size(); i++)
	{
		if (m_currentlyBuilding[i].m_building == building->getType() && m_currentlyBuilding[i].m_buildingLocation == building->getTilePosition())
		{
			Broodwar->sendText("Building started!!");
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
	//TEMPORARY FOR REAL
	//GET A WAY TO FIND THE CLOSEST WORKER TO A BASE SINCE IT MIGHT GRAB THE SCOUTING SCV
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

		if (u->getType().isWorker() && u->isGatheringMinerals())
		{
			return u;
		}
	}
	return 0;
}

void BuildingManager::beginConstructingBuilding(BWAPI::UnitType building)
{
	//have enough minerals - for now
	//make sure we keep track of the worker so that we dont 'lose' the building if we run out of minerals
	Unit builder = getAvailableWorker();
	if (builder)
	{
		TilePosition targetBuildLocation = Broodwar->getBuildLocation(building, builder->getTilePosition());
		builder->build(building, targetBuildLocation);

		//reserve the minerals because race conditions
		reservedMinerals += building.mineralPrice();
		reservedGas += building.gasPrice();

		//add this to a list of currently constructing buildings
		buildingCommand cmd(builder, building, targetBuildLocation);
		m_currentlyBuilding.push_back(cmd);
	}
	else
	{
		Broodwar->sendText("This should not happen");
	}
}
