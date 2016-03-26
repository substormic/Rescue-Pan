#include "InterfaceStats.h"

int InterfaceStats::VerifyCombat()
{
	unsigned int RedTL = 0x87332600;
	unsigned int GreenTL = 0x38802600;
	unsigned int taken = 0xd4d0be00;


	if (CheckEnemyDead())
		return 3;

	//check that if monster targeted, that said monster is not taken
	if ((pix.SearchPixelArea(RedTL, 10 + SCREEN, 71, 131 + SCREEN, 86, 15)) && (pix.SearchPixelArea(GreenTL, 10 + SCREEN, 71, 131 + SCREEN, 86, 15)))
	{
		if (pix.SearchPixelArea(taken, 10 + SCREEN, 87, 22 + SCREEN, 96, 25))
			return 2;
	}

	//check for red AND green in health bar at TopLeft!
	if ((pix.SearchPixelArea(RedTL, 10 + SCREEN, 71, 131 + SCREEN, 86, 15)) && (pix.SearchPixelArea(GreenTL, 10 + SCREEN, 71, 131 + SCREEN, 86, 15)))
		return 1;

	//check for red AND green in health bar at TopLeft!
	//if (pix.SearchPixelArea(RedTL, 10 + SCREEN, 71, 131 + SCREEN, 86, 15))
	//	return 1;

	//check for green in health bar at TopLeft!
	//if (pix.SearchPixelArea(GreenTL, 10 + SCREEN, 71, 131 + SCREEN, 86, 15))
	//	return 1;

	//check the red heart xp gain
	if (pix.VerifyPixelColor(0xdd4f0100, 843 + SCREEN, 63))
		return 1;

	return 0;
}

int InterfaceStats::VerifyCombat(int mode)
{
	unsigned int RedTL = 0x87332600;
	unsigned int GreenTL = 0x38802600;
	unsigned int taken = 0xd4d0be00;
	unsigned int taken2 = 0xd49e3400;
	unsigned int taken3 = 0x34180200;

	if (CheckEnemyDead()) //check dead before
		return 3;

	//check that if monster targeted, that said monster is not taken
	if ((pix.SearchPixelArea(RedTL, 10 + SCREEN, 71, 131 + SCREEN, 86, 15)) || (pix.SearchPixelArea(GreenTL, 10 + SCREEN, 71, 131 + SCREEN, 86, 15)))
	{
		if (pix.SearchPixelArea(taken, 10 + SCREEN, 87, 22 + SCREEN, 96, 25)) //checks if taken
		{
			bool takenmob = true;
			takenmob &= pix.SearchPixelArea(taken2, 10 + SCREEN, 87, 22 + SCREEN, 96, 25); //checks if taken for sure orange crossgaurs
			takenmob &= pix.SearchPixelArea(taken3, 10 + SCREEN, 87, 22 + SCREEN, 96, 25); //checks if taken for sure brown hilt
			if (takenmob) //if all pixels found, shits taken yo
				return 2;
		}
	}

	if (mode == 0) // LOOSE targeting
	{
		//check for red -- OR -- green in health bar at TopLeft!
		if ((pix.SearchPixelArea(RedTL, 10 + SCREEN, 71, 131 + SCREEN, 86, 15)) || (pix.SearchPixelArea(GreenTL, 10 + SCREEN, 71, 131 + SCREEN, 86, 18)))
			return 1;

	}
	else if (mode == 1) // STRICT targeting
	{

		//check for red -- AND -- green in health bar at TopLeft!
		if ((pix.SearchPixelArea(RedTL, 10 + SCREEN, 71, 131 + SCREEN, 86, 15)) && (pix.SearchPixelArea(GreenTL, 10 + SCREEN, 71, 131 + SCREEN, 86, 18)))
			return 1;
	}



	//check the red heart xp gain
	if (pix.VerifyPixelColor(0xdd4f0100, 843 + SCREEN, 63))
		return 1;

	return 0;
}

bool InterfaceStats::CheckEnemyDead()
{
	//check for red in health bar at TopLeft!
	if (pix.SearchPixelArea(0x87332600, 10 + SCREEN, 71, 11 + SCREEN, 86, 18))
		return true;
	return false;
}

//locates an enemy given its a color and a region it should be found in, rightclicks them
bool InterfaceStats::FindEnemy(unsigned int color, int x1, int y1, int x2, int y2)
{
	if (x1 > 1920) //check if coords were taken on the right screen
	{
		x1 -= 1920; //normalize to left screen
		x2 -= 1920; //normalize to left screen
	}
	POINT Enemy;
	Enemy = pix.SearchPixelAreaForPoint(color, x1+SCREEN,y1,x2+SCREEN,y2,10);

	if (Enemy.x == -1) //no Enemy found
		return false;

	mouse.SetDeviation(mouseDeviation); // set the mouse deviatons
	mouse.MouseMoveArea(Enemy.x-4,Enemy.y-4,Enemy.x+4,Enemy.y+4);
	Sleep(300);
	if (VerifyTopLeftText(HOVER_NPC) || VerifyHoverText(HOVER_NPC))//confirm mouse on Enemy
	{
		mouse.RightClick();
		return true;
	}
	return false;
}

//locates an enemy given its a color and a region it should be found in, rightclicks them
bool InterfaceStats::FindEnemy(unsigned int color, Area region)
{
	return FindEnemy(color,region.x1, region.y1, region.x2, region.y2);
}

//starts assuming an enemy is right clicked
bool InterfaceStats::Attack()
{
	Sleep(200);
	if (ChooseMenuOptionColorCheck(attackMenuOption, HOVER_NPC)) //if the menu option is has npc colors
	{
		mouse.LeftClick();
		attackTimeout = 500;
		//Sleep(4500); //allow for new combat to update
		return true;
	}
	return false;
}

//attempts to fight as long as not in combat
bool InterfaceStats::Fight(unsigned int color, int x1, int y1, int x2, int y2)
{
	MouseMoved = false;
	Area region(x1, y1, x2, y2);
	Area sector;
	SetMouseSpeed(0.3);

	if (attackTimeout > 0)
	{
		Sleep(50);
		attackTimeout -= 50;
		return true;
	}

	if ((sectorTimeout <= 0) || (combatTimeout < 4)) //reset avoided sectors
	{
		printf("RESETTING SECTORS\n");
		avoidSector1 = -1;
		avoidSector2 = -1;
		sectorTimeout = 5;
	}
	if (combatTimeout <= 0)
	{
		printf("=========Somethings Gone Wrong, NO COMBAT. exiting.\n");
		return false; //end program, cannot find combat for whatever reason
	}

	int combatStatus = VerifyCombat(1); // ===================================== SET LOOSE OR STRICT COMBAT HERE
	if (combatStatus == 0) //Not in Combat
	{
		if (combatTimeout > 0)//not in combat, decrease combat timeout by one
			combatTimeout--;
		printf("Not in combat, lets start a fight\n");
		for (int i = 0; i < numSectors; i++) //iterate through the sectors looking for mobs.
		{
			if ((i == avoidSector1) || (i == avoidSector2)) //if avoidedsector. skip this iteration
				continue;
			sector = GetSectorCoords(region, i);
			if (FindEnemy(color, sector.x1, sector.y1, sector.x2, sector.y2))
			{
				if (alternateColor != -1 && !CheckMonster(mouse.GetPosition(), alternateColor))
				{
					printf("What is this? Not a valid monster thats for sure\n");
					continue;
				}
				if (CheckMonsterHealthBar(mouse.GetPosition()))
				{
					printf("This monster seems to be taken, judging by its health bar\n");
					continue;
				}
				if (Attack())
				{
					if (sectorTimeout > 0)
						sectorTimeout--; //no sector issues found here
					printf("Attacked an enemy!\n");
					MouseMoved = true;
					return true;
				}
			}
		}
	}
	else if (combatStatus == 2) //monster is taken, do the region splitting
	{
		sectorTimeout = 5;//replenish sectorTimeout
		POINT TakenEnemy = mouse.GetPosition();
		if (avoidPointer == 1)
		{
			avoidSector1 = GetSector(region, TakenEnemy); //block sector where taken enemy was found
			printf("Monster taken, now avoiding (#1) sector %i\n", avoidSector1);
			avoidPointer = 2;
		}
		else
		{
			avoidSector2 = GetSector(region, TakenEnemy); //block sector where taken enemy was found
			printf("Monster taken, now avoiding (#2) sector %i\n", avoidSector2);
			avoidPointer = 1;
		}
		for (int i = 0; i < numSectors; i++) //iterate through the sectors looking for enemies.
		{
			if ((i == avoidSector1) || (i == avoidSector2)) //if avoidedsector. skip this iteration
				continue;
			sector = GetSectorCoords(region, i);
			if (FindEnemy(color, sector.x1, sector.y1, sector.x2, sector.y2))
			{
				if (alternateColor != -1 && !CheckMonster(mouse.GetPosition(), alternateColor))
				{
					printf("What is this? Not a valid monster thats for sure\n");
					continue;
				}
				if (CheckMonsterHealthBar(mouse.GetPosition()))
				{
					printf("This monster seems to be taken, judging by its health bar\n");
					continue;
				}
				if (Attack())
				{
					printf("Attacked an enemy!\n");
					MouseMoved = true;
					return true;
				}
			}
		}
	}

	else if (combatStatus == 3) //enemy just slain!
	{
		printf("One down, lets get another\n");
		for (int i = 0; i < numSectors; i++) //iterate through the sectors looking for mobs.
		{
			if ((i == avoidSector1) || (i == avoidSector2)) //if avoidedsector. skip this iteration
				continue;
			sector = GetSectorCoords(region, i);
			if (FindEnemy(color, sector.x1, sector.y1, sector.x2, sector.y2))
			{
				if (alternateColor != -1 && !CheckMonster(mouse.GetPosition(), alternateColor))
				{
					printf("What is this? Not a valid monster thats for sure\n");
					continue;
				}
				if (CheckMonsterHealthBar(mouse.GetPosition()))
				{
					printf("This monster seems to be taken, judging by its health bar\n");
					continue;
				}
				if (Attack())
				{
					if (sectorTimeout > 0)
						sectorTimeout--; //no sector issues found here
					printf("Attacked an enemy!\n");
					MouseMoved = true;
					return true;
				}
			}
		}
	}
	else //in combat
	{
		combatTimeout = 25;
		return true;
	}
	MouseMoved = false;
	return true;
}

//attempts to fight as long as not in combat
bool InterfaceStats::Fight(unsigned int color, Area region)
{
	
	return Fight(color, region.x1, region.y1, region.x2, region.y2);
}

//takes a selected area and returns a specific sector of said area.
//returns area of just that sector.
//*NOTE splits into vertical sectors. Sectors are 0 indexed
Area InterfaceStats::GetSectorCoords(Area region, int sector)
{
	Area Sector;
	int deltaX = region.x2 - region.x1;
	//int deltaY = region.y2 - region.y1; use if you want non-vertical sectors
	float sectorX = deltaX / numSectors; //partition the area into sector sized chunks
	Sector.x1 = region.x1 + (sectorX*sector);
	Sector.x2 = region.x1 + (sectorX*(sector + 1));
	Sector.y1 = region.y1;
	Sector.y2 = region.y2;

	return Sector;
}

//returns what sector the coordinates belong to
int InterfaceStats::GetSector(Area region, POINT coords)
{
	int sector = 0;
	int deltaX = region.x2 - region.x1;
	//int deltaY = region.y2 - region.y1; use if you want non-vertical sectors
	float sectorX = deltaX / numSectors; //partition the area into sector sized chunks
	float inSector = coords.x - region.x1;
	inSector = inSector / sectorX;
	
	sector = (int)inSector;
	if (sector < 0)
		sector = 0;

	if (sector > numSectors)
		sector = numSectors;
	return sector;

}


//returns true if color is found in bubble 
bool InterfaceStats::CheckExperienceCircle(unsigned int color, int numBubbles)
{
	bool check = true;
	if (numBubbles = 1)
	{
		check &= pix.SearchPixelArea(color, 800 + SCREEN, 60, 830 + SCREEN, 70, 10);
		//check &= pix.SearchPixelArea(0x4b483700, 800 + SCREEN, 60, 830 + SCREEN, 70,50); //checks background of bubble
	}
	return check;
}

//takes a selected area and returns a specific sector of said area.
//returns area of just that sector.
//*NOTE splits into vertical sectors. Sectors are 0 indexed
Area InterfaceStats::GetBLOOMSectorCoords(Area region, int sector)
{
	Area Sector;
	int deltaX = region.x2 - region.x1;
	int deltaY = region.y2 - region.y1; 
	float sectorX = deltaX / numSectors; //partition the area into sector sized chunks
	float sectorY = deltaY / numSectors; //partition the area into sector sized chunks
	sectorX = sectorX / 2;
	sectorY = sectorY / 2;

	Sector.x1 = region.x1 + (deltaX / 2) - (sectorX*(sector + 1)); //center of search space + sector amnt
	Sector.x2 = region.x1 + (deltaX / 2) + (sectorX*(sector + 1));
	Sector.y1 = region.y1 + (deltaY / 2) - (sectorY*(sector + 1));
	Sector.y2 = region.y1 + (deltaY / 2) + (sectorY*(sector + 1));

	//boundary conditions
	if (Sector.x1 < region.x1)
		Sector.x1 = region.x1;

	if (Sector.x2 > region.x2)
		Sector.x2 = region.x2;

	if (Sector.y1 < region.y1)
		Sector.y1 = region.y1;

	if (Sector.y2 > region.y2)
		Sector.y2 = region.y2;

	return Sector;
}

//searches for an enemy using bloom sectoring
bool InterfaceStats::SearchEnemyBloom(unsigned int color, int x1, int y1, int x2, int y2)
{
	mouseDeviation = 30; //set ccause idea is monster or whatever is close
	Area region(x1, y1, x2, y2);
	Area Sector;
	for (int i = 0; i < numSectors; i++)//iterate through teh sectors looking for enemy
	{
		printf("Searching Sector %2i\n", i);
		Sector = GetBLOOMSectorCoords(region, i);
		if (FindEnemy(color, Sector))
		{
			mouseDeviation = 250; //reset
			return true;
		}
	}
	mouseDeviation = 250;//reset
	return false;
}

bool InterfaceStats::DetecMovement()
{
	return false;

}


//checks for health bar on a monster, given ap oint of a monster, checks surrounding area for red in health bar
bool InterfaceStats::CheckMonsterHealthBar(POINT monster)
{
	bool result = true;
	result &= pix.SearchPixelArea(0xff000000, monster.x - 45, monster.y - 90, monster.x +45, monster.y + 30);
	return result;
}

//returns true if also alternate color is found
bool InterfaceStats::CheckMonster(POINT monster, unsigned int altColor)
{
	bool result = true;
	result &= pix.SearchPixelArea(altColor, monster.x - 50, monster.y - 100, monster.x + 50, monster.y + 40);
	return result;
}

//Verifies that runescape is up
bool InterfaceStats::VerifyRunescapeWindow()
{
	bool Check = true;

	Check &= pix.VerifyPixelColor(pixChat);
	Check &= pix.VerifyPixelColor(pixStats);
	Check &= pix.VerifyPixelColor(pixInv);

	return Check;
}