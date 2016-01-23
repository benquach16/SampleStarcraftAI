#include "ArmyManager.h"

using namespace BWAPI;
using namespace Filter;

ArmyManager::ArmyManager()
{

}

void ArmyManager::update(BWAPI::Position enemyLocation)
{
	for (unsigned i = 0; i < m_armyUnits.size();)
	{
		if (!m_armyUnits[i]->exists())
		{
			//remove
			m_armyUnits.erase(m_armyUnits.begin() + i);
		}
		else
		{
			//do army stuff
			//if we don't have enough army size don't do anything
			if (m_armyUnits.size() > 10)
			{
				Unit u = m_armyUnits[i];
				//but if we do, attack
				if (u->isIdle())
				{
					Unit enemy = u->getClosestUnit(IsEnemy);
					if (enemy)
					{
						u->attack(enemy);
					}
					else
					{
						//attack enemy location
						u->attack(enemyLocation);
					}
				}

			}
			i++;
		}
	}
}

void ArmyManager::addArmyUnit(BWAPI::Unit unit)
{
	m_armyUnits.push_back(unit);
}

