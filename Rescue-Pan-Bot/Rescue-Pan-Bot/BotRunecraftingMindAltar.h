/* ========================================================================= */
/*
BotHerbloreAttackPotion.h (no matching .cpp file)
Authored by Brandon Wallace and Connor Rainen.

For making runes at the mind altar.

SKILL: Runecrafting
HOURLY:		Money: - 30.5kGP/hr		XP: + 30.5k XP/hr
Requirements:
- Lvl 3 herblore required
- Location:		Begin anywhere!
- Bank:			Have "Tab 7" contain a stack of Water vials, eyes of newt, and guam leafs. (3,1-5,1)
- Inventory:	Empty please!
- Map:			Map normalized to OSBuddy def. zoom, facing north, UP.

Note:
	requires ARCEUS spells!

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
#include "InterfaceGeneral.h"
#include "InterfaceBank.h"
#include "InterfaceInventory.h"


class BotRunecraftingMindAltar {
private:
	InterfaceBank bank;
	InterfaceGeneral gen;
	InterfaceInventory inv;
	PixelHandler pix;
	Mouse mouse;

	bool openBank() {
		gen.ChangeCompassDegrees(180);
		Area b = gen.areaBox(2947 - 1920, 89, 100);
		Area b2 = gen.areaBox(2813 - 1920, 499, 60);
		if (!bank.OpenBank(b))
			if (!bank.OpenBank(b2))
				return false;
		bank.OpenTab(7);
		Sleep(150);
		if (!bank.VerifyItem(0x756A6A00, 4, 2)) {
			printf("No pure essence in index (4,2)\n");
			return false;
		}
		bank.Withdraw(4, 2, 5);
		bank.CloseBank();
		return true;
	}

	bool invValid() {
		if (!inv.VerifyItem(0x51494900, 0)) {
			printf("No home tele tabs in slot 1");
			return false;
		}
		if (!inv.VerifyItem(0x0C11D600, 1)) {
			printf("No law runes in slot 2");
			return false;
		}
		if (!inv.VerifyItem(0xD66F0C00, 2)) {
			printf("No mind runes in slot 3");
			return false;
		}
	}

	bool getToEdgeville() {
		inv.VerifyActiveInventory();
		inv.MoveToItem(0);
		Sleep(50);
		mouse.LeftClick();
		Pixel a, b;
		a.Set(0x00000000, 2518 - 1920 + SCREEN, 788);
		b.Set(0x00, 3166 - 1920 + SCREEN, 866);
		int timeout = 0;
		while (!pix.VerifyPixelColor(a) && !pix.VerifyPixelColor(b)) {
			gen.HandleHotkeys();
			Sleep(5);
			timeout++;
			if (timeout > 100) {
				printf("Never got to house\n");
				return false;
			}
		}
		timeout = 0;
		while (pix.VerifyPixelColor(a) && pix.VerifyPixelColor(b)) {
			gen.HandleHotkeys();
			Sleep(5);
			timeout++;
			if (timeout > 100) {
				printf("House never loaded\n");
				return false;
			}
		}
		gen.DefiniteClick(0x7209a000, 2, gen.areaBox(2455 - 1920 + SCREEN, 399, 50), HOVER_ACTION, HOVER_ACTION, 0, 20,147,-1);
		Sleep(7000);
		return true;
	}

	bool getToAltar() {
		inv.VerifyActiveMagic();
		mouse.MouseMove(3406 - 1920 + SCREEN + (rand() % 5), 839);
		mouse.LeftClick();
		Sleep(3000);
		gen.NormalizeCompass(UP);
		if (!gen.DefiniteClick(0xff000000, 0, gen.areaBox(2875 - 1920 + SCREEN, 248, 150), HOVER_ACTION, HOVER_ACTION, 0, 150,-1,48))
			return false;
		Sleep(3500);
		return true;
	}

	bool craftRunes() {
		if (!gen.DefiniteClick(0xff000000, 0, gen.areaBox(2379 - 1920 + SCREEN, 97, 100), HOVER_ACTION, HOVER_ACTION, 0, 70))
			return false;

		Sleep(10000);
		return true;
	}

public:
	void run() {

		InterfaceBank().openEdgevilleBank(1);
		return;


		gen.NormalizeCompass(UP);
		while (true) {
			gen.HandleHotkeys();
			if (!getToEdgeville())
				return;
			if (!openBank())
				return;
			gen.HandleHotkeys();
			if (!invValid())
				return;
			if (!getToAltar())
				return;
			if (!craftRunes())
				return;
			
		}
	}


};