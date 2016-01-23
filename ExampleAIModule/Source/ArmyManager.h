#pragma once

#include <BWAPI.h>
#include <vector>

//this class does micro and keeps track of our army units
//so that we can 'reload' the army once everything dies
class ArmyManager
{
public:
	ArmyManager();
	void update(BWAPI::Position enemyLocation);

	void addArmyUnit(BWAPI::Unit unit);
protected:
	std::vector<BWAPI::Unit> m_armyUnits;
};
