#include <iostream>
#include "InterfaceInventory.h"
#include "BotThievingPickpocket.h"
#include "BotThievingRogueChest.h"
#include "BotMagicCurse.h"
#include "BotMagicSuperheat.h"
#include "BotRangeOgre.h"
#include "BotConstructionButlerDungeonDoor.h"
#include "BotAgilityCanifisRL.h"
#include "BotMeleeCow.h"
#include "BotPK.h"
#include "BotAgilityCanifis.h"
#include "BotAgilitySeersRL.h"
#include "BotWoodcuttingDraynorRL.h"
#include "InterfaceStats.h"
#include "BotWoodcuttingDraynor.h"
#include "BotMiningDropAdro.h"
#include <stdio.h>

int main()
{
	/*
	//arguments Window, HK-ID, SPECIAL-KEYS, HOTKEY
	RegisterHotKey(NULL, 1, 0, ESCAPE); //Pause the program, (needs enter to resume)
	RegisterHotKey(NULL, 2, MOD_SHIFT, ESCAPE);

	MSG msg;
	GetMessage(&msg, NULL, 0, 0);

	//GetMessage(&msg, NULL, 0, 0);
	if (msg.message == WM_HOTKEY)
	{
		std::cout << msg.lParam << " - " << msg.wParam << std::endl;
		GetMessage(&msg, NULL, 0, 0);
	} */

	unsigned int ThievingEXP = 0x00000100;



	InterfaceInventory inv;
	PixelHandler pix;
	Keyboard key;
	Mouse mouse;
	//pix.DEBUG();
	InterfaceStats stat;

	/*while (1)
	{
		if (stat.VerifyCombat() == 1)
			printf("In Combat \n");
		else if (stat.VerifyCombat() == 2)
			printf("Target Taken \n");
		else if (stat.VerifyCombat() == 3)
			printf("Freshly Dead \n");
		else
			printf("No Combat\n");
	}*/

	//BotMagicCurse me;
	//BotMagicSuperheat me;
	//BotThievingPickpocket me;
	//BotThievingRogueChest me;
	//BotMeleeCow me;
	// 
	//BotSeersRL me;
	BotWCDraynorRL me;
	//BotCanifisRL me;
	//BotConstructionDungeonDoor me;
	//BotPK me;
	//BotAgilityCanifis me;
	//BotWoodcuttingDraynor me;
	//BotMiningDropAdro me;



	//me.run(me.Paladin1, me.Paladin1, me.cake);
	//me.run(me.AdroKnight1,me.AdroKnight2,me.PineapplePizza);
	me.run();
	//me.run();
	//me.runIron();
	//me.run(0);

	std::cin.ignore();
	return 0;
}

