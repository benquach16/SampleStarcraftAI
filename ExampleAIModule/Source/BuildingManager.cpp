#include "BuildingManager.h"

using namespace BWAPI;

BuildingManager::BuildingManager()
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
		if (Broodwar->self()->minerals() > building.mineralPrice())
		{
			//have enough minerals - for now
			//make sure we keep track of the worker so that we dont 'lose' the building if we run out of minerals
			Unit builder = getAvailableWorker();
			TilePosition targetBuildLocation = Broodwar->getBuildLocation(building, builder->getTilePosition());
			builder->build(building, targetBuildLocation);

			buildingCommand cmd(builder, building);
			m_currentlyBuilding.push_back(cmd);
			m_buildingsToBuild.pop();
		}
	}

	for (unsigned i = 0; i < m_currentlyBuilding.size(); i++)
	{
		buildingCommand cmd = m_currentlyBuilding[i];
	
	}
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
