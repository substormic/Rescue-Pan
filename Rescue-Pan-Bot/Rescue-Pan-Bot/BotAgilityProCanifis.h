/* ========================================================================= */
/*
BotAgilityVarrock.h (no matching .cpp file)
Authored by Brandon Wallace and Connor Rainen.

For running the varrock rooftop agility course with OSBuddy Pro

SKILL: Agility
HOURLY:		Money: N/A		XP: + ??? /hr
Requirements:
- No minimum skill required
- Location:		Begin at the end of the Canifis course.
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

class BotAgilityProCanifis {
private:

	InterfaceGeneral gen;
	InterfaceInventory inv;
	InterfaceStats stats;
	PixelHandler pix;
	Mouse mouse;


	unsigned int greenColor = 0x327D2900;
	unsigned int redColor = 0x75223800;//varrock: 0x983C4600;

	int tolerance = 14;

	int areas = 11;
	Area loc[11];

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
		}
		else if (compassChange) {
			gen.NormalizeCompass(mapDegrees != 90 ? DOWN : UP);
			mouse.MouseMoveArea(SCREEN + 40, 400, SCREEN + 50, 600);
			gen.ChangeCompassDegrees(mapDegrees);
		}

		if (colored) {
			if (mogExpected(obstacleNum)) {
				printf("Mark of grace on the ground!\n");
				//try to get it
				if (!gen.DefiniteClick(redColor, tolerance, loc[obstacleNum], HOVER_ACTION, 0x5DA3BE00, 0, 3))
					if (!gen.DefiniteClick(redColor, tolerance, loc[obstacleNum], HOVER_ACTION, HOVER_ACTION, 0, 20))
						return false;
			}
			else {
				if (!gen.DefiniteClick(greenColor, tolerance, loc[obstacleNum], HOVER_ACTION, 0x12F8A700, 0, 3))
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

	bool getBackOnCourse() {

		mouse.MouseMove(3532 - 1920 + SCREEN, 168);
		mouse.LeftClick();
		Sleep(8250);

		POINT p = pix.SearchPixelAreaForPoint(0x00FF0000, MiniMap);
		if (p.x != -1) {
			printf("Found the course!\n");
			mouse.MouseMove(p);
			mouse.LeftClick();
			Sleep(4000);
			if (!attemptAgility(8, 5000, true, false, 90)) {
				printf("Nah, we didn't\n");
				return false;
			}
			else {
				printf("looks like it worked!\n");
				if (!attemptAgility(1, 5000, true, false, 90))
					return false;
				if (!attemptAgility(2, 4500, true, false, 90))
					return false;
				if (!attemptAgility(3, 6500, true, false, 90))
					return false;
				return true;
			}
		}
		else {
			printf("Couldn't find the course :/ \n");
			return false;
		}
	}

public:

	BotAgilityProCanifis() {
		loc[0] = gen.areaBox(2615 - 1920 + SCREEN, 334, 50); 
		loc[1] = gen.areaBox(2716 - 1920 + SCREEN, 292, 40, 40);
		loc[2] = gen.areaBox(2435 - 1920 + SCREEN, 538, 40);
		loc[3] = gen.areaBox(2320 - 1920 + SCREEN, 801, 60, 40);
		loc[4] = gen.areaBox(2663 - 1920 + SCREEN, 970, 40);
		loc[5] = gen.areaBox(2856 - 1920 + SCREEN, 711, 40);
		loc[6] = gen.areaBox(3530 - 1920 + SCREEN, 527, 50, 30); 
		loc[7] = gen.areaBox(2726 - 1920 + SCREEN, 252, 30, 60);
		loc[8] = gen.areaBox(2820 - 1920 + SCREEN, 499, 70); //this is a makeup!
		loc[9] = gen.areaBox(2834 - 1920 + SCREEN, 83, 30, 60); //also a makeup
		loc[10] = gen.areaBox(2724 - 1920 + SCREEN, 207, 70, 50); //makeup
	}

	void run() {
		int laps = 0;
		bool skipOne = false;

		while (gen.VerifyOSbuddy()) {
			gen.HandleHotkeys();
			handleRunEnergy();
			inv.AttemptToEatAtHp(0xDDD9D900, 50);

			if (!skipOne)
				if (!attemptAgility(0, 6500, true, false, 90))
					return;
			if (!attemptAgility(1, 5000, true, false, 90)) {
				printf("weird glitch thing\n");
				if (!attemptAgility(10, 5500, true, false, 90))
					return;
			}
			if (!attemptAgility(2, 4500, true, false, 90))
				return;
			if (!attemptAgility(3, 6500, true, false, 90))
				return;
			while (!attemptAgility(4, 4000, true, false, 90)) {
				if (!getBackOnCourse()) {
					printf("Something went wrong, leaving\n");
					return;
				}
				else {
					printf("got back on track!\n");
				}
			}
			if (!attemptAgility(5, 8500, true, false, 90))
				return;
			if (!attemptAgility(6, 8000, true, false, 90)) {
				printf("same weird glitch\n");
				mouse.MouseMove(2960 - 1920 + SCREEN, 564);
				mouse.LeftClick();
				Sleep(1000);
				if (!attemptAgility(6, 8000, true, false, 90)) {
					return;
				}
			}
				
			if (!attemptAgility(7, 6000, true, false, 90)) {
				printf("You're trying to get back on course\n");
				if (!attemptAgility(9, 8500, true, false, 90)) {
					return;
				}
				else {
					printf("You were successful!\n");
					skipOne = true;
				}
			}
			else {
				skipOne = false;
			}
				

			printf("You've done %d laps!\n", ++laps);
		}
		printf("Logged out\n");
		return;
	}

};