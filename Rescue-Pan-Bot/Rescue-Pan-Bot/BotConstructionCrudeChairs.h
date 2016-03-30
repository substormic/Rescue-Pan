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

	}

	//Overload which allows you not to waste a rune if you just teleported.
	bool openCammyBank(bool teleport) {
		if (!teleport) {


		}
		else {
			openCammyBank();
		}
	}

	//Teleports to house; enables build mode; clicks "build" on the left chair spot.
	bool teleportToHouse() {
		if (!Teleport(House))
			return false; //couldnt teleport

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


	}


};