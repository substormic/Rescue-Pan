#pragma once
#include "InterfaceGeneral.h"
#include "InterfaceInventory.h"

class BotMiningDropAdro {
	unsigned int oreColor = 0x321A1100;
	unsigned int inventoryColor = 0x61492D00;
	int tol = 5;
	int totalMined = 0;
	Area RockRegion;
	InterfaceInventory inv;
	InterfaceGeneral gen;
	PixelHandler pix;
	Mouse mouse;
	
	bool clickRock(Area rockRegion) {
		int timeout = 0;
		POINT pt;
		while (timeout < 2000) {
			if (timeout == 1000) {
				printf("Timed out finding ore by color, tyring random region\n");
			}
			if (timeout < 1000) {
				if (pix.SearchPixelArea(oreColor, rockRegion.x1 + SCREEN, rockRegion.y1, rockRegion.x2 + SCREEN, rockRegion.y2, tol)) {
					pt = pix.SearchPixelAreaForPoint(oreColor, rockRegion.x1 + SCREEN, rockRegion.y1, rockRegion.x2 + SCREEN, rockRegion.y2, tol);
					mouse.MouseMove(pt);
				}
				else {
					mouse.MouseMoveArea(rockRegion.x1 + SCREEN, rockRegion.y1, rockRegion.x2 + SCREEN, rockRegion.y2);
				}
			}
			else {
				mouse.MouseMoveArea(rockRegion.x1 + SCREEN, rockRegion.y1, rockRegion.x2 + SCREEN, rockRegion.y2);
			}			
			mouse.RightClick();
			Sleep((rand() % 30) + 70);
			if (gen.ChooseMenuOptionColorCheck(0, 0x00FFFF00)) {
				mouse.LeftClick();
				return true;
			}
			timeout++;
		}
		return false;
	}

	bool didGetOre() {
		int startingOre = countInventory(inventoryColor);
		int MiningTimeout = 0;
		while (countInventory(inventoryColor) == startingOre) {
			Sleep(10);
			gen.HandleAutoLogOut();
			MiningTimeout++;
			if (MiningTimeout == 20) {
				printf("You still haven't found ore, let's try clicking one more time\n");
				clickRock(RockRegion);
			}
			if (MiningTimeout > 100) {
				return false;
			}
		}
		return true;
	}
	int countInventory(unsigned int itemColor) {
		for (int i = 0; i < 28; i++) {
			if (inv.SearchIndexForColor(i, itemColor)) {
				continue;
			}
			else {
				return i;
			}
		}
		return 27;
	}

public:
	void run() {
		RockRegion.x1 = 2630 - 1920;
		RockRegion.y1 = 422;
		RockRegion.x2 = 2890 - 1920;
		RockRegion.y2 = 653;

		while (inv.VerifyInventoryOpen()) {
			if (!clickRock(RockRegion)) {
				printf("Couldn't find ore \n");
				return;
			}
			if (!didGetOre()) {
				printf("You never got another ore, something went wrong :/\n");
				return;
			}
			totalMined++;
			if (inv.SearchIndexForColor(27, inventoryColor)) {
				printf("Inventory full. Dropping.");
				inv.DropAllItems();
				printf(" %d mined for %d XP.\n", totalMined, totalMined * 35);
				
			}
		}
	}
	




};