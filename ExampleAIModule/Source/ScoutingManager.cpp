#include "ScoutingManager.h"

using namespace BWAPI;

ScoutingManager::ScoutingManager() : m_startLocation(0), m_enemyLocation(0), m_scout(0)
{
	BWTA::readMap();
	BWTA::analyze();
	m_baseLocations = BWTA::getStartLocations();
	m_startLocation = BWTA::getStartLocation(Broodwar->self())->getRegion();
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
			//no more scouting
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
