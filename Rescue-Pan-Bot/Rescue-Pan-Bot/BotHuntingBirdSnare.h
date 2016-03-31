#pragma once
#include "InterfaceGeneral.h"
#include "InterfaceBank.h"
#include "InterfaceInventory.h"
#include "Pixel.h"
#include "Mouse.h"

class BotHuntingBirdSnare {
private:
	InterfaceBank bank;
	InterfaceGeneral gen;
	InterfaceInventory inv;
	PixelHandler pix;
	Mouse mouse;

	Area trapArea;
	Area bodyArea = gen.areaBox(2760 - 1920 + SCREEN, 529, 30);

	unsigned int yellowC2 = 0xD19D2E00;
	unsigned int greenC2 = 0x678C1800;
	unsigned int redC2 = 0xA53D1C00;

	unsigned int snareInvColor = 0x5D3C2200;
	unsigned int snareDeadColor2 = 0x64412300;
	unsigned int snareDeadColor = 0x533B1E00;
	unsigned int boneColor = 0xB8AFAF00;
	unsigned int birdMeatColor = 0x9A747200;
	int snareLayTime = 3600;

	int failedSnares=0;
	int successfulSnares=0;

	int birdSnaresAllowed = 1;


	void prayBones() {
		for (int i = 0; i < 28; i++) {
			if (inv.VerifyItem(boneColor, i)) {
				inv.MoveToItem(i);
				mouse.LeftClick();
				Sleep(800);
			}
		}
	}

	bool laySnare() {
		int index = inv.SearchInventoryForItem(snareInvColor);
		if (index != -1) {
			inv.MoveToItem(index);
			Sleep(100);
			mouse.LeftClick();
			Sleep(snareLayTime);
			return true;
		}
		return false;
	}

	//given a point representing a matching color, generate the area to try to click in
	Area generateTrapArea(POINT p) {
		return gen.areaBox(p, 5);
	}

	bool fixDeadSnares() {

		/* first, find all snares showing red, that a bird messed up */
		int tol = 3;
		POINT snaresDown = pix.SearchPixelAreaForPoint(redC2, trapArea, tol);
		while (snaresDown.x != -1) {
			POINT p = snaresDown; //trap on screen
			POINT q; //where you'll be when you walk to the trap.
			POINT qq; //where you'll be when you walk to the trap if you found it too high.
			POINT qqq; //where you'll be when you walk to the trap if you found it too low.
			q.x = p.x - 43;
			q.y = p.y - 5;
			qq.x = q.x + 8;
			qq.y = q.y + 28;
			qqq.x = p.x - 45;
			qqq.y = p.y - 50;
			Area a = generateTrapArea(p);
			Area b = generateTrapArea(q);
			Area c = generateTrapArea(qq);
			Area d = generateTrapArea(qqq);
			if (!gen.DefiniteClick(redC2, tol, a, HOVER_ACTION, 0xFFFFFF00, 1, 30)) { //walk here
				snaresDown = pix.SearchPixelAreaForPoint(redC2, trapArea, tol);
				continue;
			}
			Sleep(1200);//walk time
			if (!gen.DefiniteClick(0x0000FF00, 0, b, HOVER_ACTION, HOVER_ACTION, 0, 7))
				if (!gen.DefiniteClick(0x0000FF00, 0, c, HOVER_ACTION, HOVER_ACTION, 0, 9))
					if (!gen.DefiniteClick(0x0000FF00, 0, d, HOVER_ACTION, HOVER_ACTION, 0, 9))
						return false;
			Sleep(snareLayTime);
			if (!laySnare())
				printf("no snares available\n");
			Sleep(1100);
			snaresDown = pix.SearchPixelAreaForPoint(redC2, trapArea, tol);
			failedSnares++;
			printf("Failed snares: %d\n", failedSnares);
		}

		/* Next, find any snares laying on the ground*/
		tol = 2;
		snaresDown = pix.SearchPixelAreaForPoint(snareDeadColor, trapArea, tol);

		while (snaresDown.x != -1) {
			if (!gen.DefiniteClick(snareDeadColor, tol, trapArea, HOVER_ITEM, HOVER_ITEM, 1, 1))
				break;
			Sleep(snareLayTime);
			snaresDown = pix.SearchPixelAreaForPoint(snareDeadColor, trapArea, tol);
			failedSnares++;
		}
		return true;
		
	}

	bool collectGoodSnares() {
		int tol = 2;
		POINT greenSnare = pix.SearchPixelAreaForPoint(greenC2, trapArea, tol);
		while (greenSnare.x != -1) {
			POINT p = greenSnare; //trap on screen
			POINT q; //where you'll be when you walk to the trap.
			POINT qq; //where you'll be when you walk to the trap if you found it too high.
			POINT qqq; //where you'll be when you walk to the trap if you found it too low.
			q.x = p.x - 43;
			q.y = p.y - 5;
			qq.x = q.x + 8;
			qq.y = q.y + 28;
			qqq.x = p.x - 45;
			qqq.y = p.y - 50;
			Area a = generateTrapArea(p);
			Area b = generateTrapArea(q);
			Area c = generateTrapArea(qq);
			Area d = generateTrapArea(qqq);
			if (!gen.DefiniteClick(greenC2, tol, a, HOVER_ACTION, 0xFFFFFF00, 1, 30)) { //walk here
				greenSnare = pix.SearchPixelAreaForPoint(greenC2, trapArea, tol);
				continue;
			}
			Sleep(1200);//walk time
			if (!gen.DefiniteClick(0x0000FF00, 0, b, HOVER_ACTION, HOVER_ACTION, 0, 7))
				if (!gen.DefiniteClick(0x0000FF00, 0, c, HOVER_ACTION, HOVER_ACTION, 0, 9))
					if (!gen.DefiniteClick(0x0000FF00, 0, d, HOVER_ACTION, HOVER_ACTION, 0, 9))
						return false;
			Sleep(snareLayTime);
			if (!laySnare())
				printf("no snares available\n");
			Sleep(1100);
			greenSnare = pix.SearchPixelAreaForPoint(greenC2, trapArea, tol);
			successfulSnares++;
			printf("Successful snares: %d\n", successfulSnares);
			inv.DropItemsColored(birdMeatColor);
			prayBones();
		}
		return true;
	}

public:

	BotHuntingBirdSnare() {
		trapArea.x1 = 2768 - 1920 + SCREEN;
		trapArea.y1 = 310;
		trapArea.x2 = 2967 - 1920 + SCREEN;
		trapArea.y2 = 786;
	}

	void run() {
		gen.NormalizeCompass(UP);
		laySnare();
		while (true) {
			gen.HandleHotkeys();
			if (!collectGoodSnares()) {
				printf("Quitting because you weren't able to pick up the snare you walked into\n");
				return;
			}
			if (!fixDeadSnares()) {
				printf("Quitting because you weren't able to pick up the snare you walked into\n");
				return;
			}
		}
	}





};