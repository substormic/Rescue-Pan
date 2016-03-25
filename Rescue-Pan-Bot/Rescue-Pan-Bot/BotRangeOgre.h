#pragma once
#include "InterfaceStats.h"
#include "InterfaceInventory.h"

class BotRangeOgre
{
	protected:
		InterfaceStats stat;
		InterfaceInventory inv;
		Mouse mouse;

public:
	void run()
	{
		mouse.ChangeSpeed(0.4);
		bool combat = true;
		unsigned int OgreNew = 0x917b5600;
		while (combat)
		{
			stat.HandleAutoLogOut();
			stat.HandleHotkeys();

			POINT cursor = mouse.GetPosition();
			combat = stat.Fight(OgreNew, 260 + SCREEN, 250, 1440 + SCREEN, 460);
			stat.CheckLevelUp();
			if (stat.MouseMoved == true)
				mouse.MouseMoveArea(cursor.x, cursor.y,cursor.x+3, cursor.y+3);
			if (!combat)
			{
				stat.Logout();
				std::cin.ignore();
			}


		}
		return;
	}


};



/* //working range bot
#include <iostream>
#include "InterfaceInventory.h"
#include "BotRangeOgre.h"
#include "InterfaceStats.h"
#include <stdio.h>

int main()
{
PixelHandler pix;
Mouse mouse;
InterfaceInventory inv;
InterfaceStats stat;
bool combat = true;
while (combat)
{
pix.DEBUG();
//BotRangeOgre me;
//me.run();
//inv.ChooseMenuOption(1);
unsigned int OgreOld = 0xcbb89a00;
unsigned int OgreNew = 0x917b5600;
combat = stat.Fight(OgreNew, 260 + SCREEN, 250, 1440 + SCREEN, 460);

stat.CheckLevelUp();

if (!combat)
{
stat.Logout();
std::cin.ignore();
}
//else
//	printf("nope\n");
//inv.VerifyActiveInv();
//pix.SearchPixelArea(0xffdf6f00, 1553, 25, 1553, 25,10);
//std::cin.ignore();
}
return 0;
}

*/