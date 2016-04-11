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

class BotFishingBarbPro
{
private:
	InterfaceInventory inv;
	InterfaceStats stat;
	PixelHandler pix;
	InterfaceGeneral gen;
	Mouse mouse;

	unsigned int fishColor = 0x71637F00;
	unsigned int redColor = 0xFF000000;
	
	bool fishing() {
		gen.HandleAutoLogOut();
		return !pix.SearchPixelArea(redColor, gen.areaBox(3293 - 1920 + SCREEN, 64, 2));
	}

	void dropFishies() {
		for (int i = 2; i < 28; i++) {
			inv.DropItem(i);
		}
	}

	bool clickASpot() {
		Area fishArea;
		fishArea.x1 = 2487;
		fishArea.x2 = 2947 - 1920 + SCREEN;
		fishArea.y1 = 282;
		fishArea.y2 = 986;
		POINT pixe = pix.SearchPixelAreaForPoint(fishColor, fishArea,4);
		if (pixe.x == -1) {
			printf("working without finding the pixel\n");//
		}
		else {
			pixe.y -= 15;
			fishArea = gen.areaBox(pixe, 10);
		}
		bool success = gen.DefiniteClick(fishColor, 10, fishArea, HOVER_NPC, HOVER_NPC, 0, 10);
		if (success) Sleep(8000);//sleep long enough for OSBuddy to know we're fishing!
		return success;
	}

public:
	void run() {
		while (true) {
			gen.HandleAutoLogOut();
			gen.HandleHotkeys();

			clickASpot();

			while (fishing()) {
				Sleep(100);
			}
			
			if (inv.SearchIndexForColor(27, 0x998A8A00) ||
				inv.SearchIndexForColor(27, 0xB4625400)) {
				dropFishies();
			}

		}
	}
};