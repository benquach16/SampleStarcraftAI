#pragma once

#include <BWAPI.h>
#include <BWTA.h>
#include <set>

//Utility functions go here


BWAPI::TilePosition getClosestExpansion(BWAPI::TilePosition currentExpo);
BWAPI::TilePosition getChokepoint(BWAPI::TilePosition currentChoke);
BWAPI::Unit getAvailableWorker();

