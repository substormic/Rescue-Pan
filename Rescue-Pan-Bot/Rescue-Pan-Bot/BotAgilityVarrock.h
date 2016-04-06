/* ========================================================================= */
/*
BotAgilityVarrock.h (no matching .cpp file)
Authored by Brandon Wallace and Connor Rainen.

For running the varrock rooftop agility course with OSBuddy Pro

SKILL: Agility
HOURLY:		Money: N/A		XP: + ??? /hr
Requirements:
- No minimum skill required
- Location:		Begin at the end of the varrock course.
- Bank:			N/A
- Inventory:	None.
- Map:			Map normalized to OSBuddy full zoom out, facing north, UP.

Independence:Intermittent play	(2/5)


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
#include "InterfaceStats.h"

class BotAgilityVarrock {
private:
	InterfaceBank bank;
	InterfaceGeneral gen;
	InterfaceInventory inv;
	InterfaceStats stats;
	PixelHandler pix;
	Mouse mouse;


	unsigned int greenColor = 0x327D2900;
	unsigned int redColor = 0x983C4600;

	int tolerance = 14;

	int areas = 9;
	Area loc[9];

	bool attemptAgility(int obstacleNum, int sleepWithRun, bool colored, bool compassChange, int mapDegrees, bool forceCompass) {
		if (obstacleNum >= areas) {
			printf("Trying to go to an obstacle that doesn't exist!\n");
			return false;
		}
		int timeout = 0;
		if (colored && compassChange && forceCompass) {
			do
			{
				gen.NormalizeCompass(mapDegrees != 90 ? DOWN : UP);
				mouse.MouseMoveArea(SCREEN + 40, 400, SCREEN + 50, 600);
				gen.ChangeCompassDegrees(mapDegrees);
				Sleep(120);
				timeout++;
			} while (!pix.SearchPixelArea(greenColor, loc[obstacleNum], tolerance) && timeout < 50);
		} else if (compassChange) {
			gen.NormalizeCompass(mapDegrees != 90 ? DOWN : UP);
			mouse.MouseMoveArea(SCREEN + 40, 400, SCREEN + 50, 600);
			gen.ChangeCompassDegrees(mapDegrees);
		}

		if (colored) {
			if (mogExpected(obstacleNum)) {
				printf("Mark of grace on the ground!\n");
				//try to get it
				if (!gen.DefiniteClick(redColor, tolerance, loc[obstacleNum], HOVER_ACTION, HOVER_ACTION, 0, 20))
					return false;
			}
			else {
				if (!gen.DefiniteClick(greenColor, tolerance, loc[obstacleNum], HOVER_ACTION, HOVER_ACTION, 0, 20))
					return false;
			}
		}
		else {
			if (!gen.DefiniteClick(0xFF000000, 0, loc[obstacleNum], HOVER_ACTION, HOVER_ACTION, 0, 40))
				return false;
		}
		Sleep(stats.checkRunOn() ? sleepWithRun : sleepWithRun * 3);
		return true;
	}

	bool attemptAgility(int obstacleNum, int sleepWithRun, bool colored, bool compassChange, int mapDegrees) {
		return attemptAgility(obstacleNum, sleepWithRun, colored, compassChange, mapDegrees, false);
	}

	bool mogExpected(int obstacleNum) {
		if (pix.SearchPixelArea(redColor, loc[obstacleNum], tolerance)) {
			return true;
		}
		return false;
	}

	bool attemptMOG() { //note: incredibly broken, but here's a cool concept.
		//this function is NOT implemented, nor is it remotely safe.
		if (pix.SearchPixelArea(0xa1880900, SCREEN + 10, 40, SCREEN + 1429, 740, 3)) {
			printf("Found the Mark of grace\n");
			POINT a;
			a = pix.SearchPixelAreaForPoint(0xA1880900, SCREEN + 10, 40, SCREEN + 1429, 740, 3);
			Area home = gen.areaBox(2737, 540, 7);
			mouse.MouseMove(a);
			mouse.RightClick();
			if (gen.ChooseMenuOptionColorCheck(0, HOVER_ITEM)) {
				mouse.LeftClick();
				mouse.MouseMoveArea(home);
				mouse.RightClick();
				Sleep(5000);
				gen.ChooseMenuOption(0);
				mouse.LeftClick();
				Sleep(5000);
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}

	void handleRunEnergy() {
		if (!stats.checkRunOn()) {
			if (!pix.VerifyPixelColor(0X13131300, 3409 - 1920 + SCREEN, 177, 2)) {
				mouse.MouseMove(3409 - 1920 + SCREEN, 177);
				mouse.LeftClick();
			}
		}
	}

public:

	BotAgilityVarrock() {
		loc[0] = gen.areaBox(2640 - 1920 + SCREEN, 267, 20, 100); //with compass at 0 degrees.
		loc[1] = gen.areaBox(2359 - 1920 + SCREEN, 521, 40, 40); //compass reset to up
		loc[2] = gen.areaBox(2260 - 1920 + SCREEN, 336, 40);
		loc[3] = gen.areaBox(2402 - 1920 + SCREEN, 536, 40);
		loc[4] = gen.areaBox(2993 - 1920 + SCREEN, 907, 20, 80);
		loc[5] = gen.areaBox(2629 - 1920 + SCREEN, 325, 20, 80); //compass set to 180 // no color
		loc[6] = gen.areaBox(2525 - 1920 + SCREEN, 284, 30); // compass set to 180 // no color
		loc[7] = gen.areaBox(2908 - 1920 + SCREEN, 161, 30, 60);// compass reset to up
		loc[8] = gen.areaBox(2892 - 1920 + SCREEN, 243, 15, 50);
	}

	void run() {
		while (gen.VerifyOSbuddy()) {
			gen.HandleHotkeys();
			handleRunEnergy();

			if (!attemptAgility(0, 10000, true, true, 0,true))
				return;
			if (!attemptAgility(1, 10000, true, true, 90))
				return;
			if (!attemptAgility(2, 6000, true, false, 90))
				return;
			if (!attemptAgility(3, 11000, true, false, 90))
				return;
			if (!attemptAgility(4, 4000, true, false, 90))
				return;
			if (!attemptAgility(5, 9000, false, true, 180))
				return;
			if (!attemptAgility(6, 10000, false, false, 180))
				return;
			if (!attemptAgility(7, 5000, true, true, 90))
				return;
			if (!attemptAgility(8, 4000, true, false, 90))
				return;


		}
		printf("Logged out\n");
		return;
	}

};