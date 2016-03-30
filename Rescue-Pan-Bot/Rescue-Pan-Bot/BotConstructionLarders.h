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
- Inventory:	Law runes (0) Air runes (1) Hammer (2) Saw (3) Nails (4) Planks (rest)
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

class BotConstructionOakLarders {
private:
	InterfaceBank bank;
	InterfaceGeneral gen;
	InterfaceInventory inv;
	PixelHandler pix;
	Mouse mouse;

	Pixel House;
	Pixel Camelot;
	unsigned int GhostChair2 = 0xcbc6c200; //connor's ultra white.
	unsigned int ghostLarder = 0xD9D5D400;
	int chairs = 0;

	bool verifyInventorySetup() {
		inv.VerifyActiveInventory();
		if (!inv.SearchIndexForColor(0, 0x0C11D600)) {
			printf("Must have law runes in index 0 (top left)");
			return false;
		}
		
		if (!inv.SearchIndexForColor(1, 0x38210300)) {
			printf("Must have hammer in index 2");
			return false;
		}
		if (!inv.SearchIndexForColor(1, 0x38210300)) {
			printf("Must have saw in index 3");
			return false;
		}
		return true;
	}

	//Teleports to camelot, opens bank
	bool openCammyBank() {
		if (!Teleport(Camelot)) {
			printf("Teleport failed, lacked runes\n");
			return false;
		}
		openCammyBank(false);
	}

	//Overload which allows you not to waste a rune if you just teleported.
	bool openCammyBank(bool teleport) {
		if (!teleport) {
			gen.NormalizeCompass(UP);
			Area treeSearch1;
			treeSearch1.x1 = 2128 - 1920 + SCREEN;
			treeSearch1.y1 = 245;
			treeSearch1.x2 = 2414 - 1920 + SCREEN;
			treeSearch1.y2 = 569;
			Area treeSearch2 = gen.areaBox(2262 - 1920 + SCREEN, 409, 55);
			Area bankSearch1 = gen.areaBox(2077 - 1920, 154, 15, 30);
			unsigned int treeColor = 0x73522900;
			unsigned int treeColor2 = 0x77542A00;
			if (!gen.DefiniteClick(treeColor, 10, treeSearch1, HOVER_ACTION, HOVER_ACTION, 0, 70)) {
				printf("Can't find the first tree outside cammy tele.");
				return false;
			}
			Sleep(4000);
			if (!gen.DefiniteClick(treeColor2, 10, treeSearch2, HOVER_ACTION, HOVER_ACTION, 0, 70)) {
				printf("Can't find the second tree outside cammy tele.");
				return false;
			}
			Sleep(5250 + (rand() % 250));
			gen.NormalizeCompass(0);
			if (!bank.OpenBank(bankSearch1)) {
				printf("bank didn't open or something\n");
				return false;
			}
			return true;
		}
		else {
			return openCammyBank();
		}
	}

	bool withdrawHouseSupplies() {
		if (!bank.VerifyBankOpen()) {
			printf("Bank should've been open\n");
			return false;
		}
		bank.OpenTab(7);
		Sleep(100 + (rand() % 40));
		if (!bank.VerifyItem(0x6A503100, 2, 1)) {
			printf("No oak planks are at index 1,1 in tab 7\n");
			return false;
		}
		bank.Withdraw(2, 1, 5); //withdraw all, so we can run out
		Sleep(50);
		bank.CloseBank();
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
		if (!Teleport(House))
			return false; // could not teleport to house

		houseLoaded();
		gen.NormalizeCompass(UP);
		inv.ActivateBuildingMode();
		houseLoaded();
		Sleep(100);

		Area lardClick = inv.areaBox(2907 - 1920 + SCREEN, 143, 110, 35);
		POINT moveLard = pix.SearchPixelAreaForPoint(ghostLarder, lardClick.x1, lardClick.y1, lardClick.x2, lardClick.y2, 6);
		if (moveLard.x != -1)
		{
			mouse.MouseMove(moveLard);
			Sleep(150);
			mouse.RightClick();
			Sleep(60);
			inv.ChooseMenuOption(2);
			Sleep(50);
			mouse.LeftClick();
		}
		Sleep(5000000);
		return;
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
		printf("Chair built!\n");
		return true;
	}

	void handleRunEnergy() {
		Pixel pixeltowatch;
		pixeltowatch.Set(0x131313, 3408 - 1920 + SCREEN, 176);
		Pixel b = pixeltowatch;
		b._color = 0xA2A39500;
		if ( pix.VerifyPixelColor(b)) {
			mouse.MouseMove(pixeltowatch._x, pixeltowatch._y);
			mouse.LeftClick();
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
		Keyboard().TypeNum(optionNum + 1);
	}

	//note: assumes chair menu is started open
	bool buildChairLoop() {
		inv.VerifyActiveInventory();
		Area larderOnMenu = gen.areaBox(2404 - 1920 + SCREEN, 474, 2);
		Area larderOnScreen = gen.areaBox(2783 - 1920 + SCREEN, 455, 40); 
		Area emptyLarderOS = gen.areaBox(2739 - 1920 + SCREEN, 426, 40);
		unsigned int chairBuildColor = 0x8E8E8000;
		unsigned int chairUnbuildColor = 0xD3CFCE00;
		int counter = 0;
		while (counter < 3 && confirmChairAvail()) {
			gen.HandleHotkeys();
			waitForMenu();
			//select the shit chair
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
			//yes really you dumb shit.
			handleDialogBox(0);

			counter++;
			if (counter < 3)
			{
				//finally, click on the goddamn chair to build again.
				gen.DefiniteClick(chairUnbuildColor, 3, emptyLarderOS, 0xFFFFFF00, HOVER_ACTION, 2, 20);
				waitForMenu();
			}
		}
		printf("Out of wood\n");
		chairs += counter;
		return true;
	}


	bool Teleport(Pixel tele)
	{
		bool result = inv.PrepareSpell(tele);
		if (!result)
		{
			printf("=========== ERROR: No Runes to cast teleport! ===============\n");
		}
		Sleep(2000);
		return result;
	}

public:

	BotConstructionOakLarders()
	{
		Camelot._x = 5;
		Camelot._y = 3;
		Camelot._color = TELEBLOCK._color;
		House._x = 2;
		House._y = 3;
		House._color = TELEBLOCK._color;
	}

	void run() {

		while (gen.VerifyOSbuddy())
		{
			gen.HandleHotkeys();
			gen.HandleAutoLogOut();
			handleRunEnergy();

			openCammyBank();

			if (!withdrawHouseSupplies()) { return; }

			teleportToHouse();

			buildChairLoop();

			printf("You've made %d larders for %d XP\n", chairs, chairs * 480);

			if (chairs / 3 >= 300 && chairs / 3 < 302) {
				gen.Logout();
				Sleep(10000);
				gen.HandleAutoLogOut();
			}

		}


	}


};