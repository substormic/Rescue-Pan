/* ========================================================================= */
/*
BotCookingRoguesDen.h (no matching .cpp file)
Authored by Brandon Wallace and Connor Rainen.

For running the varrock rooftop agility course with OSBuddy Pro

SKILL: Cooking
HOURLY:		Money: Variable	XP: + 85k/hr (with tuna) (150k/hr with sweetcorn)
Requirements:
- No minimum skill required. If lvl 1, start with shrimps!
- Location:		Be in the rogues den, directly south of the fireplace.
- Bank:			Have "Tab 7" contain a food that can be cooked over a fire in index (3,2)
- Inventory:	Empty please!
- Map:			Map normalized to OSBuddy default zoom, facing north, UP.

Independence: Lazy monitor		(4/5)

note: this bot will need to be reset each time you run out of a food in your inventory.

note: Independence scale:
Active play			(1/5)
Intermittent play	(2/5)
Frequent monitor	(3/5)
Lazy monitor		(4/5)
Perfect Automation	(5/5)
*/
/* ========================================================================= */
#pragma once

#include "InterfaceBank.h"
#include "InterfaceInventory.h"
#include "InterfaceGeneral.h"
#include "InterfaceStats.h"

class BotCookingRoguesDen {

	InterfaceBank bank;
	InterfaceGeneral gen;
	InterfaceInventory inv;
	InterfaceStats stats;
	PixelHandler pix;
	Mouse mouse;


	unsigned int fireColor = 0xB68C1600;
	unsigned int bankColor = 0x3B341500;
	unsigned int dialogBoxColor = 0x1C928E00;
	int dbX = 2405 - 1920 + SCREEN;
	int dbY = 907;

	Area benedictArea;
	Area fireArea;
	POINT lastValidRightClick;

	bool cookOpenBank() {
		gen.DefiniteClick(bankColor, 3, benedictArea, HOVER_NPC, HOVER_NPC, 1, 60);
		int timeout = 0;
		while (!bank.VerifyBankOpen()) {
			Sleep(5);
			timeout++;
			if (timeout > 200) {
				printf("bank didn't open\n");
				return false;
			}
		}
		return true;
	}

	bool bankPart() {
		if (!cookOpenBank()) {
			gen.NormalizeCompass(UP);
			if (!cookOpenBank())
				return false;
		}
		bank.OpenTab(7);
		bank.DepositBackpack();
		bank.Withdraw(3,2, 5); //todo: make this withdraw "all but one" to be safe?
		bank.CloseBank();
		Sleep(600 + (rand() % 200));
		return true;
	}

	bool handleDialogBox() {
		int timeout = 0;
		while (!pix.VerifyPixelColor(dialogBoxColor, dbX, dbY, 2)) {
			Sleep(5);
			timeout++;
			if (timeout > 150) {
				printf("Dialog box never opened\n");
				return false;
			}
		}
		
		mouse.MouseMoveArea(gen.areaBox(2183 - 1920 + SCREEN, 933, 3));
		mouse.RightClick();
		gen.ChooseMenuOption(3);
		mouse.LeftClick();
		return true;


	}

	bool handleCookLvlUp() {
		if (pix.VerifyPixelColor(0xA5821A00, 1987 - 1920 + SCREEN, 923, 4)) {
			printf("Level up!\n");
			inv.MoveToItem(27);
			mouse.LeftClick();
			if (!gen.DefiniteClick(fireColor, 5, fireArea, HOVER_ACTION, HOVER_ACTION, 0, 110)) {
				printf("Couldn't find fire\n");
				return false;
			}
			if (!handleDialogBox())
				return false;
			Sleep(1000);
			return true;

		}
		return true;
	}

	bool findFireFast() {
		int timeout = 0;
		while (timeout < 25) {
			timeout++;
			if (timeout == 0 && lastValidRightClick.x != -1) {

				mouse.MouseMove(lastValidRightClick);
			}
			else {
				mouse.MouseMoveArea(fireArea);
			}
			mouse.RightClick();
			Sleep(50 + (timeout * 10));
			POINT p = mouse.GetPosition();
			Area mouseCheck;
			mouseCheck.x1 = p.x - 40;
			mouseCheck.y1 = p.y + 15;
			mouseCheck.x2 = p.x + 80;
			mouseCheck.y2 = p.y + 300;
			POINT p2 = pix.SearchPixelAreaForPoint(HOVER_ACTION, mouseCheck);
			if (p2.x != -1) {
				p2.y -= 5;
				mouse.MouseMoveSLD(p2.x,p2.y);
				Area doubleCheck = gen.areaBox(p2, 3);
				if (!pix.VerifyPixelColor(HOVER_ACTION,mouse.GetPosition().x,mouse.GetPosition().y)){
				//if (!pix.SearchPixelArea(HOVER_ACTION, doubleCheck)) {
					printf("Had it and lost it\n");
					return false;
				}
				else {
					mouse.LeftClick();
					lastValidRightClick = p;
					return true;
				}

			}
			else {
				mouse.MouseMoveArea(gen.areaBox(3005-1920+SCREEN, 185, 400));
			}
		}
		printf("Sorry, didn't work");
		return false;
	}
	
	bool findFireSlow() {
		if (!gen.DefiniteClick(fireColor, 5, fireArea, HOVER_ACTION, HOVER_ACTION, 0, 110)) {
			if (!gen.DefiniteClick(fireColor, 10, fireArea, HOVER_ACTION, HOVER_ACTION, 0, 110)) {
				printf("Couldn't find fire\n");
				return false;
			}
		}
		return true;
	}

	bool cookPart() {
		inv.MoveToItem(0);
		mouse.LeftClick();
		POINT lastItem;
		lastItem.x = 3498 - 1920 + SCREEN;
		lastItem.y = 985;
		unsigned int lastItemColor = pix.GetPixelColor(lastItem.x, lastItem.y); //may need to adjust for different foods
		
		if (!findFireFast()) { //just going for it with an area
			if (!findFireSlow()) { //trying to find the "fire" blue text
				printf("Couldn't find the fire either way\n");
				return false;
			}
		}
		
		if (!handleDialogBox())
			return false;
		int timeout = 0;
		while (true)
		{
			gen.HandleAutoLogOut();
			timeout++;
			if (!handleCookLvlUp()) {
				printf("Problem with levelup protocol\n");
				return false;
			}
			if (!pix.VerifyPixelColor(lastItemColor, lastItem.x, lastItem.y, 2)) {
				printf("Finished cooking this inventory\n");
				return true;
			}
			if (timeout > 28000) {
				printf("Never finished cooking\n");
				return false;
			}
		}
		//can't reach this.
		return false;
	}


public:

	BotCookingRoguesDen() {
		/*benedictArea.x1 = 2672;
		benedictArea.y1 = 463;
		benedictArea.x2 = 2984;
		benedictArea.y2 = 920;*/

		benedictArea.x1 = 2669 - 1920 + SCREEN;
		benedictArea.y1 = 484;
		benedictArea.x2 = 2706 - 1920 + SCREEN;
		benedictArea.y2 = 583;

		fireArea.x1 = 2723 - 1920 + SCREEN;
		fireArea.y1 = 461;
		fireArea.x2 = 2776 - 1920 + SCREEN;
		fireArea.y2 = 505;

		lastValidRightClick.x = -1;
		lastValidRightClick.y = -1;

	}

	void run() {
		gen.NormalizeCompass(UP);

		while (gen.VerifyOSbuddy()) {
			gen.HandleHotkeys();
			gen.HandleAutoLogOut();
			
			if (!bankPart())
				return;

			if (!cookPart())
				return;
		}


	}


};