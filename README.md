BenBot
==========

BenBot is a Starcraft: BroodWar AI bot using the BWAPI framework. Intended as an example for the ACM club at UC Riverside,
it's also my personal bot. So it can also be semi-competitive.

The main goal of the bot is to show off handling building queues in Starcraft Broodwar without having to deal with
worker issues. It does this by reserving minerals and checking building statuses to see if a building has started.


Code Architecture
---------

There are two other classes - BuildingManager and ScoutingManager

Currently the hiearchy works as follows

    BenBot 
	
	| ------------------------------------------
	|                    \                      \
	
    ScoutingManager     BuildingManager             ArmyManager
	
