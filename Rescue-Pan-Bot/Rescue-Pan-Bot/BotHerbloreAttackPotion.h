/* ========================================================================= */
/*
BotHerbloreAttackPotion.h (no matching .cpp file)
Authored by Brandon Wallace and Connor Rainen.

For herblore XP by making attack potions at the GE

SKILL: Herblore
HOURLY:		Money: - 30.5kGP/hr		XP: + 30.5k XP/hr
Requirements:
- Lvl 3 herblore required
- Location:		Begin in edgeville bank, top left booth. Bank closed.
- Bank:			Have "Tab 7" contain a stack of Water vials, eyes of newt, and guam leafs. (3,1-5,1)
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


class BotHerbloreAttackPotion {
private:
	InterfaceBank bank;
	InterfaceGeneral gen;
	InterfaceInventory inv;
	PixelHandler pix;
	Mouse mouse;

	Area bankCoords = gen.areaBox(2741 - 1920, 584, 20);
	unsigned int guamBankColor = 0x033C0500;
	unsigned int eyeBankColor = 0xDCD8D800;
	Pixel guamPotionChat;
	Pixel attackPotionChat;

	Pixel guamPotionInv;
	Pixel attackPotionInv;
	Pixel levelUpPixel;

	int pots = 0;


	void setX() {
		bank.OpenBank(bankCoords);
		bank.OpenTab(7);
		bank.DepositBackpack();
		bank.Withdraw(3, 1, 4);
		Sleep(800);
		Keyboard().TypeNum(9, 1);
		Sleep(1000);
		inv.MoveToItem(0);
		mouse.RightClick();
		gen.ChooseMenuOption(3);
		Sleep(120);
		mouse.LeftClick();
		bank.CloseBank();
	}

	bool bankPart() {
		bank.OpenBank(bankCoords);
		if (!bank.VerifyItem(guamBankColor, 5, 1))
			return false;
		if (!bank.VerifyItem(eyeBankColor, 4, 1))
			return false;
		bank.OpenTab(7);
		bank.DepositBackpack();
		bank.Withdraw(3, 1, 3);
		bank.Withdraw(4, 1, 3);
		bank.Withdraw(5, 1, 3);
		bank.CloseBank();
		return true;
	}

	bool handleDialog(Pixel pp) {
		int timeout = 0;
		while (!pix.VerifyPixelColor(pp._color,pp._x,pp._y,4)) {
			timeout++;
			Sleep(10);
			if (timeout > 90) {
				printf("dialog never came\n");
				return false;
			}
		}
		mouse.MouseMoveArea(gen.areaBox(pp._x, pp._y, 1));
		mouse.RightClick();
		Sleep(20);
		gen.ChooseMenuOption(3); 
		Sleep(120);
		mouse.LeftClick();
	}

	bool waitForPotion(Pixel p, int timeout) {
		int tt = 0;
		while (inv.VerifyInventoryOpen()) {
			tt++;
			if (pix.VerifyPixelColor(p._color,p._x,p._y,3)) {
				return true;
			}
			if (pix.VerifyPixelColor(levelUpPixel._color,levelUpPixel._x,levelUpPixel._y,3) || tt > timeout) {
				printf("Timeout :(\n");
				return false;
			}
		}
	}

	void useHerbOnWater() {
		unsigned int herbColor = 0x022E0200;
		unsigned int vialColor = 0x777DAA00;

		Sleep(150);
		inv.VerifyActiveInventory();
		inv.MoveToItem(herbColor);//move to an herb
		mouse.LeftClick();
		Sleep(50 + (rand() % 20));
		inv.MoveToItem(vialColor);//use on a vial of water
		mouse.LeftClick();
	}

	void useEyeOnPotion() {
		unsigned int eyeColor = 0xD1CBCB00;
		unsigned int potionColor = 0x8CA6A700;

		inv.MoveToItem(eyeColor);//move to an eye
		mouse.LeftClick();
		Sleep(50 + (rand() % 20));
		inv.MoveToItem(potionColor);//use on a potion
		mouse.LeftClick();
	}

	bool herbPart() {
		useHerbOnWater();
		if (!handleDialog(guamPotionChat))
			return false;
		if (!waitForPotion(guamPotionInv,300) && inv.VerifyInventoryOpen()) {
			printf("Level up!\n");
			useHerbOnWater();
			handleDialog(guamPotionChat);
			waitForPotion(guamPotionInv, 300);
		}
		useEyeOnPotion();
		if (!handleDialog(attackPotionChat))
			return false;
		if (!waitForPotion(attackPotionInv, 300) && inv.VerifyInventoryOpen()) {
			printf("Level up!\n");
			useEyeOnPotion();
			handleDialog(attackPotionChat);
			waitForPotion(attackPotionInv, 300);
		}
		return true;
	}

public:

	BotHerbloreAttackPotion() {
		guamPotionChat.Set(0x907E3400, 2179 - 1920 + SCREEN, 924);
		attackPotionChat.Set(0x907E3400, 2179 - 1920 + SCREEN, 924);
		guamPotionInv.Set(0x8CA5A700, 3373 - 1920 + SCREEN, 844);
		attackPotionInv.Set(0x34C9CF00, 3375 - 1920 + SCREEN, 843);

		levelUpPixel.Set(0x034A0500, 1963 - 1920 + SCREEN, 945);
	}

	void run() {
		setX();
		while (true) {
			gen.HandleHotkeys();
			gen.HandleAutoLogOut();
			if (!bankPart()) {
				printf("Out of guam\n");
				return;
			}
			if (!herbPart())
				return;
			pots += 9;
			printf("Made %d potions for %d XP \n", pots, pots * 25);
		}


	}


};