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
			Unit enemy = u->getClosestUnit(IsEnemy,1000);
			if (u->isIdle())
			{
				

				if (enemy)
				{
					if (u->getType() == Terran_Siege_Tank_Tank_Mode)
					{
						//siege up first if we are a siege tank
						if (enemy->getPosition().getApproxDistance(u->getPosition()) < 400)
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
				else
				{
					if (u->getType() == Terran_Siege_Tank_Siege_Mode)
					{
						u->unsiege();
					}
					if (u->getType() == Terran_Marine)
					{
						Unit bunker = findAvailableBunker();
						if (bunker != NULL)
						{
							//go into bunker
							u->load(bunker);
						}
					}
				}
			}

			else
			{
				//make sure we attack if there is an enemy while we move
				//do specialized siege tank code here
				//if they are attacking
				Unit enemy = u->getClosestUnit(IsEnemy);
				if (enemy)
				{
					if (u->getType() == Terran_Siege_Tank_Tank_Mode)
					{
						//siege up first if we are a siege tank
						if (enemy->getPosition().getApproxDistance(u->getPosition()) < 400)
						{
							u->siege();
						}
						else
						{
							u->unsiege();
						}
					}
				}

				if (u->isAttacking() && u->getType() == Terran_Siege_Tank_Tank_Mode)
				{
					u->siege();
				}
				
			}

			//if we don't have enough army size don't do anything
			if (m_armyUnits.size() - getUnusableUnits() > 10)
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

int ArmyManager::getUnusableUnits()
{
	int counter = 0;
	for (int i = 0; i < m_armyUnits.size(); i++)
	{
		if (m_armyUnits[i]->isLoaded())
			counter++;
	}
	return counter;
}

