#include "BuildingManager.h"

#include "Util.h"

using namespace BWAPI;

BuildingManager::BuildingManager() : reservedMinerals(0), reservedGas(0)
{

}


void BuildingManager::buildQueue(UnitType building, TilePosition place)
{
	
	buildingQueue newBldg(building, place);
	m_buildingQueue.push(newBldg);
}

void BuildingManager::buildAsync(UnitType building, TilePosition place)
{
	buildingQueue newBldg(building, place);
	m_buildingsToBuild.push_back(newBldg);
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
		UnitType building = m_buildingQueue.front().m_building;
		if (getAvailableMinerals() >= building.mineralPrice() && getAvailableGas() >= building.gasPrice())
		{
			std::string str("building " + building.getName());
			Broodwar->sendText(str.c_str());
			beginConstructingBuilding(m_buildingQueue.front());
			m_buildingQueue.pop();
		}
	}

	//iterate through async building queue
	for (unsigned i = 0; i < m_buildingsToBuild.size();)
	{
		buildingQueue newBldg = m_buildingsToBuild[i];
		BWAPI::UnitType building = newBldg.m_building;
		if (getAvailableMinerals() >= building.mineralPrice() && getAvailableGas() >= building.gasPrice())
		{
			beginConstructingBuilding(newBldg);
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
			Broodwar->sendText(m_currentlyBuilding[i].m_building.getName().c_str());
			//Broodwar << Broodwar->getLastError() << std::endl;
			//dont think this line is needed
			//m_currentlyBuilding[i].m_buildingWorker = getAvailableWorker();
			//
			//setup a timer
			if (!isAreaExplored(m_currentlyBuilding[i].m_buildingLocation))
			{


				Broodwar->sendText("area not explored?");
				m_currentlyBuilding[i].m_buildingWorker->move(Position(m_currentlyBuilding[i].m_buildingLocation));
			}
			else
			{
				
				Broodwar->sendText("area is explored?");
				//find a new bldg location
				if (m_currentlyBuilding[i].m_lastCommand < Broodwar->getFrameCount())
				{
					//give some time to go explore
					m_currentlyBuilding[i].m_buildingLocation = Broodwar->getBuildLocation(m_currentlyBuilding[i].m_building, m_currentlyBuilding[i].m_buildingWorker->getTilePosition());
					m_currentlyBuilding[i].m_buildingWorker->build(m_currentlyBuilding[i].m_building, m_currentlyBuilding[i].m_buildingLocation);
					m_currentlyBuilding[i].m_lastCommand = Broodwar->getFrameCount() + 100;
				}
			}


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


void BuildingManager::setNextExpansionLocation(TilePosition expand)
{
	m_nextExpandLocation = expand;
}

void BuildingManager::beginConstructingBuilding(buildingQueue newBldg)
{
	//have enough minerals - for now
	//make sure we keep track of the worker so that we dont 'lose' the building if we run out of minerals
	UnitType building = newBldg.m_building;
	Unit builder = getAvailableWorker();
	if (builder)
	{
		TilePosition targetBuildLocation;
		if (building.isResourceDepot())
		{
			//build this at an expo
			//find closest build location
			targetBuildLocation = m_nextExpandLocation;
		}
		else
		{
			//do we build at custom location
			if (newBldg.m_desiredPosition == TilePositions::None)
			{

				targetBuildLocation = Broodwar->getBuildLocation(building, builder->getTilePosition());
			}
			else
			{

				targetBuildLocation = Broodwar->getBuildLocation(building, newBldg.m_desiredPosition);
			}
		}
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

bool BuildingManager::isAreaExplored(TilePosition area)
{

	return Broodwar->isExplored(area);

}
