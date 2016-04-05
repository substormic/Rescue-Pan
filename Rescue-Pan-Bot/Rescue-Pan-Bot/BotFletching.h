/* ========================================================================= */
/*
BotFletching.h (no matching .cpp file)
Authored by Brandon Wallace and Connor Rainen.

For fletching, stringing bows

SKILL: Fletching
HOURLY:		Money: - ???	XP:  ???
Requirements:
- No minimum level
- Location:		Begin in edgeville bank, top left booth. Bank closed.
- Bank:			Have "Tab 7" contain bow strings (0, 2) and bows (1, 2)
- Inventory:	Empty please!
- Map:			Map normalized to OSBuddy def. zoom, facing north, UP.

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


class BotFletching {
private:
	InterfaceBank bank;
	InterfaceGeneral gen;
	InterfaceInventory inv;
	PixelHandler pix;
	Mouse mouse;

	Area bankCoords = gen.areaBox(2741 - 1920, 584, 20);
	Pixel dialog;

	unsigned int bowstringColor = 0x685C3E00;

	int c = 0;

	void setX() {
		bank.OpenBank(bankCoords);
		bank.OpenTab(7);
		bank.DepositBackpack();
		bank.Withdraw(0, 2, 4);
		Sleep(800);
		Keyboard().TypeNum(14, 2);
		Sleep(1000);
		inv.MoveToItem(0);
		mouse.RightClick();
		gen.ChooseMenuOption(3);
		Sleep(120);
		mouse.LeftClick();
		bank.CloseBank();
	}


	bool bankPart() {
		if (!bank.OpenBank(bankCoords)) {
			gen.NormalizeCompass(UP);
			if (!bank.OpenBank(bankCoords))
				return false;
		}
		bank.OpenTab(7);
		if (!bank.VerifyItem(bowstringColor, 0, 2)) {
			printf("Out of bowstrings!\n");
				return false;
		}
		bank.DepositBackpack();
		bank.Withdraw(0, 2, 3);
		bank.Withdraw(1, 2, 3);
		bank.CloseBank();
		Sleep(160 + (rand() % 35));
		return true;
	}

	bool lvlup() {
		return pix.VerifyPixelColor(0x52090500, 2029 - 1920 + SCREEN, 911, 2);
	}

	bool fletchPart() {
		int timeout = 0;
		bool leveled = false;
		inv.MoveToItem(14);
		Sleep(10);
		mouse.LeftClick();
		Sleep(10 + (rand() % 20));
		inv.MoveToItem(13);
		Sleep(10);
		mouse.LeftClick();
		if (!handleDialogBox()) {
			printf("Trying one more time\n");
			inv.MoveToItem(14);
			Sleep(10);
			mouse.LeftClick();
			Sleep(10 + (rand() % 20));
			inv.MoveToItem(13);
			Sleep(10);
			mouse.LeftClick();
			if (!handleDialogBox())
				return false;
		}

		while (inv.VerifyItem(bowstringColor, 13)) {
			timeout++;
			if (lvlup() || timeout > 400) {
				if (leveled) {
					printf("We already tried again and you timed out again\n");
					return false;
				}
				printf("You leveled up!\n");
				inv.MoveToItem(27);
				mouse.LeftClick();
				inv.MoveToItem(13);
				mouse.LeftClick();
				if (!handleDialogBox())
					return false;
				leveled = true;
				timeout = 0;
			}
		}
		return true;
	}

	bool handleDialogBox() {
		int timeout = 0;
		while (!pix.VerifyPixelColor(dialog._color, dialog._x, dialog._y, 2)) {
			Sleep(10);
			timeout++;
			if (timeout > 200) {
				printf("Dialog never opened\n");
				return false;
			}
		}
		mouse.MouseMoveArea(gen.areaBox(2178 - 1920 + SCREEN, 932, 2));
		mouse.RightClick();
		gen.ChooseMenuOption(3);
		mouse.LeftClick();
		return true;
	}

public:
	BotFletching() {
		dialog.Set(0x1C928E00, 2405 - 1920 + SCREEN, 907);
	}

	void run() {
		setX();
		while (gen.VerifyOSbuddy()) {
			gen.HandleHotkeys();
			gen.HandleAutoLogOut();
			
			if (!bankPart()) {
				printf("Bank error\n");
				return;
			}
			if (!fletchPart()) {
				printf("Fletch error\n");
				return;
			}
			c++;
			printf("Made %d bows\n", c);

		}


	}

};