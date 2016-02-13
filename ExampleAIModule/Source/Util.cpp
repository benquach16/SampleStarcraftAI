#include "Util.h"

using namespace BWAPI;
using namespace BWTA;

//This exists so we can make sure we dont accidently return an expansion that already has been expanded too
std::set<BWTA::BaseLocation*> allBases;

void initBases()
{
	allBases = BWTA::getBaseLocations();
}

BWAPI::TilePosition getNextExpansionLocation(TilePosition currentExpo)
{
	//remove the current expo from the list
	//loop through expos and find the nearest one
	std::set<BWTA::BaseLocation*> bases;

	for (auto &u : allBases)
	{
		//make sure we delete the current expansion
		if (u->getTilePosition() == currentExpo)
		{
			allBases.erase(u);
		}
	}
	BWTA::BaseLocation *ret = *allBases.begin();

	int minDistance = currentExpo.getApproxDistance(ret->getTilePosition());

	//make sure we dont accidently find current expansion
	for (auto &u : allBases)
	{
		int tempDistance = currentExpo.getApproxDistance(u->getTilePosition());
		if (tempDistance < minDistance && tempDistance > 10)
		{
			//found a closer base
			minDistance = tempDistance;
			ret = u;
		}
	}
	return ret->getTilePosition();
}


BWAPI::TilePosition getChokepoint(TilePosition currentChoke)
{
	int minDistance = 99999;

	std::set<BWTA::Chokepoint*> chokepoints = BWTA::getChokepoints();
	for (auto &u : chokepoints)
	{

	}
}

BWAPI::Unit getAvailableWorker()
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

BWAPI::Unit findAvailableBunker()
{
	//search for a not full bunker
	for (auto &u : Broodwar->self()->getUnits())
	{
		if (u->getType() == UnitTypes::Terran_Bunker)
		{
			if (u->getLoadedUnits().size() < UnitTypes::Terran_Bunker.spaceProvided())
			{
				return u;
			}
		}
	}

	//no bunker
	return NULL;
}
