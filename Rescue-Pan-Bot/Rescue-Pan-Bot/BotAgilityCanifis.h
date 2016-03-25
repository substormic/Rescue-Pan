#pragma once
#include "InterfaceStats.h"
//Start this bot on the first tree in the game
// begin with camera northed, and red mark zoom at top down view


class BotAgilityCanifis
{
private:
	int stage;
	InterfaceStats stat;
	Pixel TreeStart1, TreeStart2, TreeStart3; //the pixel checks to make sure im in the write place for finding the tree
	
public:
	BotAgilityCanifis()
	{
		TreeStart1.Set(0xaeaea200, 1245 + SCREEN, 671); //top open book in bank
		TreeStart2.Set(0xb3b2a700, 1282 + SCREEN, 1002); //bottom open book in bank
		TreeStart3.Set(0x4a533100, 742 + SCREEN, 331); //tree thats next obstacle
	}

	void run()
	{
		stage = 0;
		while (1)
		{
			switch (stage)
			{
			case(0) : printf("Stage 0\n");
				if (stat.CheckLocation(TreeStart1, TreeStart2, TreeStart3))
					printf("success\n");
				break;
			case(1) : printf("Stage 1\n");
					break;
			}
			std::cin.ignore();
		}
	}

};