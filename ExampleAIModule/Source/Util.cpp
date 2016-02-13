#include "Util.h"

using namespace BWAPI;
using namespace BWTA;



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
