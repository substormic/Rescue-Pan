/* ========================================================================= */
/*
BotConstructionCrudeChairs.h (no matching .cpp file)
Authored by Brandon Wallace and Connor Rainen.

For creating crude wooden chairs at a POH

SKILL: Construction
HOURLY:		Money: - ???		XP: + ???
Requirements:
- No minimum skill required
- Location:		Begin in Varrock square.
- Bank:			Have "Tab 7" contain a stack of gold bars in index (0,1)
- Inventory:	Hold an amulet mould in the top left of your inventory
- Map:			Map normalized to OSBuddy def. zoom, facing north, UP.

Independence: Lazy Monitor (4/5)

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

	unsigned int GhostChair = 0xcbc6c200;
	
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
		inv.VerifyActiveMagic();
		return true;

	}

	//Overload which allows you not to waste a rune if you just teleported.
	bool openCammyBank(bool teleport) {
		if (!teleport) {
			gen.NormalizeCompass(UP);
			Area treeSearch1;
			treeSearch1.x1 = 2207 - 1920 + SCREEN;
			treeSearch1.y1 = 245;
			treeSearch1.x2 = 2358 - 1920 + SCREEN;
			treeSearch1.y2 = 521;
			Area treeSearch2 = gen.areaBox(2257 - 1920 + SCREEN, 409, 10);
			Area bankSearch1 = gen.areaBox(1973 - 1920, 170, 10,30);
			unsigned int treeColor = 0x73522900;
			unsigned int treeColor2 = 0x77542A00;
			if (!gen.DefiniteClick(treeColor, 10, treeSearch1, HOVER_ACTION, HOVER_ACTION, 0, 30)) {
				printf("Can't find the first tree outside cammy tele.");
				return false;
			}
			Sleep(4000);
			if (!gen.DefiniteClick(treeColor2, 10, treeSearch2, HOVER_ACTION, HOVER_ACTION, 0, 30)) {
				printf("Can't find the second tree outside cammy tele.");
				return false;
			}
			Sleep(3000);
			gen.NormalizeCompass(0);
			if (!bank.OpenBank(bankSearch1)) {
				printf("bank didn't open or something\n");
				return false;
			}
			return true;
		}
		else {
			openCammyBank();
			return true;
		}
	}

	//Teleports to house; enables build mode; clicks "build" on the left chair spot.
	bool teleportToHouse() {
		//if (!Teleport(House))
		//	return false; // could not teleport to house
		Area ChairClick = inv.areaBox(817,274, 25);
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
			printf("Couldnt find chair\n");
		return true;
	}


	bool Teleport(Pixel tele)
	{
		bool result = inv.PrepareSpell(tele);
		if (!result)
		{
			printf("=========== ERROR: No Runes to cast teleport! ===============\n");
		}
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
		teleportToHouse();


	}


};