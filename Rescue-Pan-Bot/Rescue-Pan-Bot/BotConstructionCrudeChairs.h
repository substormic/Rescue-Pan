/* ========================================================================= */
/*
BotConstructionCrudeChairs.h (no matching .cpp file)
Authored by Brandon Wallace and Connor Rainen.

For creating crude wooden chairs at a POH

SKILL: Construction
HOURLY:		Money: - ???		XP: + ???
Requirements:
- No minimum skill required
- Location:		Begin anywhere.
- Bank:			Have "Tab 7" contain a stack of planks (1,1) (2nd row, 2nd col)
- Inventory:	Law runes (0) Air runes (1) Hammer (2) Saw (3) Nails (4) Planks (rest)
- Map:			Map normalized to OSBuddy def. zoom, facing north, UP.

Independence: Frequent monitor (3/5)

note: Expects parlor north of portal.
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

class BotConstructionCrudeChairs {
private:
	InterfaceBank bank;
	InterfaceGeneral gen;
	InterfaceInventory inv;
	PixelHandler pix;
	Mouse mouse;

	Pixel House;
	Pixel Camelot;
	unsigned int GhostChair2 = 0xcbc6c200; //connor's ultra white.
	unsigned int GhostChair = 0xC1BCB300;
	int chairs = 0;
	
	bool verifyInventorySetup() {
		inv.VerifyActiveInventory();
		if (!inv.SearchIndexForColor(0, 0x0C11D600)) {
			printf("Must have law runes in index 0 (top left)");
			return false;
		}
		if (!inv.SearchIndexForColor(1, 0xDCD8D800)) {
			printf("Must have air runes in index 1");
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
			Area treeSearch2 = gen.areaBox(2262 - 1920 + SCREEN, 409, 35);
			Area bankSearch1 = gen.areaBox(2077 - 1920, 154, 15,30);
			unsigned int treeColor = 0x73522900;
			unsigned int treeColor2 = 0x77542A00;
			if (!gen.DefiniteClick(treeColor, 10, treeSearch1, HOVER_ACTION, HOVER_ACTION, 0, 70)) {
				printf("Can't find the first tree outside cammy tele.");
				return false;
			}
			Sleep(4000);
			if (!gen.DefiniteClick(treeColor2, 10, treeSearch2, HOVER_ACTION, HOVER_ACTION, 0, 30)) {
				printf("Can't find the second tree outside cammy tele.");
				return false;
			}
			Sleep(4000);
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
		if (!bank.VerifyItem(0x5A482A00, 1, 1)) {
			printf("No planks are at index 1,1 in tab 7\n");
			return false;
		}
		bank.Withdraw(1, 1, 6);
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
		while (pix.VerifyPixelColor(color,p1.x,p1.y) && pix.VerifyPixelColor(color, p2.x, p2.y) && pix.VerifyPixelColor(color, p3.x, p3.y)){
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

		Area ChairClick = inv.areaBox(2680 - 1920 + SCREEN, 215, 35);
		POINT MoveChair = pix.SearchPixelAreaForPoint(GhostChair, ChairClick.x1, ChairClick.y1, ChairClick.x2, ChairClick.y2, 5);
		if (MoveChair.x != -1)
		{
			mouse.MouseMove(MoveChair);
			Sleep(150);
			mouse.RightClick();
			Sleep(60);
			inv.ChooseMenuOption(2);
			Sleep(50);
			mouse.LeftClick();
		}
		else
		{
			printf("Couldnt find chair\n");
			return false;
		}
		return true;
	}
	bool waitForChairBuild(Area cb, unsigned int color) {
		int timeout = 0;
		while (!pix.SearchPixelArea(color, cb.x1, cb.y1, cb.x2, cb.y2, 5)){
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

	bool confirmChairAvail() {
		unsigned int red = 0xFF000000;
		return (!pix.VerifyPixelColor(red, 2401 - 1920 + SCREEN, 356));
	}

	bool waitForMenu() {
		unsigned int col = 0xFF981F00;
		int timeout = 0;
		while (!pix.VerifyPixelColor(col, 2557 - 1920 + SCREEN, 327)) {
			Sleep(5);
			timeout++;
			if (timeout > 1000) {
				return false;
			}
		}
		return true;
	}

	void handleDialogBox(int optionNum) {
		while (!pix.VerifyPixelColor(0x1C928E00, 2338-1920+SCREEN, 903, 1)) {
			Sleep(5);
		}
		Keyboard().TypeNum(optionNum + 1);
	}

	//note: assumes chair menu is started open
	bool buildChairLoop() {
		inv.VerifyActiveInventory();
		Area chair = gen.areaBox(2410 - 1920 + SCREEN, 363, 2);
		Area chairBuildA = gen.areaBox(2745 - 1920 + SCREEN, 488, 25); //from below, first time 
		Area chairBuildB = gen.areaBox(2735 - 1920 + SCREEN, 578, 40); //from above
		unsigned int chairBuildColor = 0x271E0500;
		unsigned int chairUnbuildColor = 0xBDB7AF00;
		int counter = 0;
		while (confirmChairAvail()) {
			gen.HandleHotkeys();
			waitForMenu();
			Area cb = counter ? chairBuildB : chairBuildA;
			//select the shit chair
			mouse.MouseMoveArea(chair);
			mouse.RightClick();
			Sleep(50 + (rand() % 20));
			gen.ChooseMenuOption(0);
			Sleep(30 + (rand() % 20));
			//build it
			mouse.LeftClick();
			
			waitForChairBuild(cb, chairBuildColor);
			
			//now remove it
			gen.DefiniteClick(chairBuildColor, 3, cb, HOVER_ACTION, HOVER_ACTION, 3, 20);
			//yes really you dumb shit.
			handleDialogBox(0);

			counter++;
			cb = counter ? chairBuildB : chairBuildA;
			
			//finally, click on the goddamn chair to build again.
			gen.DefiniteClick(chairUnbuildColor, 3, cb, 0xFFFFFF00, HOVER_ACTION, 2, 20);
			waitForMenu();
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

	BotConstructionCrudeChairs()
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

			if (!openCammyBank()) { return; }

			if (!withdrawHouseSupplies()) { return; }

			if (!teleportToHouse()) { return; }

			if (!buildChairLoop()) { return; }

			printf("You've made %d chairs for %d XP\n",chairs, chairs * 58);

		}


	}


};