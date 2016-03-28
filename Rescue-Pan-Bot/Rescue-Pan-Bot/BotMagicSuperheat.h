#pragma once
#include "InterfaceInventory.h"
#include "InterfaceBank.h"

// START THIS BOT with nature rune in 1st
// Start with fire staff equipped
// start with camera north centered and zoom all the way in
class BotMagicSuperheat
{
private:
	InterfaceInventory inv;
	InterfaceBank bank;
	Mouse mouse;
	PixelHandler pix;
	POINT Superheat; // x,y coords in spell book
	unsigned int ironOre = 0x341b1200;
	unsigned int coal = 0x23221500;

public:
	BotMagicSuperheat()
	{
		Superheat.x = 4;
		Superheat.y = 3;
	}

	void initialize()
	{
		inv.SetMouseSpeed(0.5);
		inv.NormalizeCompass(1); //initialize
		inv.SetMouseDeviation(30);
		bank.SetMouseDeviation(40);

	}

	void run()
	{
		//initialize();
		while (1) //each loop starts with a full bar of superheated steel bars
		{

			inv.HandleHotkeys();
			Deposit();
			if (!GetItems())
				break;
			if (!DoSpell())
				break;
		}
		inv.Logout();
	}


	void runIron()
	{
		//initialize();
		while (1) //each loop starts with a full bar of superheated steel bars
		{
			inv.HandleHotkeys();
			Deposit();
			if (!GetIron())
				break;
			if (!DoSpellIron())
				break;
		}
		inv.Logout();
	}


	bool DoSpellIron()
	{
		printf("Casting Superheat on inventory!\n");
		POINT spell = inv.GetSpellCoords(SUPERHEAT._x, SUPERHEAT._y);
		bool result = false;
		Sleep(30);
		for (int i = 1; i <= 27; i++) //inv only holds nine iron, and 18 coal
		{
			if (inv.PrepareSpell(SUPERHEAT)) //if spell is castable
			{
				inv.MoveToItem(i);
				Sleep(70);
				if (!inv.VerifyItem(ironOre, i))
				{
					printf("============ Iron not found to cast on! ============ \n");
					return false; //item not found
				}
				Sleep(30);
				mouse.LeftClick();
				printf("Waiting for spellbook for next cast: %i\n", i);
				while (!inv.VerifyMagicOpen()) //wait for magic book to return
				{
					inv.HandleHotkeys();
					if (inv.HandleAutoLogOut())
						inv.VerifyActiveMagic();
				} //null op for waiting
				Sleep(150);
				result = true; //if at least one time it was successful
			}
			else
			{
				printf("============ Could not prepare the spell! ============ \n");
				return false; //couldnt find any spell
			}
		}
		return result;
	}

	bool DoSpell()
	{
		printf("Casting Superheat on inventory!\n");
		POINT spell = inv.GetSpellCoords(SUPERHEAT._x, SUPERHEAT._y);
		bool result = false;
		Sleep(30);
		for (int i = 1; i <= 9; i++) //inv only holds nine iron, and 18 coal
		{
			if (inv.PrepareSpell(SUPERHEAT)) //if spell is castable
			{
				inv.MoveToItem(i);
				Sleep(70);
				if (!inv.VerifyItem(ironOre, i))
				{
					printf("============ Iron not found to cast on! ============ \n");
					return false; //item not found
				}
				Sleep(30);
				mouse.LeftClick();
				printf("Waiting for spellbook for next cast: %i\n", i);
				while (!pix.SearchPixelArea(SUPERHEAT._color, spell.x, spell.y, spell.x + INV_MAGIC_SPELL_WIDTH, spell.y + INV_MAGIC_SPELL_HEIGHT, 10)) //wait for magic book to return
				{
					inv.HandleHotkeys();
				} //null op for waiting
				Sleep(150);
				result = true; //if at least one time it was successful
			}
			else
			{
				printf("============ Could not prepare the spell! ============ \n");
				return false; //couldnt find any spell
			}
		}
		return result;
	}

	bool GetIron()
	{
		if (bank.VerifyBankOpen())
		{
			bank.OpenTab(7);
			Sleep(60);
			if (bank.VerifyItem(ironOre, 0, 0))
				bank.Withdraw(0, 0, WITHDRAW_ALL);
			else
			{
				printf("======= Ran out of Iron ========\n");
				return false; //ran out of material
			}
			Sleep(30);
			bank.CloseBank();
			Sleep(80);
			return true;
		}
		return false;
	}

	bool GetItems()
	{
		if (bank.VerifyBankOpen())
		{
			bank.OpenTab(7);
			Sleep(60);
			if (bank.VerifyItem(ironOre, 0, 0))
				bank.Withdraw(0, 0, WITHDRAW_LASTX);
			else
			{
				printf("======= Ran out of Iron ========\n");
				return false; //ran out of material
			}
			Sleep(100);
			if (bank.VerifyItem(coal, 1, 0))
				bank.Withdraw(1, 0, WITHDRAW_ALL);
			else
			{
				printf("======= Ran out of coal ========\n");
				return false; //ran out of material
			}
			bank.CloseBank();
			Sleep(80);
			return true;
		}
		return false;
	}

	bool OpenBank()
	{
		Area region;
		region.x1 = 700 + SCREEN;
		region.y1 = 400;
		region.x2 = 950+SCREEN;
		region.y2 = 600;
		POINT bankCoord = pix.SearchPixelAreaForPoint(0x46300300, region.x1 + SCREEN, region.y1, region.x2, region.y2 + SCREEN, 8);
		if (bankCoord.x != -1)
		{
			region.x1 = bankCoord.x - 5;
			region.y1 = bankCoord.y - 5;
			region.x2 = bankCoord.x + 5;
			region.y2 = bankCoord.y + 5;
		}
		else
		{
			printf("Couldnt find the color\n");
		}
		return bank.OpenBank(region);
	}

	bool Deposit()
	{
		printf("Depositing Steel bars\n");
		OpenBank();
		Sleep(300);
		if (bank.VerifyBankOpen())
		{
			inv.MoveToItem(1);
			mouse.RightClick();
			Sleep(30);
			inv.ChooseMenuOption(WITHDRAW_ALL);
			mouse.LeftClick();
			Sleep(50);
			return true;
		}
		return false;
	}

};