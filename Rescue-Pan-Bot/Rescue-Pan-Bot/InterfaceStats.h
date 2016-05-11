#pragma once
#include "InterfaceGeneral.h"

class InterfaceStats : public InterfaceGeneral
{
protected:
	int numSectors = 8; //number of sections to split a monster search region into
	int avoidSector1 = -1; //region of screen to ignore when seraching due to a taken monster
	int avoidSector2 = -1; //region of screen to ignore when seraching due to a taken monster
	int avoidPointer = 1; // pointer to which avoid sector to update
	int attackMenuOption = 0; //what menu option number the attack event is
	int combatTimeout = 25; //number of consecutive times it can find itself not in combat
	int sectorTimeout = 5; //number of times before avoided sectors are nulled.
	int attackTimeout = 0; // the miliseconds of wait after an attack successful clicked
	int mouseDeviation = 250;

public:
	bool fightme = 0; //if im set to 1, logns sleep for gobbys
	unsigned int alternateColor = -1;
	bool MouseMoved = false; // for mouse movement returning
	//returns '0' if not in combat, '1' if in combat, and '2' if monster is already taken by someone else, and '3' if the monster is just dying
	int VerifyCombat();

	//different options of combat verifying. 
	// '0' is loose verification. returns true on a number of combat identifiers
	// '1' is strict. it returns on XP bubble and only once youve dealt damage to something
	int VerifyCombat(int mode);

	//if already in combat, this checks for when the monster reaches 0/0 health
	bool CheckEnemyDead();

	//finds an enemy in some region, moves mouse to enemy. if once mouse moved, hover checks that enemy is there  rightclicks Returns true. else false
	bool FindEnemy(unsigned int color, int x1, int y1, int x2, int y2);


	//finds an enemy in some region, moves mouse to enemy. if once mouse moved, hover checks that enemy is there  rightclicks Returns true. else false
	bool FindEnemy(unsigned int color, Area region);

	//attempts to attack a found enemy. Assumes potential enemy has already been right clicked.
	bool Attack();

	//attempts to attack a found enemy. Assumes potential enemy has already been right clicked.
	//searches for a level color (green if you're a high level) as well
	bool Attack(unsigned int levelColor);

	//does sector searching. this is in attempt to not always choose top left monster, and to avoid getting stuck on taken monsters.
	//takes in color ofa  monster to fight, as well as teh area to look for said monster. handles all combat stuff
	bool Fight(unsigned int color, int x1, int y1, int x2, int y2);
	
	//area version of the above
	bool Fight(unsigned int color, Area region);

	//does everything the other fight does, but makes sure you see the color of text like "(Lvl 13)"
	bool Fight(unsigned int color, int x1, int y1, int x2, int y2, unsigned int levelColor);

	//takes a selected area and returns a specific sector of said area.
	//returns area of just that sector.
	Area GetSectorCoords(Area region, int sector);

	//returns what sector the coordinates belong to
	int GetSector(Area region, POINT coords);

	//returns true if color is found in bubble
	bool CheckExperienceCircle(unsigned int color, int numBubbles);

	//Bloom Sectoring is CENTER sectoring. it starts search region at center and expands outward
	//takes a selected area and returns a specific sector of said area.
	//returns area of just that sector.
	Area GetBLOOMSectorCoords(Area region, int sector);

	//searches for an enemy using bloom sectoring
	bool SearchEnemyBloom(unsigned int color, int x1, int y1, int x2, int y2);

	//checks for health bar on a monster
	bool CheckMonsterHealthBar(POINT monster);

	//checks for a monsters second color, for more reliable finding
	bool CheckMonster(POINT monster, unsigned int altColor);

	//detects when you move in game
	bool DetecMovement();

	//Verifies that runescape is up
	bool VerifyRunescapeWindow();

	//given a color, returns true if a dot of said color is on the minimap
	// red = item, yellow=npc, white = player
	// NEEDS WORK
	bool MiniMapDot(unsigned int color);

	//DONT USE ME IM BAD
	bool CheckBorderingPixels(unsigned int color, POINT point, int& counter);

	//returns true if run is on.
	bool checkRunOn() {
		return PixelHandler().VerifyPixelColor(0xD9B43B00, 3418 - 1920 + SCREEN, 177, 2);
	}

};