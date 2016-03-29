#pragma once
#include "InterfaceInventory.h"

//starts in the tri iron mine next to adro, with zoom all the way in
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
	POINT R1, R2, R3;

	Area Rock1;
	Area Rock2;
	Area Rock3;

public:
	BotMiningTriAd()
	{
		R1.x = 700 + SCREEN;
		R1.y = 520;

		R2.x = 795 + SCREEN;
		R2.y = 610;

		R3.x = 955 + SCREEN;
		R3.y = 520;

		Rock1 = inv.areaBox(R1,40);
		Rock2 = inv.areaBox(R2, 40);
		Rock3 = inv.areaBox(R3, 40);
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
		}
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
		while (!pix.SearchPixelArea(Oreless, rock.x1, rock.y1, rock.x2, rock.y2, 8))
		{
			;
		}
		return true;
	}

	void run()
	{
		initialize();
		while (1)
		{
			inv.HandleHotkeys();
			if (inv.CheckLastItem(ironOre) || inv.CheckLastItem(ironOre2) || inv.CheckLastItem(Emerald)) //if inventory full
			{
				printf("Iron found in last spot\n");
				//inv.DropAllItems();
				inv.DropItemsColored(ironOre);
				inv.DropItemsColored(Emerald);
			}

			MineTime();
		}
	}

};