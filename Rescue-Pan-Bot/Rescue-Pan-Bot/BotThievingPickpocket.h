#pragma once
#include "InterfaceStats.h"
#include "InterfaceInventory.h"

//Run this with an empty inventory
//use run(color1, color2, foodColor)
//wield a bow with no arrows so you can't attack!


class BotThievingPickpocket
{
protected:
	InterfaceStats stat;
	InterfaceInventory inv;
	Mouse mouse;
	Area defaultRegion;

public:
	//ENEMY COLOR LIST
	unsigned int VarrockEastGuard1= 0xcc9c0c00; //skirt color
	unsigned int VarrockEastGuard2 = 0x6b510500;
	unsigned int AdroKnight1 = 0x5c0dc600;//purple chest
	unsigned int AdroKnight2 = 0x29055900;
	unsigned int Watchmen1 = 0x070b8100; // better blue?
	unsigned int Watchmen2 = 0x06086c00; // blue shoulder
	unsigned int Paladin1 = 0xdea33800; //yellow hair
	unsigned int Paladin2 = 0xb4abb400; //white shrt

	//FOOD LIST
	unsigned int monkfishMouth = 0xa6666300;
	unsigned int cake = 0xa6999800;
	unsigned int bread = 0x2e230200;
	unsigned int PineapplePizza = 0xa5a30900;

	//Other things. MAKE SURE TO INIALIZE COLORS
	unsigned int ThievingEXP = 0x00000100;
	unsigned int color, color2, food;

	int timeout = 350; //timeout fro getting exp
	int respawnTimeout = 45;
	int findTimeout = 35;
	 

	BotThievingPickpocket()
	{
		// Default search region and mouse speeds
		defaultRegion.x1 = 250 + SCREEN; // 100; //250
		defaultRegion.y1 = 150;//  60;  //250
		defaultRegion.x2 = 1250 + SCREEN;//  1500; //1300
		defaultRegion.y2 = 800;//  950; //800
		stat.SetMouseSpeed(0.4);
		mouse.ChangeSpeed(0.4);
		inv.SetMouseSpeed(0.4);


		color = Watchmen1;  //=========================================  SET ME =============================================
		color2 = Watchmen2; //=========================================  SET ME =============================================
		food = bread; //=========================================  SET ME =============================================
	}

	//Runs HARDCODED params
	void run()
	{
		while (1)
		{
			//GENERAL INITIALIZERS
			stat.HandleAutoLogOut();
			stat.HandleHotkeys();
			if (!stat.VerifyRunescapeWindow())
			{
				printf("No rs\n");
				continue;
			}

			//ERROR BOT-END HANDLING/EATING
			HandleTimeoutEnds(food);

			//Actually do the thieving
			Thieve(color, color2, defaultRegion.x1, defaultRegion.y1, defaultRegion.x2, defaultRegion.y2);

			//End of loop timeout coutners
			timeout--;
			respawnTimeout--;
		}
	}

	//Runs given an enemy and food
	void run(unsigned int enemy1, unsigned int enemy2, unsigned int food)
	{
		while (1)
		{
			//GENERAL INITIALIZERS
			stat.HandleAutoLogOut();
			stat.HandleHotkeys();
			if (!stat.VerifyRunescapeWindow())
				continue;

			//ERROR BOT-END HANDLING/EATING
			if (!HandleTimeoutEnds(food))
				break;

			//Actually do the thieving
			Thieve(enemy1, enemy2, defaultRegion.x1, defaultRegion.y1, defaultRegion.x2, defaultRegion.y2);

			//End of loop timeout coutners
			timeout--;
			respawnTimeout--;
		}
	}

	bool Thieve(unsigned int enemy1, unsigned int enemy2, int x1, int y1, int x2, int y2)
	{

		printf("thiving tiem, General Timeout: %2i, Find-Timeout: %2i\n", timeout, findTimeout);
		if (!stat.SearchEnemyBloom(enemy1, x1, y1, x2, y2))
		{
			printf("Attempting second color bloom\n");
			if (!stat.SearchEnemyBloom(enemy2, x1, y1, x2, y2)) // if nothign found at all!
			{
				printf("Nothing found on screen\n");
				findTimeout--;
				return false; //nothing found, return
			}
			else
			{
				printf("Replenishing find timeout - second bloom\n");
				findTimeout = 35; //something found
			}
		}
		else
		{
			printf("Replenishing find timeout - FIRST bloom\n");
			findTimeout = 35; //something found
		}

		//If enemy found and right clicked
		Sleep(100);
		if (stat.ChooseMenuOptionColorCheck(1, HOVER_NPC))
		{
			mouse.LeftClick();
			//Sleep(200);
		}

		return true;
	}

	//Handles all timeout related stuff - and also eating
	bool HandleTimeoutEnds(unsigned int Eatme)
	{

		if (!inv.AttemptToEatAtHp(Eatme, 75))
		{
			printf("============================= NO FOOOD FOUND!! =====================\n");
			inv.Logout();
			return false;
		}

		// TIME-OUT HANDLERS for when things go wrong
		if (stat.CheckExperienceCircle(ThievingEXP, 1)) // <------------- replenish all timeouts on successful action
		{
			printf("Replenishing ALL timeouts\n");
			findTimeout = 35;
			timeout = 350;
		}
		//When not finding enemies for a while, move around a little to see if theyre offscreen
		if (findTimeout < 20) // <---------------------- Enemy Finding Timeout Danger-Mode
		{
			mouse.MouseMoveArea(1540 + SCREEN, 100, 1580 + SCREEN, 150);
			mouse.LeftClick();
			Sleep(500);
			mouse.MouseMoveArea(1540 + SCREEN, 100, 1580 + SCREEN, 150);
			mouse.LeftClick();
			Sleep(500);
			mouse.MouseMoveArea(1540 + SCREEN, 100, 1580 + SCREEN, 150);
			mouse.LeftClick();
			Sleep(1000);
			timeout = 350; //replenish general timeout, itll still fail if we find nothing 20 times of moving
		}

		if (respawnTimeout <= 0)//move to reset respawn of trapped guy
		{
			printf("time to twist up the spawn~!\n");
			//Sleep(5000);
			Sleep(1000);
			mouse.MouseMoveArea(1550 + SCREEN, 110, 1570 + SCREEN, 125);
			mouse.LeftClick();
			Sleep(500);
			respawnTimeout = 75;
		}

		if (findTimeout <= 0)
		{
			printf("=========================== Could not find any more thieving targets ====================");
			Sleep(10000); //if recent combat, lets the logout.
			inv.Logout();
			return false; //somethings gone wrong
		}


		if (timeout <= 0)
		{
			printf("=========================== General Timeout Reached ====================");
			Sleep(10000); //if recent combat, lets the logout.
			inv.Logout();
			return false; //somethings gone wrong
		}

		return true; //base case, nothings wrong
	}


};

/*
//runs completely default
void run()
{
stat.SetMouseSpeed(0.4);
mouse.ChangeSpeed(0.4);
inv.SetMouseSpeed(0.4);
while (1)
{
stat.HandleAutoLogOut();

if (!stat.VerifyRunescapeWindow())
continue;

stat.HandleHotkeys();

//ALWAYS BEGIN WITH THIS in loop

/* //unneeded if you bring a bow with no arrows

if (stat.VerifyCombat() == 1)
{
stat.SearchEnemyBloom(color, defaultRegion.x1, defaultRegion.y1, defaultRegion.x2, defaultRegion.y2);
if (stat.ChooseMenuOptionColorCheck(0, HOVER_NPC)) //attack!
{
mouse.LeftClick();
Sleep(3000);
}
continue;
}
// =======end combat 

if (findTimeout < 20) // not finding anything, lets move around
{
	mouse.MouseMoveArea(1540, 100, 1580, 150);
	mouse.LeftClick();
	Sleep(500);

	mouse.MouseMoveArea(1540, 100, 1580, 150);
	mouse.LeftClick();
	Sleep(500);

	mouse.MouseMoveArea(1540, 100, 1580, 150);
	mouse.LeftClick();
	Sleep(1000);

	timeout = 350; //replenish general timeout, itll still fail if we find nothing 20 times of moving
}

if (stat.CheckExperienceCircle(ThievingEXP, 1)) //replenish all timeouts
{
	printf("Replenishing ALL timeouts\n");
	findTimeout = 35;
	timeout = 350;
}

if (respawnTimeout <= 0)//move to reset respawn of trapped guy
{
	printf("time to twist up the spawn~!\n");
	//Sleep(5000);
	Sleep(1000);
	mouse.MouseMoveArea(1550, 110, 1570, 125);
	mouse.LeftClick();
	Sleep(500);
	respawnTimeout = 75;
}

if (findTimeout <= 0)
{
	printf("=========================== Could not find any more thieving targets ====================");
	inv.Logout();
	break;
}


if (timeout <= 0)
{
	printf("=========================== General Timeout Reached ====================");
	inv.Logout();
	break;
}

if (!inv.AttemptToEatAtHp(food, 75))
{
	printf("============================= NO FOOOD FOUND!! =====================\n");
	inv.Logout();
	break;
}

//stat.FindEnemy(color, 250, 250, 1300, 800);
if (!stat.SearchEnemyBloom(color, defaultRegion.x1, defaultRegion.y1, defaultRegion.x2, defaultRegion.y2))
{
	printf("Attempting second color bloom\n");
	if (!stat.SearchEnemyBloom(color2, defaultRegion.x1, defaultRegion.y1, defaultRegion.x2, defaultRegion.y2)) // if nothign found at all!
	{
		printf("Nothing found on screen\n");
		findTimeout--;
	}
	else
	{
		printf("Replenishing find timeout - second bloom\n");
		findTimeout = 35; //something found
	}
}
else
{
	printf("Replenishing find timeout - FIRST bloom\n");
	findTimeout = 35; //something found
}

printf("thiving tiem, General Timeout: %2i, Find-Timeout: %2i\n", timeout, findTimeout);
Sleep(100);
if (stat.ChooseMenuOptionColorCheck(1, HOVER_NPC))
{
	mouse.LeftClick();
	//Sleep(200);
}
timeout--;
respawnTimeout--;
//std::cin.ignore();
		}
	}


*/