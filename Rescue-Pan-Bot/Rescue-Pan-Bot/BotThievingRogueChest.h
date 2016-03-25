#pragma once
#include "InterfaceInventory.h"

class BotThievingRogueChest
{
private:
	int SleepTimer;
	int lootTimer;
	bool autoLoot;
	bool mouseSet;
	POINT Curs;
	MSG msg;

public: 
	InterfaceInventory inv;
	Mouse mouse;
	PixelHandler pix;

	BotThievingRogueChest()
	{
		printf("No Mouse position for chest set yet\nF1 - Logout\nF2 - Set Mouse\nF3 - Loot Once\nF4 - Auto Loot\n");

		SleepTimer = 0;
		lootTimer = 10;
		autoLoot = false; //initialize
		mouseSet = false;

		mouse.SetDeviation(50);
		mouse.ChangeSpeed(0.4);
		inv.SetMouseSpeed(0.4);

		 //arguments Window, HK-ID, SPECIAL-KEYS, HOTKEY
		RegisterHotKey(NULL, 1, 0, ESCAPE); //Pause the program, (needs enter to resume)
		RegisterHotKey(NULL, 2, MOD_SHIFT, ESCAPE);
		RegisterHotKey(NULL, 3, 0, VK_F1);
		RegisterHotKey(NULL, 4, 0, VK_F2);
		RegisterHotKey(NULL, 5, 0, VK_F3);
		RegisterHotKey(NULL, 6, 0, VK_F4);

	}

	~BotThievingRogueChest()
	{
		UnregisterHotKey(NULL, 1);
		UnregisterHotKey(NULL, 2);
		UnregisterHotKey(NULL, 3);
		UnregisterHotKey(NULL, 4);
		UnregisterHotKey(NULL, 5);
		UnregisterHotKey(NULL, 6);
	}

	void run()
	{
		while (1)
		{
			HandleMyHotkeys();
			if (autoLoot && SleepTimer <= 0) //sleep has gone by
			{
				LootChest();
				SleepTimer = 18000;
			}
			Sleep(50);
			SleepTimer -= 50;
		}

	}

	void LootChest()
	{
		POINT menu;
		//move mouse to cursor appointed area
		mouse.MouseMoveArea(Curs.x - 2, Curs.y - 3, Curs.x + 3, Curs.y + 2);
		Sleep(30);
		mouse.RightClick();
		Sleep(30);
		menu = inv.GetMenuOptionCoords(1);
		inv.ChooseMenuOption(1);
		Sleep(30);
		if (pix.SearchPixelArea(HOVER_ACTION, menu.x, menu.y, menu.x + 3 * MENU_MINWIDTH, menu.y + MENU_OPTION))
		{
			mouse.LeftClick();
			lootTimer = 10;
		}
		else
		{
			if (lootTimer > 0)
			{
				POINT Cursor = mouse.GetPosition();
				mouse.MouseMove(Cursor.x - 300, Cursor.y - 300);
				lootTimer--;
				LootChest();
			}
			return;
		}
	}

	//handles what hotkeys do
	void HandleMyHotkeys()
	{
		PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (msg.message == WM_HOTKEY)
		{
			if (msg.wParam == 1) //1st hotkey
			{
				autoLoot = false;
				SleepTimer = 0;
				printf("================ Program Paused - Press ENTER to continue ==================\n");
				std::cin.ignore(); //pause until enter is pressed
			}

			if (msg.wParam == 2) //2nd hotkey
			{
				printf("================ Program EXITING ==================\n");
				exit(0); //exit porgram
			}

			if (msg.wParam == 3) //3st hotkey
			{
				autoLoot = false;
				SleepTimer = 0;
				printf("================ F1 - LOGOUT QUICK!  ==================\n");
				inv.SetMouseSpeed(0.3);
				inv.LogoutQuick();
				printf("Logout Successful\n");
			}

			if (msg.wParam == 4) //4st hotkey
			{
				//autoLoot = false;
				//SleepTimer = 0;
				Curs = mouse.GetPosition();
				printf("================ F2 - Setting Mouse Coordinates: (%4i,%4i)  ==================\n", Curs.x, Curs.y);
				mouseSet = true;
			}

			if (msg.wParam == 5) //5st hotkey
			{
				autoLoot = false;
				SleepTimer = 0;
				printf("================ F3 - Loot Once ==================\n");
				LootChest();
			}

			if (msg.wParam == 6) //6st hotkey
			{
				autoLoot = true;
				SleepTimer = 0;
				printf("================ F4 - Auto Looting ==================\n");
			}

			msg.message = 0; //reset the message so that it resume;
			return;
		}
	}

};