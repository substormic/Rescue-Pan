#pragma once
#include "InterfaceInventory.h"
#include "InterfaceStats.h"

class BotMeleeCow
{
protected:
	InterfaceInventory inv;
	InterfaceStats stat;
	Mouse mouse;
	unsigned int cake = 0xa6999800;
	unsigned int goblin1 = 0x9c676100;
	unsigned int goblin2 = 0x9f701900;
	int currentStyle = 1; // 1 is attack, 2 is strength

public:

	//expects scimitars !!===================
	// if mode is 0, doesnt swith styles
	void run(int mode)
	{
		while (1)
		{
			inv.HandleHotkeys();

			if (stat.CheckLevelUp() && mode == 1)
			{
				//switch styles
				inv.VerifyActiveCombatStyle(); //open combat 
				Sleep(200);
				if (currentStyle == 1) //switch to strength
				{
					mouse.MouseMoveArea(1536+SCREEN,790,1595+SCREEN,825);
					mouse.LeftClick();
					currentStyle = 2;
					Sleep(2500);
				}
				else if (currentStyle == 2) //switch to attk
				{
					mouse.MouseMoveArea(1452 + SCREEN, 790, 1512 + SCREEN, 825);
					mouse.LeftClick();
					currentStyle = 1;
					Sleep(2500);
				}

			}

			if (!inv.AttemptToEatAtHp(cake, 75)) //no more food?
			{
				//out of food, exit
				Sleep(10000); // allows fight to finish
				inv.Logout();
				break;
			}
			if (!stat.Fight(goblin2, 200 + SCREEN, 100, 1300 + SCREEN, 900))
			{
				// Exit
			}
		}
	}
	};