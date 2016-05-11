/* ========================================================================= */
/*
BotFishingBarbPro.h (no matching .cpp file)
Authored by Connor Rainen and Brandon Wallace.

For fishing with OSBuddy Pro in barbarian outpost

SKILL: Fishing
HOURLY:		Money: 0		XP: + 14k/hr at lvl 20
Requirements:
- Min. fishing level 20
- Location:		Barbarian outpost stream
- Bank:			N/A
- Inventory:	1st spot should be a pole, 2nd should be feathers. Lots.
- Map:			Map normalized FULL ZOOM OUT, facing north, UP.
- Other:		N/A

Independence: Perfect Automation	(5/5)

note: Independence scale:
Active play			(1/5)
Intermittent play	(2/5)
Frequent monitor	(3/5)
Lazy monitor		(4/5)
Perfect Automation	(5/5)
*/
/* ========================================================================= */
#pragma once
#include "InterfaceInventory.h"
#include "InterfaceStats.h"
#include "InterfaceBank.h"

class BotWoodcuttingYewPro
{
private:
	InterfaceInventory inv;
	InterfaceStats stat;
	PixelHandler pix;
	InterfaceGeneral gen;
	InterfaceBank bank;
	Mouse mouse;

	unsigned int idleColor = 0xFF9C0000;

	unsigned int yewColor = 0xEC155900;
	unsigned int mapColor = 0xCB030300;

	int s = SCREEN - 1920;
	Area yewSearch = gen.areaBox(2740 + s, 519, 300);

	bool choppin() {
		gen.HandleAutoLogOut();
		return !pix.SearchPixelArea(idleColor, gen.areaBox(2022+s,70,2));
	}

	void dropFishies() {
		for (int i = 1; i < 28; i++) {
			inv.DropItem(i);
			gen.HandleHotkeys();
		}
	}

	bool clickaTree() {
		int tol = 5;
		POINT p = pix.SearchPixelAreaForPoint(yewColor, 2580 + s, 234, 2803 + s, 952, tol);

		int timeout = 0;
		while (timeout < 1000 && inv.CheckIfInGame()) {
			timeout++;
			if (p.x != -1) {
				printf("Found a tree\n");
				p.x -= 10;
				Area tree = gen.areaBox(p, 15);
				if (gen.DefiniteClick(yewColor, tol+1, tree, HOVER_ACTION, HOVER_ACTION, 0, 20)) {
					Sleep(6000);
					return true;
				}
				else {
					return false;
				}
				printf("Found a tree after %d tries\n", --timeout);
				return true;
			}
			p = pix.SearchPixelAreaForPoint(yewColor, 2680 + s, 234, 2803 + s, 952, tol);
			printf(".");
		}
		printf("yew search timeout\n");
		return false;
	}

	bool clickASpot() {
		POINT pixe = pix.SearchPixelAreaForPoint(yewColor, yewSearch);
		if (pixe.x == -1) {
			return false;
		}
		else {
			pixe.x -= 10;
			Area tree = gen.areaBox(pixe, 15);
			if (gen.DefiniteClick(yewColor, 5, tree, HOVER_ACTION, HOVER_ACTION, 0, 20)) {
				Sleep(8000);
				return true;
			}
			else {
				return false;
			}
		}
	}

	bool soYouThinkYouCanBank() {
		Area b = gen.areaBox(3072 - 1920, 239, 100);
		Area b2 = gen.areaBox(2950 - 1920, 160, 30);
		if (bank.OpenBank(b,false,130) || bank.OpenBank(b2,false,150)) {
			//we got the bank open
			inv.MoveToItem(1);
			mouse.RightClick();
			gen.ChooseMenuOption(5);
			mouse.LeftClick();
			bank.CloseBank();
			clickMiniMap();
			printf("BANKEDDDDDDDDD\n");
			return true;
		}		
		return false;

	}


	bool clickMiniMap() {
		POINT p = pix.SearchPixelAreaForPoint(mapColor, MiniMap);

		int timeout = 0;
		while (timeout < 1000 && inv.CheckIfInGame()) {
			timeout++;
			if (p.x != -1) {
				p.x += 2;
				p.y -= 5;
				mouse.MouseMove(p);
				mouse.LeftClick();
				Sleep(8000);
				printf("Found a tree after %d tries\n", --timeout);
				return true;
			}
			p = pix.SearchPixelAreaForPoint(mapColor, MiniMap);
		}
		printf("Minimapsearch timeout\n");
		return false;
	}

public:
	void run() {
		while (true) {
			gen.HandleAutoLogOut();
			gen.HandleHotkeys();

			/*if (!clickMiniMap()) {
				printf("Never found a tree! I should quit.\n");
				return;
			}

			if (!clickASpot()) {
				printf("Can't find a tree\n");
			}*/

			clickaTree();

			while (choppin()) {
				Sleep(100);
			}

			if (inv.VerifyInventoryFull() && !soYouThinkYouCanBank()){
				dropFishies();
			}

		}
	}
};