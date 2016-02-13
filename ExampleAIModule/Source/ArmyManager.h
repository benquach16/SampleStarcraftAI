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


	int getNumUnits(BWAPI::UnitType unittype);
	//find units in bunkers, transport ships, etc
	int getUnusableUnits();
protected:
	std::vector<BWAPI::Unit> m_armyUnits;
};
