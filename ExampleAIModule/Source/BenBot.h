#pragma once
#include <BWAPI.h>
#include <queue>
#include <BWTA.h>

#include "ScoutingManager.h"
#include "BuildingManager.h"
#include "ArmyManager.h"


// Remember not to use "Broodwar" in any global class constructor!

class ExampleAIModule : public BWAPI::AIModule
{
public:
	// Virtual functions for callbacks, leave these as they are.
	virtual void onStart();
	virtual void onEnd(bool isWinner);
	virtual void onFrame();
	virtual void onSendText(std::string text);
	virtual void onReceiveText(BWAPI::Player player, std::string text);
	virtual void onPlayerLeft(BWAPI::Player player);
	virtual void onNukeDetect(BWAPI::Position target);
	virtual void onUnitDiscover(BWAPI::Unit unit);
	virtual void onUnitEvade(BWAPI::Unit unit);
	virtual void onUnitShow(BWAPI::Unit unit);
	virtual void onUnitHide(BWAPI::Unit unit);
	virtual void onUnitCreate(BWAPI::Unit unit);
	virtual void onUnitDestroy(BWAPI::Unit unit);
	virtual void onUnitMorph(BWAPI::Unit unit);
	virtual void onUnitRenegade(BWAPI::Unit unit);
	virtual void onSaveGame(std::string gameName);
	virtual void onUnitComplete(BWAPI::Unit unit);
	// Everything below this line is safe to modify.

protected:
	bool followingBuildOrder;
	
	std::vector<BWAPI::Unit> m_expansions;

	BWAPI::Position m_enemyLocation;
	BWAPI::TilePosition m_currentExpansionLocation;

	ScoutingManager m_scoutingManager;
	BuildingManager m_buildingManager;
	ArmyManager m_armyManager;
	
	//temmporary = have a manager for this!!!
	int m_workersMiningGas;
	int m_armyUnits;
};
