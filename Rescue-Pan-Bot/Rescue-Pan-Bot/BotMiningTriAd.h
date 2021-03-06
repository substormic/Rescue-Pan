#pragma once
#include "InterfaceInventory.h"

/* ========================================================================= */
/*
BotConstructionCrudeChairs.h (no matching .cpp file)
Authored by Connor Rainen.

For creating crude wooden chairs at a POH

SKILL: Mining
HOURLY:		Money: - 0		XP: + 25k
Requirements:
- No minimum skill required
- Location:		Begin In the Mine next to Adrogune, in the 3 iron spot
- Bank:			N/a
- Inventory:	Empty
- Map:			OSBuddy ZOOM all the way in

Independence: Frequent monitor (4/5)

note: Does not autoLogout handle yet

note: Independence scale:
Active play			(1/5)
Intermittent play	(2/5)
Frequent monitor	(3/5)
Lazy monitor		(4/5)
Perfect Automation	(5/5)
*/
/* ========================================================================= */
class BotMiningTriAd
{
private:
	InterfaceInventory inv;
	Mouse mouse;
	PixelHandler pix;
	int Rock = 1;
	unsigned int ironOre = 0x31191000;
	unsigned int ironOre2 = 0x60482d00;
	unsigned int IronOreAvail = 0x47241800;
	unsigned int Oreless = 0x3d373700;
	unsigned int Emerald = 0x067b0900;
	unsigned int Saphire = 0x080b8500;
	unsigned int Ruby = 0x690d0500;
	int fail = 20;

	POINT R1, R2, R3;

	Area Rock1;
	Area Rock2;
	Area Rock3;

public:
	BotMiningTriAd()
	{
		R1.x = 680 + SCREEN;
		R1.y = 560;

		R2.x = 795 + SCREEN;
		R2.y = 650;

		R3.x = 950 + SCREEN;
		R3.y = 560;

		Rock1 = inv.areaBox(R1,40,65);
		Rock2 = inv.areaBox(R2, 25,65);
		Rock3 = inv.areaBox(R3, 40,65);
	}

	void initialize()
	{
		inv.NormalizeCompass(UP);
		inv.SetMouseDeviation(40);
		inv.SetMouseSpeed(0.3);
	}

	void MineTime()
	{
		switch (Rock)
		{
		case 1:	MineRock(Rock1); Rock = 2; printf("Changing to rock 2\n");  break;
		case 2:	MineRock(Rock2); Rock = 3; printf("Changing to rock 3\n"); break;
		case 3:	MineRock(Rock3); Rock = 1; printf("Changing to rock 1\n"); break;
		default: Rock = 1;
		}
	}

	void WaitOnOre(Area rock)
	{
		if (pix.SearchPixelArea(Oreless, rock.x1, rock.y1, rock.x2, rock.y2, 8))
			printf("Trying to Mine, waiting on ore\n"); // wait till it has ore

		while (pix.SearchPixelArea(Oreless, rock.x1, rock.y1, rock.x2, rock.y2, 8))
		{
			; //wait until it is not oreless
		}
		return;
	}

	bool MineRock(Area rock)
	{

		WaitOnOre(rock);

		int MineTimeout = 7;
		POINT Iron = pix.SearchPixelAreaForPoint(IronOreAvail, rock.x1, rock.y1, rock.x2, rock.y2,15);
		if (Iron.x != -1)
		{
			Area Ore = inv.areaBox(Iron, 8);
			mouse.MouseMoveArea(Ore);
			Sleep(250);

			while (!inv.VerifyTopLeftText(HOVER_ACTION)) //while not on a rock
			{
				if (MineTimeout <= 0)
				{
					printf("Rock could not be found to mine\n");
					return false;
				}
				POINT Iron = pix.SearchPixelAreaForPoint(IronOreAvail, rock.x1, rock.y1, rock.x2, rock.y2, 15);
				if (Iron.x != -1)
				{
					Area Ore = inv.areaBox(Iron, 8);
					mouse.MouseMoveArea(Ore);
					Sleep(250);
				}
				MineTimeout--;
			}
			Sleep(250);
			mouse.LeftClick();
			Sleep(1500);
			printf("Waiting for Rock to finish being mined before moving to next\n"); // wait till it has ore
			MineTimeout = 7;
			while (!pix.SearchPixelArea(Oreless, rock.x1, rock.y1, rock.x2, rock.y2, 8))
			{
				if (MineTimeout <= 0)
				{
					printf("Rock was never really waiting i guess\n");
					fail--;
					return false;
				}
				MineTimeout--;
			}
			fail = 15;
			return true;
		}
		fail--;
		return false;
	}

	void run()
	{
		initialize();
		while (1)
		{
			if (fail <= 0)
			{	
				inv.Logout();
				return;
			}
			inv.HandleHotkeys();
			if (inv.CheckLastItem(ironOre) || inv.CheckLastItem(ironOre2) || inv.CheckLastItem(Emerald) || inv.CheckLastItem(Saphire) || inv.CheckLastItem(Ruby)) //if inventory full
			{
				printf("Iron found in last spot\n");
				//inv.DropAllItems();
				inv.DropItemsColored(ironOre);
				inv.DropItemsColored(Emerald);
				inv.DropItemsColored(Saphire);
				inv.DropItemsColored(Ruby);
			}

			MineTime();
		}
	}

};