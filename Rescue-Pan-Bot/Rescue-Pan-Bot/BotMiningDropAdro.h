#pragma once
#include "InterfaceGeneral.h"
#include "InterfaceInventory.h"
//meant to be run in adrogene on the left set of 3 iron ores with map facing north and default zoom
//use MiningTriAd, it's better.
class BotMiningDropAdro {
	unsigned int oreColor = 0x3B1E1500;
	unsigned int inventoryColor = 0x61492D00;
	int tol = 2;
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
		while (!inv.SearchIndexForColor(startingOre, inventoryColor))
		{
			MiningTimeout++;
			Sleep(10);
			if (MiningTimeout == 75) {
				printf("Re-clicking.\n");
				clickRock(RockRegion);
			}
			if (MiningTimeout > 320) {
				if (inv.VerifyInventoryOpen()) {
					printf("Dropping everything, you might've gotten a non-ore\n");
					inv.DropAllItems();
					return true;
				}
				else {
					printf("Inventory isn't open, gotta leave :/\n");
					return false;
				}
			}
		}
		return true;

		/*while (countInventory(inventoryColor) == startingOre) {
			Sleep(100);
			gen.HandleAutoLogOut();
			MiningTimeout++;
			if (MiningTimeout == 9) {
				printf("You still haven't found ore, let's try clicking one more time\n");
				clickRock(RockRegion);
			}
			if (MiningTimeout > 50) {
				if (!hardCheckItem(startingOre)) {
					printf("Mine timed out.\n");
					return true; //todo: fix this; it should be "false," but I want bot to run forever. Risky.
				}
				else {
					printf("you got something, not an ore!\n");
					inv.DropAllItems();
					return true;
				}
			}
		}
		return true; */
	}
	bool hardCheckItem(int loc) {
		inv.MoveToItem(loc);
		bool Item = inv.VerifyTopLeftText(HOVER_ITEM); //checks both hover and top left to see if item exists 
		Item |= inv.VerifyHoverText(HOVER_ITEM);
		return Item;

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
		return 28;
	}

public:
	void run() {
		RockRegion.x1 = 2646 - 1920;
		RockRegion.y1 = 529;
		RockRegion.x2 = 2798 - 1920;
		RockRegion.y2 = 619;

		while (inv.VerifyInventoryOpen()) {
			if (inv.SearchIndexForColor(27, inventoryColor)) {
				printf("Inventory full. Dropping.");
				inv.DropAllItemsFast();
				printf(" %d mined for %d XP.\n", totalMined, totalMined * 35);

			}
			if (!clickRock(RockRegion)) {
				printf("Couldn't find ore \n");
				return;
			}
			if (!didGetOre()) {
				printf("You never got another ore, something went wrong :/\n");
				return;
			}
			totalMined++;
			
		}
	}
	




};