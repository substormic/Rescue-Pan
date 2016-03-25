#pragma once
#include "InterfaceInventory.h"

//RUN VISUAL STUDIO AS ADMIN if you want to block input when doing spells

class BotPK
{
private:
	unsigned int shark = 0x9d795d00;
	POINT Curs;
	MSG msg;

public:
	InterfaceInventory inv;
	Mouse mouse;
	PixelHandler pix;

	BotPK()
	{
		printf("I am ready to assist you in Player Killing\nEsc - Logout\nF1 - Entangle\nF2 - Teleblock\nF3 - Empty\nF4 - Empty\nF5 - Eat Shark\n");

		mouse.SetDeviation(50);
		mouse.ChangeSpeed(0.3);
		inv.SetMouseSpeed(0.3);

		//arguments Window, HK-ID, SPECIAL-KEYS, HOTKEY
		RegisterHotKey(NULL, 1, 0, ESCAPE); // Quickly logs out
		RegisterHotKey(NULL, 2, MOD_SHIFT, ESCAPE); // pauses the program now
		RegisterHotKey(NULL, 3, 0, VK_F1); // F1 - Entangle
		RegisterHotKey(NULL, 4, 0, VK_F2); // F2 - Teleblock
		RegisterHotKey(NULL, 5, 0, VK_F3); // EMPTY
		RegisterHotKey(NULL, 6, 0, VK_F4); // EMPTY
		RegisterHotKey(NULL, 7, 0, VK_F5); // F5 - Eat a Shark


	}

	~BotPK()
	{
		UnregisterHotKey(NULL, 1);
		UnregisterHotKey(NULL, 2);
		UnregisterHotKey(NULL, 3);
		UnregisterHotKey(NULL, 4);
		UnregisterHotKey(NULL, 5);
		UnregisterHotKey(NULL, 6);
		UnregisterHotKey(NULL, 7);
	}

	void run()
	{
		while (1)
		{
			HandleMyHotkeys();
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
				printf("================ ESCAPE TIME - Logging out! ==================\n");
				inv.SetMouseSpeed(0.3);
				inv.LogoutQuick();
				printf("Logout Successful\n");
			}

			if (msg.wParam == 2) //2nd hotkey
			{
				printf("================ Program Paused ==================\n");
				std::cin.ignore(); //pause until enter is pressed
			}

			if (msg.wParam == 3) //3st hotkey
			{
				printf("================ F1 - Entangle Setup!  ==================\n");
				BlockInput(true);
				POINT prevPos = mouse.GetPosition();
				if (!inv.PrepareSpell(ENTANGLE))
					printf("ERROR - No runes for spell\n");
				mouse.MouseMoveArea(prevPos.x - 2, prevPos.y - 2, prevPos.x + 2, prevPos.y + 2);
				BlockInput(false);
			}

			if (msg.wParam == 4) //4st hotkey
			{
				printf("================ F2 - TeleBlock Setup  ==================\n");
				BlockInput(true);
				POINT prevPos = mouse.GetPosition();
				if (!inv.PrepareSpell(TELEBLOCK))
					printf("ERROR - No runes for spell\n");
				mouse.MouseMoveArea(prevPos.x - 2, prevPos.y - 2, prevPos.x + 2, prevPos.y + 2);
				BlockInput(false);
			}

			if (msg.wParam == 5) //5st hotkey
			{
				printf("================ F3 - EMPTY ==================\n");
			}

			if (msg.wParam == 6) //6st hotkey
			{
				printf("================ F4 - EMPTY ==================\n");
			}


			if (msg.wParam == 7) //6st hotkey
			{
				printf("================ F5 - Eating a Shark! ==================\n");
				BlockInput(true);
				inv.AttemptToEat(shark);
				BlockInput(false);
			}

			msg.message = 0; //reset the message so that it resume;
			return;
		}
	}

};