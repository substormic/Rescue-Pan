/* ========================================================================= */
/*
BotFiremaking.h (no matching .cpp file)
Authored by Connor Rainen and Brandon Wallace.

For automating the firemaking process by burning all logs in inventory.

SKILL: Firemaking
HOURLY:		Money: - (variable)		XP: + (variable)
Requirements:
- No minimum skill required
- Location:		Anywhere near a  bank.
- Bank:			N/A
- Inventory:	Hold tinderbox in first inventory spot.
- Map:			Map normalized to OSBuddy def. zoom, facing north, UP.
- Other:		Set the logtype and logicon variables

Independence: Intermittend Play (2/5)

note: Independence scale:
Active play			(1/5)
Intermittent play	(2/5)
Frequent monitor	(3/5)
Lazy monitor		(4/5)
Perfect Automation	(5/5)
*/
/* ========================================================================= */
#pragma once
#include "InterfaceInventory.h"
#include "InterfaceStats.h"

class BotFiremaking
{
protected:
	InterfaceInventory inv;
	InterfaceStats stat;
	PixelHandler pix;
	Mouse mouse;
	POINT characterCenter;
	Area LogSearch;
	unsigned int LogType;
	unsigned int LogIcon;
	POINT LastItem;


public:
	unsigned int OakLog = 0x73522900;
	unsigned int OakIcon = 0x916e4500;
	unsigned int WillowIcon = 0x554b2700;
	unsigned int WillowLog = 0x3c340f00;
	unsigned int MapleIcon = 0x724c1c00;
	unsigned int MapleLog = 0x51300500;

	BotFiremaking() //defaults
	{
		LogType = OakLog;
		LogIcon = OakIcon;
		characterCenter.x = 811 + SCREEN;
		characterCenter.y = 536;
		LogSearch = inv.areaBox(characterCenter, 50, 35); //log under me checking region
		LastItem = inv.GetItemCoords(27);
		inv.SetMouseSpeed(0.4);
	}

	void initialize()
	{
		inv.NormalizeCompass(UP); //makes sure camera is north facing and camera is birds eye
	}

	//actually checks if a fire is in the process of being set, not if theres a fire
	bool CheckFireUnderMe() //check if logs are currently being lit under me
	{
		return pix.SearchPixelArea(LogType, LogSearch.x1, LogSearch.y1, LogSearch.x2, LogSearch.y2, 5);
	}

	//returns false if no logs to light
	bool SetFire()
	{
		if (inv.MoveToItem(LogType)|| inv.MoveToItem(LogIcon)) //sometimes one color doesnt work randomly
		{
			mouse.LeftClick();
			printf("Logs Found\n");
			Sleep(30);
			inv.MoveToItem(0);
			Sleep(30);
			mouse.LeftClick();
			Sleep(30);
		}
		else
		{
			//mouse.LeftClick();
			printf("Where are the logs at?\n");
			return false;
		}

		Sleep(1500);
		return true;

	}

	void run()
	{
		initialize();
		while (1)
		{
			inv.HandleHotkeys();
			while (CheckFireUnderMe()) //while fire is under, do nothing
			{
				inv.HandleHotkeys();
			}
			printf("NOTHING, starting fire\n");
			SetFire();
			printf("Fire Under Me\n");
		}

	}

	void run(unsigned int Log, unsigned int LogInvIcon)
	{
		LogType = Log;
		LogIcon = LogInvIcon;
		run();

	}

};