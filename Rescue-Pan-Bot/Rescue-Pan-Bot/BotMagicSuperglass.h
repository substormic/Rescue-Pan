/* ========================================================================= */
/*
BotMagicSuperglass.h (no matching .cpp file)
Authored by Brandon Wallace and Connor Rainen.

For a little magic, a lot of crafting XP, and $$$.

SKILL: Herblore
HOURLY:		Money: + ???	XP: + ???
Requirements:
- Lvl 77 magic, and lunars
- Location:		Begin in edgeville bank, top left booth. Bank closed.
- Bank:			Have "Tab 7" contain a stack of seaweed, sand (5,2 ; 6,2)
- Inventory:	Astrals in spot 1, wielding smoke staff.
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


class BotMagicSuperglass {
private:
	InterfaceBank bank;
	InterfaceGeneral gen;
	InterfaceInventory inv;
	PixelHandler pix;
	Mouse mouse;

	Area bankCoords = gen.areaBox(2744 - 1920, 580, 24);

	unsigned int seaweedBankColor = 0x05563800;
	unsigned int sandBankColor = 0x88693100;

	int invs = 0;


	void setX() {
		bank.OpenBank(bankCoords);
		bank.OpenTab(7);
		bank.Withdraw(5, 2, 4);
		Sleep(800);
		Keyboard().TypeNum(13, 2);
		Sleep(1000);
		bank.CloseBank();
		inv.VerifyActiveMagic();
		Sleep(400);
	}

	bool bankPart() {
		gen.HandleAutoLogOut();
		if (!bank.OpenBank(bankCoords,15))
			if (!bank.OpenBank(bankCoords, 20)) {
				
				gen.NormalizeCompass(UP);
				if (!bank.OpenBank(bankCoords, 80) && !bank.VerifyBankOpen()) {
					printf("bank didn't open\n");
					return false;
				}
			}

		bank.OpenTab(7);
		Sleep(40);

		if (!bank.VerifyItem(seaweedBankColor, 5, 2)) {
			Sleep(500);
			if (!bank.VerifyItem(seaweedBankColor, 5, 2))
				return false;
		}
		if (!bank.VerifyItem(sandBankColor, 6, 2))
			return false;

		
		//deposit glass
		inv.MoveToItem(1);
		mouse.RightClick();
		gen.ChooseMenuOption(5);
		mouse.LeftClick();

		bank.Withdraw(5, 2, 3);
		bank.Withdraw(6, 2, 3);
		bank.CloseBank();
		Sleep(100 + (rand() % 20));
		return true;
	}

	bool castSpell() {
		if (inv.VerifyActiveMagic())
			Sleep(500);
		Sleep(10 + (rand() % 10));
		if (!pix.VerifyPixelColor(0xE3416E00, 3528 - 1920 + SCREEN, 815, 2)) {
			printf("No spell");
			return false;
		}
		mouse.MouseMoveArea(gen.areaBox(3528 - 1920 + SCREEN, 815, 2));
		mouse.LeftClick();
		Sleep(1385 + (rand() % 10));
		return true;
	}

public:

	void run() {

		setX();
		gen.NormalizeCompass(UP);

		while (true) {
			gen.HandleHotkeys();
			gen.HandleAutoLogOut();
			if (!bankPart()) {
				printf("Out of supplies\n");
				return;
			}
			if (!castSpell())
				return;

			invs += 1;
			printf("Made %d inventories of glass\n", invs);
		}


	}

	void run(bool safeMode) {
		int logoutCountdown = 1800;
		if (safeMode) {
			setX();
			gen.NormalizeCompass(UP);
		}
		while (true) {
			gen.HandleHotkeys();
			gen.HandleAutoLogOut();
			if (!bankPart()) {
				printf("Out of supplies\n");
				return;
			}
			if (!castSpell())
				return;

			invs += 1;
			printf("Made %d inventories of glass\n", invs);
			logoutCountdown -= 1;
			
			if (logoutCountdown < 0) {
				gen.Logout();
				Sleep(10000 + (rand() % 100));
				gen.HandleAutoLogOut();
				Sleep(1000);
				logoutCountdown = 1800;

			}
		}


	}

};