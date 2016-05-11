#pragma once
#include "InterfaceStats.h"
#include "InterfaceInventory.h"
#include "InterfaceGeneral.h"

class BotRangeOgre
{
	protected:
		InterfaceStats stat;
		InterfaceInventory inv;
		PixelHandler pix;
		InterfaceGeneral gen;
		Mouse mouse;

		bool purpItemsExist() {
			if (pix.SearchPixelArea(0xFF00FF00, 260 + SCREEN, 250, 1440 + SCREEN, 580)) {
				return true;
			}
			return false;
		}

		void attemptTelegrab() {

			inv.VerifyActiveMagic();
			inv.MoveToSpell(5, 2);
			mouse.LeftClick();
			POINT p = pix.SearchPixelAreaForPoint(0xFF00FF00, 260 + SCREEN, 250, 1440 + SCREEN, 580);
			if (p.x != -1) {
				p.y += 10;
				p.x += 30;
				mouse.MouseMove(p);
				Sleep(100);
				mouse.RightClick();
				Sleep(1200);
				if (gen.ChooseMenuOptionColorCheck(1, 0x00FF0000)) {
					mouse.LeftClick();
				}
				else {
					mouse.MouseMoveArea(gen.areaBox(SCREEN + 20, 20, 5, 5));
				}
			}
			else {
				return;
			}

		}

public:
	void run(int mode)
	{
		mouse.ChangeSpeed(0.4);
		bool combat = true;
		bool failOnce = false;
		unsigned int OgreNew = 0x917b5600;
		unsigned int Ogre = 0xc9b59500;
		bool alreadyBeeped = false;
		stat.alternateColor = Ogre;
		while (combat)
		{
			stat.HandleAutoLogOut();
			stat.HandleHotkeys();

			POINT cursor = mouse.GetPosition();
			combat = stat.Fight(OgreNew, 260 + SCREEN, 250, 1440 + SCREEN, 580,0x00FF0000);
			stat.CheckLevelUp();

			/*if (purpItemsExist()) {
				attemptTelegrab();
			}*/

			if (purpItemsExist()) {
				if (!alreadyBeeped) {
					Beep(1000, 4000);
					alreadyBeeped = true;
				}
			}
			else {
				alreadyBeeped = false;
			}


			if (mode == 1 && stat.MouseMoved == true)
				mouse.MouseMoveArea(cursor.x, cursor.y,cursor.x+3, cursor.y+3);
			if (!combat)
			{
				stat.Logout();
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