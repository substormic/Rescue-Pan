#pragma once
#include "InterfaceInventory.h"

class BotMagicTeleAlch
{
private:
	InterfaceInventory inv;
	POINT CamTele; // x,y coords in spell book
	POINT HighAlch; // x,y coords in spell book

public:
	BotMagicTeleAlch()
	{
		CamTele.x = 5;
		CamTele.y = 3;
		HighAlch.x = 6;
		HighAlch.y = 4;
	}

	void run()
	{
		while (1)
		{
			if (inv.CheckSpellRunes(MAGIC_TELE, 5, 3))
				printf("Active Tele!\n");
			else
				printf("nuttin\n");
			if (inv.CheckSpellRunes(MAGIC_ALCH, 5, 4))
				printf("Active Alch!\n");
			else
				printf("nuttin\n");
		}
	}

};