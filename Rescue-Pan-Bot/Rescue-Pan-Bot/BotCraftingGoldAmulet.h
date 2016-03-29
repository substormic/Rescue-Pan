/* ========================================================================= */
/*
BotCraftingGoldAmulet.h (no matching .cpp file)
Authored by Brandon Wallace and Connor Rainen.

For smelting gold bars into unstrung amulets at a furnace. 

SKILL: Crafting
HOURLY:		Money: + 2.5k		XP: + 35k
Requirements:
	- No minimum skill required
	- Location:		Stand in front of the northeastern-most bank booth in edgeville
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

class BotCraftingGoldAmulet {
private:
	InterfaceBank bank;
	InterfaceGeneral gen;
	InterfaceInventory inv;
	PixelHandler pix;
	Mouse mouse;

	unsigned int furnaceColor = 0x44261800; //0x895F0600; 
	unsigned int goldColor = 0x5B450500;

	unsigned int furnaceColor2 = 0x4D2E1B00;
	
	
	
	Area bReg2;

	bool openFurnaceBackup() {
		Area backup = box(2608 - 1920 + SCREEN, 673, 10);

		if (!gen.DefiniteClick(0x4D2E1B00, 10, backup, HOVER_ACTION, HOVER_ACTION, 0, 20)) {
			return false;
		}
		Sleep(6000); 
		return true;
	}

	bool craft(int amt) {
		int to = 0;
		while (!pix.VerifyPixelColor(0x7D850700, 2468 - 1920 + SCREEN, 507, 5)) {
			Sleep(10);
			to++;
			if (to == 400) {
				printf("Interface never opened.\n");
				inv.MoveToItem(27);
				mouse.LeftClick();
				mouse.MouseMove(2795 - 1920 + SCREEN, 540);
				int my = 540;
					while (!gen.VerifyTopLeftText(HOVER_ACTION) && my > 520) {
						my--;
						mouse.MouseMove(2795 - 1920 + SCREEN, my);
						Sleep(100);
					}
					my = 547;
					int mx = 2795;
					while (!gen.VerifyTopLeftText(HOVER_ACTION) && mx < 2850) {
						mx++;
						mouse.MouseMove(mx - 1920 + SCREEN, my);
						Sleep(75);
					}
				mouse.LeftClick();
			}
			if (to > 800){
				return false;
			}
		}
		mouse.MouseMove(2467 - 1920 + SCREEN, 509);
		Sleep(100);
		mouse.RightClick();
		gen.ChooseMenuOption(3);
		mouse.LeftClick();
		Sleep(800);
		Keyboard().TypeNum(amt);
		Sleep(1.8 * amt * 1000);
		return true;
	}

	bool cFfromF() {
		int timeout = 0;
		Area b = box(2810 - 1920 + SCREEN, 530, 14);
		while (timeout < 24) {
			timeout++;
			mouse.MouseMoveArea(b);
			Sleep(100);
			if (gen.VerifyTopLeftText(HOVER_ACTION)) {
				mouse.LeftClick();
				return true;
			}
		}
		return false;

	}

	Area box(int x, int y, int r) {
		Area t;
		t.x1 = x - r;
		t.y1 = y - r;
		t.x2 = x + r;
		t.y2 = y + r;
		return t;
	}

public:

	void run() {
		int ams = 0;

		Area bRegA = box(2753 - 1920, 590, 40);

		Area fReg = box(3334 - 1920 + SCREEN, 350, 60);

		Area fReg2 = box(2820 - 1920 + SCREEN, 503, 10);
		
		Area bRegB = box(2700 - 1920, 820, 50);

		if (!inv.VerifyItem(0xA79A9900, 0)) {
			printf("You need an amulet mould in inventory spot 0\n");
			return;
		}

		//open bank from bank
		if (!bank.OpenBank(bRegA)) {
			printf("Couldn't get bank open\n");
			return;
		}


		while (true)
		{
			bank.OpenTab(7);
			Sleep(120);
			if (!bank.VerifyItem(goldColor, 0, 1)) {
				printf("Gold isn't there in the bank\n");
				return;
			}
			
			//withdraw all but one gold.
			bank.Withdraw(0, 1, 6);
			bank.CloseBank();
			
			//click furnace
			if (!gen.DefiniteClick(furnaceColor, 2, fReg, HOVER_ACTION, HOVER_ACTION, 0, 20)) {
				Area aa = box(3435 - 1920 + SCREEN, 303, 50);
				Area bb = box(3361 - 1920 + SCREEN, 242, 30);
				if (!gen.DefiniteClick(furnaceColor, 2, aa, HOVER_ACTION, HOVER_ACTION, 0, 20)) {
					if (!gen.DefiniteClick(furnaceColor, 2, bb, HOVER_ACTION, HOVER_ACTION, 0, 20)) {
						printf("Can't find furnace!\n");
						return;
					}

				}
				printf("had to click from a far bank\n");
			}
			//select a gold bar in inventory
			mouse.MouseMove(inv.GetItemCoords(1));
			mouse.LeftClick();

			//wait for furnace to open up. get back to furnace if you clicked a tree you big stupid idiot.
			int timeout = 0;
			bool foundFurnaceThroughNormalMeans = true;
			while (!pix.SearchPixelArea(goldColor, 2220 - 1920 + SCREEN, 940, 2235 - 1920 + SCREEN, 955, 5)) {
				timeout++;
				Sleep(10);
				if (timeout > 300) {
					printf("Furnace never opened\n");
					if (!openFurnaceBackup()) {
						return;
					}
					else {
						foundFurnaceThroughNormalMeans = false;
						break;
					}
				}
			}

			//this ensures clicktoFurnace accuracy.
			Sleep(400);

			//use gold bar on furnace
			if (foundFurnaceThroughNormalMeans && !cFfromF()) {
				printf("Lost furnace\n"); 
				return; 
			}
			
			if (!craft(27)) {
				return;
			}

			//get back home to bank
			gen.DefiniteClick(0x63605C00, 0, box(2159 - 1920 + SCREEN, 532, 58), HOVER_ACTION, HOVER_ACTION, 0, 85);
			Sleep(6000);


			if (!bank.OpenBank(bRegB)) {
				printf("Couldn't get bank open\n");
				return;
			}

			//deposit things
			inv.MoveToItem(1);
			mouse.RightClick();
			Sleep(100);
			gen.ChooseMenuOption(5);
			mouse.LeftClick();
			Sleep(100);
			ams += 27;
			printf("Deposited a round of amulets. Made %d and %d XP so far\n", ams, ams * 30);
		}


	}



};