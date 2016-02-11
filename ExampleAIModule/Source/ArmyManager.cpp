#include "ArmyManager.h"
#include "Util.h"

using namespace BWAPI;
using namespace Filter;
using namespace UnitTypes;

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
			//automatically attack close units

			Unit u = m_armyUnits[i];
			Unit enemy = u->getClosestUnit(IsEnemy);
			if (u->isIdle())
			{
				

				if (enemy)
				{
					if (u->getType() == Terran_Siege_Tank_Tank_Mode)
					{
						//siege up first if we are a siege tank
						if (enemy->getPosition().getApproxDistance(u->getPosition()) < 3000)
						{
							u->siege();
						}
						else
						{
							u->unsiege();
						}
					}
					else
					{

						u->attack(enemy);
					}
				}
				else if (u->getType() == Terran_Marine)
				{
					Unit bunker = findAvailableBunker();
					if (bunker != NULL)
					{
						//go into bunker
						u->load(bunker);
					}
				}
			}

			//if we don't have enough army size don't do anything
			if (m_armyUnits.size() > 7)
			{
				//but if we do, attack
				if (u->isIdle())
				{
		
					//attack enemy location
					u->attack(enemyLocation);
					
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

int ArmyManager::getNumUnits(BWAPI::UnitType unittype)
{
	int counter = 0;
	for (int i = 0; i < m_armyUnits.size(); i++)
	{
		if (m_armyUnits[i]->getType() == unittype)
		{
			counter++;
		}
	}
	return counter;
}

