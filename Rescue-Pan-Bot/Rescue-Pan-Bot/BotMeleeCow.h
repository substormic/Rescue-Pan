#pragma once
#include "InterfaceInventory.h"
#include "InterfaceStats.h"
//despite the name, this trains melee with goblins
//run in mode 0 to not switch combat styles

//run in mode 1 to switch between att & str if training with scim

//start this near goblins, ideally in fallador area


class BotMeleeCow
{
protected:
	InterfaceInventory inv;
	InterfaceStats stat;
	InterfaceGeneral gen;
	Mouse mouse;
	unsigned int cake = 0xa6999800;
	unsigned int goblin1 = 0x9c676100;
	unsigned int goblin2 = 0x9f701900;
	unsigned int cocatrice = 0x55541500;
	unsigned int yak = 0x6F6A6400;
	unsigned int yak2 = 0x140C0900;
	unsigned int lobby = 0xAA480A00;
	int currentStyle = 1; // 1 is attack, 2 is strength

public:

	//expects scimitars !!===================
	// if mode is 0, doesnt swith styles
	void run(int mode)
	{
		stat.fightme = 1;
		stat.alternateColor = yak2;
		while (1)
		{
			inv.HandleHotkeys();
			gen.HandleAutoLogOut();

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

			if (!inv.AttemptToEatAtHp(lobby, 50)) //no more food?
			{
				//out of food, exit
				Sleep(10000); // allows fight to finish
				inv.Logout();
				break;
			}
			if (!stat.Fight(yak, 200 + SCREEN, 100, 1300 + SCREEN, 900,0x00FF0000))
			{
				// Exit
			}
		}
	}
	};