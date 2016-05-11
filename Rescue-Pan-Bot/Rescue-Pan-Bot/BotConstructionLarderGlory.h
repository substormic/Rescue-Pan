/* ========================================================================= */
/*
BotConstructionCrudeChairs.h (no matching .cpp file)
Authored by Brandon Wallace and Connor Rainen.

For creating oak larders at POH

SKILL: Construction
HOURLY:		Money: - 1.19M/hr		XP: + 115k/hr
Requirements:
- No minimum skill required
- Location:		Begin anywhere.
- Bank:			Have "Tab 7" contain a stack of oak planks (2,1) (2nd row, 3rd col)
- Inventory:	Teletabs(0) Hammer (1) Saw (2) Nails (3) Planks (rest)
- Map:			Map normalized to OSBuddy def. zoom, facing north, UP.

Independence: Lazy monitor (4/5)

note: Expects kitchen north of portal.
note: Remove "right click high scores" from OSBuddy settings

note: Independence scale:
Active play			(1/5)
Intermittent play	(2/5)
Frequent monitor	(3/5)
Lazy monitor		(4/5)
Perfect Automation	(5/5)
*/
/* ========================================================================= */
#pragma once
#include "InterfaceGeneral.h"
#include "InterfaceBank.h"
#include "InterfaceInventory.h"
#include "Pixel.h"
#include "Mouse.h"

class BotConstructionLarderGlory {
private:
	InterfaceBank bank;
	InterfaceGeneral gen;
	InterfaceInventory inv;
	InterfaceStats stats;
	PixelHandler pix;
	Mouse mouse;

	bool firstRetry = false;

	Pixel House;
	Pixel Camelot;
	unsigned int GhostChair2 = 0xcbc6c200; //connor's ultra white.
	unsigned int ghostLarder = 0xD9D5D400;
	unsigned int ghostLarder2 = 0xD4D0CF00; //from an angle? this isn't used yet.
	int chairs = 0;
	int lardsSinceLog = 0;

	bool get2bank() {

		bool r = bank.OpenBank(gen.areaBox(3285-1920, 867, 60, 20),80);
		if (!r) {
			printf("trying to open bank via compass rotation\n");
			stats.ChangeCompassDegrees(180);
			r |= bank.OpenBank(gen.areaBox(3049 - 1920, 142, 20, 40));
			if (!r) {
				r |= bank.OpenBank(gen.areaBox(2976 - 1920, 135, 20));
			}
			gen.NormalizeCompass(UP);
			if (!r) {
				printf("That didn't work, let's check for 3 other problems\n");
				//attempt to handle the poll booth having been opened
				if (pix.VerifyPixelColor(0x7C1D1B00, 2799 - 1920 + SCREEN, 345, 4)) {
					printf("Poll was open\n");

					mouse.MouseMoveArea(gen.areaBox(2850 - 1920 + SCREEN, 307, 1));
					mouse.LeftClick();
					Sleep(100);
					r |= bank.OpenBank(gen.areaBox(2528 - 1920, 401, 50));
				}
				else if (pix.VerifyPixelColor(0xFF981F00, 2641-1920+SCREEN, 345,5)) {
					printf("Collect was open\n");
					mouse.MouseMove(2829 - 1920 + SCREEN, 348);
					mouse.LeftClick();
					Sleep(400);
					r |= bank.OpenBank(gen.areaBox(2800-1920, 530,40));
				}
				else if (bank.OpenBank(gen.areaBox(2778 - 1920, 569, 20))) {
					printf("You got to that 'i cant reach that' place\n");
					r = true;
				}

			}
		}
		
		return r;
	}

	bool get2Edgeville() {
		unsigned int gloryMinimapColor = 0xA00D9D00;
		unsigned int gloryWorldColor = 0xEFB70E00;

		POINT p = pix.SearchPixelAreaForPoint(gloryMinimapColor, MiniMap, 1);
		if (p.x == -1) 
			return false;
		mouse.MouseMove(p);
		mouse.LeftClick();
		Sleep(5000);
		
		Area gloryWorld = gen.areaBox(2732, 481, 200);

		POINT p2 = pix.SearchPixelAreaForPoint(gloryWorldColor, gloryWorld,4);
		if (p2.x == -1) {
			Sleep(1000);
			p2 = pix.SearchPixelAreaForPoint(gloryWorldColor, gloryWorld, 10);
			if (p2.x == 1) {
				printf("we can't find the glory\n");
				return false;
			}
		}
		Area smallArea = gen.areaBox(p2, 10);

		bool s = gen.DefiniteClick(gloryWorldColor, 4, smallArea, HOVER_ACTION, HOVER_ACTION, 0, 15);
		if (s) {
			Sleep(3000);
			return true;
		}
		else {
			printf("Trying again with glory finding\n");
			p2 = pix.SearchPixelAreaForPoint(gloryWorldColor, gloryWorld, 4);
			if (p2.x == -1) {
				printf("we can't find the glory\n");
				return false;
			}
			smallArea = gen.areaBox(p2, 10);
			if (!gen.DefiniteClick(gloryWorldColor, 4, smallArea, HOVER_ACTION, HOVER_ACTION, 0, 70)) {
				printf("couldn't click the glory even though we found one\n");
				return false;
			}
			else {
				printf("Got the glory on 2nd try\n");
				return true;
			}
		}

	}

	bool verifyInventorySetup() {
		inv.VerifyActiveInventory();
		if (!inv.SearchIndexForColor(0, 0x0C11D300)) {
			printf("Must have teletabs in index 0 (top left)");
			return false;
		}

		if (!inv.SearchIndexForColor(1, 0x3C240300)) {
			printf("Must have hammer in index 1");
			return false;
		}
		if (!inv.SearchIndexForColor(2, 0x38210300)) {
			printf("Must have saw in index 2");
			return false;
		}
		return true;
	}

	

	bool withdrawHouseSupplies() {
		if (!bank.VerifyBankOpen() && gen.VerifyOSbuddy()) {
			printf("Bank should've been open\n");
			return false;
		}
		firstRetry = false;
		bank.OpenTab(7);
		Sleep(100 + (rand() % 40));
		if (!bank.VerifyItem(0x6A503100, 2, 1)) {
			printf("No oak planks are at index 1,1 in tab 7\n");
			return false;
		}
		bank.Withdraw(2, 1, 5); //withdraw all, so we can run out
		Sleep(50);
		bank.CloseBank();
		Sleep(100);
		return true;
	}

	bool houseLoaded() {
		POINT p1;
		POINT p2;
		POINT p3;
		unsigned int color = 0x00000000;
		p1.x = 2196 - 1920 + SCREEN;
		p1.y = 212;

		p2.x = 2420 - 1920 + SCREEN;
		p2.y = 850;

		p3.x = 2768 - 1920 + SCREEN;
		p3.y = 350;
		int timeout = 0;
		while (!pix.VerifyPixelColor(color, p1.x, p1.y) && !pix.VerifyPixelColor(color, p2.x, p2.y) && !pix.VerifyPixelColor(color, p3.x, p3.y)) {
			timeout++;
			Sleep(5);
			if (timeout > 1000) {
				printf("House loading menu never arrived\n");
				return false;
			}
		}
		timeout = 0;
		while (pix.VerifyPixelColor(color, p1.x, p1.y) && pix.VerifyPixelColor(color, p2.x, p2.y) && pix.VerifyPixelColor(color, p3.x, p3.y)) {
			timeout++;
			Sleep(5);
			if (timeout > 1000) {
				printf("House didn't load\n");
				return false;
			}
		}
		return true;
	}

	//Teleports to house; enables build mode; clicks "build" on the left chair spot.
	bool teleportToHouse() {
		
		inv.VerifyActiveInventory();
		inv.MoveToItem(0);
		mouse.LeftClick();
		Sleep(10);
		if (!houseLoaded()) {
			printf("House load error\n");
			return false;
		}
		inv.ActivateBuildingMode();
		if (!houseLoaded()) {
			printf("Building mode error\n");
			return false;
		}
		Sleep(800);

		unsigned int ghostLarder3 = 0xD2CDCC00;
		unsigned int ghostLarder4 = 0xDAD6D500;
		unsigned int ghostLarder5 = 0xD5D0CF00;
		unsigned int ghostLarder6 = 0xB1ABA900;
		//unsigned int ghostLarder7 = 0xB7B1B0

		Area lardClick = inv.areaBox(2871 - 1920 + SCREEN, 123, 50,20);
		Area bLard = inv.areaBox(2898 - 1920 + SCREEN, 122, 50, 20);
		Area bLard2 = inv.areaBox(2916 - 1920 + SCREEN, 151, 40);
		POINT moveLard = pix.SearchPixelAreaForPoint(ghostLarder, lardClick.x1, lardClick.y1, lardClick.x2, lardClick.y2, 6);
		if (moveLard.x == -1) {
			printf("Using backup coords\n");
			Sleep(100);
			moveLard = pix.SearchPixelAreaForPoint(ghostLarder, bLard, 8);
		}
		if (moveLard.x == -1) {
			printf("Using backup coords 2\n");
			Sleep(100);
			moveLard = pix.SearchPixelAreaForPoint(ghostLarder2, bLard, 10);
		}
		if (moveLard.x == -1) {
			printf("Using backup coords 3\n");
			Sleep(100);
			moveLard = pix.SearchPixelAreaForPoint(ghostLarder3, bLard, 15);
		}
		if (moveLard.x == -1) {
			printf("Using backup coords 4\n");
			Sleep(100);
			moveLard = pix.SearchPixelAreaForPoint(ghostLarder, bLard2, 15);
		}
		if (moveLard.x == -1) {
			printf("Using backup coords 5\n");
			Sleep(100);
			moveLard = pix.SearchPixelAreaForPoint(ghostLarder4, bLard, 15);
		}
		if (moveLard.x == -1) {
			printf("Using backup coords 6\n");
			Sleep(100);
			moveLard = pix.SearchPixelAreaForPoint(ghostLarder4, bLard2, 15);
		}
		if (moveLard.x == -1) {
			printf("Using backup coords 7\n");
			Sleep(500);
			moveLard = pix.SearchPixelAreaForPoint(ghostLarder6, bLard2, 20);
		}
		if (moveLard.x == -1) {
			printf("Using backup coords 8\n");
			Sleep(1000);
			moveLard = pix.SearchPixelAreaForPoint(ghostLarder4, bLard2, 25);
		}
		if (moveLard.x != -1)
		{
			mouse.MouseMove(moveLard);
			Sleep(150);
			mouse.RightClick();
			Sleep(60);
			if (!inv.ChooseMenuOptionColorCheck(2, HOVER_ACTION)) {
				printf("We clicked wrong\n");
				return false;
			}
			Sleep(50);
			mouse.LeftClick();
		}
		else
		{
			printf("Couldnt find larder\n");
			return false;
		}
		return true;

	}

	bool waitForChairBuild(Area cb, unsigned int color) {
		int timeout = 0;
		while (!pix.SearchPixelArea(color, cb.x1, cb.y1, cb.x2, cb.y2, 5)) {
			timeout++;
			Sleep(5);
			if (timeout > 500) {
				printf("Chair didn't ever build :(\n");
				return false;
			}
		}
		return true;
	}

	void handleRunEnergy() {
		Pixel pixeltowatch;
		pixeltowatch.Set(0x13131300, 3408 - 1920 + SCREEN, 176);
		Pixel b = pixeltowatch;
		b._color = 0xA2A39500;

		Pixel runOff;
		runOff.Set(0x9C6F4700, 3417 - 1920 + SCREEN, 178);

		if (pix.VerifyPixelColor(b)) {
			printf("Activating run");
			mouse.MouseMove(pixeltowatch._x, pixeltowatch._y);
			mouse.LeftClick();
			Sleep(100);
		}
		//if run is off and energy < 50: //deactivated temporarily.
		if (pix.VerifyPixelColor(runOff) && pix.VerifyPixelColor(pixeltowatch)) {
			printf("run depleted, waiting\n");
			Sleep(90 * 1000);
			//mouse.MouseMove(pixeltowatch._x, pixeltowatch._y);
			//mouse.LeftClick();
			handleRunEnergy();
			Sleep(100);
		}

	}


	bool confirmChairAvail() {
		unsigned int red = 0xFF000000;
		return (!pix.VerifyPixelColor(red, 2400 - 1920 + SCREEN, 465));
	}

	bool waitForMenu() {
		unsigned int col = 0xFF981F00;
		int timeout = 0;
		while (!pix.VerifyPixelColor(col, 2538 - 1920 + SCREEN, 319)) {
			Sleep(5);
			timeout++;
			if (timeout > 1000) {
				return false;
			}
		}
		return true;
	}

	void handleDialogBox(int optionNum) {
		int timeout = 0;
		while (!pix.VerifyPixelColor(0x1C928E00, 2338 - 1920 + SCREEN, 903, 1)) {
			Sleep(5);
			timeout++;
			if (timeout > 1000) {
				return;
			}
		}
		Keyboard().TypeNum(optionNum + 1, 1);
	}

	//note: assumes chair menu is started open
	bool buildChairLoop() {
		inv.VerifyActiveInventory();
		Area larderOnMenu = gen.areaBox(2404 - 1920 + SCREEN, 474, 2);
		Area larderOnScreen = gen.areaBox(2783 - 1920 + SCREEN, 455, 40);
		Area emptyLarderOS = gen.areaBox(2739 - 1920 + SCREEN, 446, 40);
		unsigned int chairBuildColor = 0x8E8E8000;
		unsigned int chairUnbuildColor = 0xD3CFCE00;
		int counter = 0;
		while (counter < 3 && confirmChairAvail()) {
			gen.HandleHotkeys();
			waitForMenu();
			//select the  chair
			mouse.MouseMoveArea(larderOnMenu);
			mouse.RightClick();
			Sleep(130 + (rand() % 10));
			gen.ChooseMenuOption(0);
			Sleep(10 + (rand() % 20));
			//build it
			mouse.LeftClick();

			waitForChairBuild(larderOnScreen, chairBuildColor);

			//now remove it
			gen.DefiniteClick(chairBuildColor, 3, larderOnScreen, HOVER_ACTION, HOVER_ACTION, 3, 20);
			//yes really you dummy.
			handleDialogBox(0);

			counter++;
			if (counter < 3)
			{
				//finally, click on the goddamn chair to build again.
				gen.DefiniteClick(chairUnbuildColor, 3, emptyLarderOS, 0xFFFFFF00, HOVER_ACTION, 2, 20);
				waitForMenu();
			}
		}
		chairs += counter;
		lardsSinceLog += counter;
		return true;
	}


	

public:

	BotConstructionLarderGlory()
	{
		
	}

	void run() {

		while (gen.VerifyOSbuddy())
		{
			gen.HandleHotkeys();
			gen.HandleAutoLogOut();
			
			if (!teleportToHouse())
				return;

			if (!buildChairLoop())
				return;

			handleRunEnergy();

			if (!get2Edgeville())
				return;

			if (!get2bank()) {
				printf("can't find bank\n");
				return;
			}

			if (!withdrawHouseSupplies()) { return; }

			printf("%d (%d)\n", chairs/3,chairs);

			if (lardsSinceLog > 300){
				gen.Logout();
				Sleep(10000);
				gen.HandleAutoLogOut();
				Sleep(500);
				lardsSinceLog = 0;
			}

		}


	}


};