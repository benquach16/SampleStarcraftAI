#pragma once

#include <BWAPI.h>
#include <BWTA.h>
#include <set>

//Utility functions go here


BWAPI::TilePosition getClosestExpansion(BWAPI::TilePosition currentExpo);
//find closest chokepoint to this position
BWAPI::TilePosition getChokepoint(BWAPI::TilePosition currentChoke);
BWAPI::Unit getAvailableWorker();
BWAPI::Unit findAvailableBunker();

