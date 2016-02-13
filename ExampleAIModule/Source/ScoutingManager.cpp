#include "ScoutingManager.h"
#include "Util.h"
using namespace BWAPI;

ScoutingManager::ScoutingManager() : m_startLocation(0), m_enemyLocation(0), m_scout(0)
{
	BWTA::readMap();
	BWTA::analyze();
	m_baseLocations = BWTA::getStartLocations();
	m_startLocation = BWTA::getStartLocation(Broodwar->self())->getRegion();
	m_allBases = BWTA::getBaseLocations();
}

void ScoutingManager::setScout(BWAPI::Unit scout)
{
	m_scout = scout;
}

void ScoutingManager::removeScout()
{
	if (m_scout)
	{
		m_scout->stop();
		m_scout = 0;
	}
}

void ScoutingManager::setEnemyMain(BWTA::Region *enemyLocation)
{
	Broodwar->sendText("Found enemy base!");
	m_enemyLocation = enemyLocation;
}

void ScoutingManager::update()
{
	//go to each scouting location
	//and find enemy main
	if (m_scout && m_enemyLocation)
	{
		//no more scouting
		//go home
		m_scout->move(m_startLocation->getCenter());
		removeScout();
	}
	if (m_scout && !m_enemyLocation)
	{
		
		if (m_baseLocations.size())
		{
			auto i = m_baseLocations.begin();
			m_scout->move((*i)->getPosition());
			if (m_scout->getPosition().getApproxDistance((*i)->getPosition()) < 100)
			{
				//close enough so knock it off the list

				m_baseLocations.erase(m_baseLocations.begin());
				i = m_baseLocations.begin();
			}

		}
		else
		{
			//this happens when you run out of bases to search but didnt find the enemy
			//invalid map then
		}
	}


}

bool ScoutingManager::hasScout()
{
	return m_scout;
}

bool ScoutingManager::foundEnemyMain()
{
	return m_enemyLocation;
}

Position ScoutingManager::getEnemyLocation()
{
	return m_enemyLocation->getCenter();
}

TilePosition ScoutingManager::getNextExpansionLocation(TilePosition currentExpansion)
{
	//loop through expos and find the nearest one
	std::set<BWTA::BaseLocation*> bases;
	
	for (auto &u : m_allBases)
	{
		//make sure we delete the current expansion
		if (u->getTilePosition() == currentExpansion)
		{
			m_allBases.erase(u);
		}
	}
	BWTA::BaseLocation *ret = *m_allBases.begin();

	int minDistance = currentExpansion.getApproxDistance(ret->getTilePosition());

	//make sure we dont accidently find current expansion
	for (auto &u : m_allBases)
	{
		int tempDistance = currentExpansion.getApproxDistance(u->getTilePosition());
		if (tempDistance < minDistance && tempDistance > 10)
		{
			//found a closer base
			minDistance = tempDistance;
			ret = u;
		}
	}
	return ret->getTilePosition();
}
