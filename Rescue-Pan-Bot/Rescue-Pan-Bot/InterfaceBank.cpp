#include "InterfaceBank.h"

InterfaceBank::InterfaceBank()
{
	ActiveTab._color = 0x483e3300;
	ActiveTab._x = 2;
	ActiveTab._y = 2; //pixels from top left where color should be 

	BankTab.x = 500 + SCREEN;
	BankTab.y = 92;

	BankItem.x = 510 + SCREEN;
	BankItem.y = 130;

	BankClose.x = 912 + SCREEN;
	BankClose.y = 59;
}

//Check bank open
bool InterfaceBank::VerifyBankOpen()
{
	bool result = true;
	result &= pix.SearchPixelArea(bankText, 650 + SCREEN, 60, 770 + SCREEN, 80, 10); // orange text in header (tab, bank of runescape, etc)
	result &= pix.SearchPixelArea(bankText, 460 + SCREEN, 60, 480 + SCREEN, 80, 10); // orange text on bank spaces in top left of bank
	return result;
}

//returns Area or coords that contains tab, ZERO indexed
Area InterfaceBank::GetBankTabCoords(int index)
{
	Area coord;
	coord.x1 = BankTab.x + ((BANK_TAB_WIDTH + BANK_TAB_SPACER)*index);
	coord.x2 = BankTab.x + ((BANK_TAB_WIDTH + BANK_TAB_SPACER)*index) + BANK_TAB_WIDTH;
	coord.y1 = BankTab.y;
	coord.y2 = BankTab.y + BANK_TAB_HEIGHT;
	return coord;
}

//moves mouse to bank tab
void InterfaceBank::MoveToTab(int index)
{
	Area coords = GetBankTabCoords(index);
	mouse.MouseMoveArea(coords.x1, coords.y1, coords.x2, coords.y2);
	return;
}

//moves mouse to bank tab
bool InterfaceBank::OpenTab(int index)
{
	if (!VerifyBankTabOpen(index))
	{
		MoveToTab(index);
		Sleep(20);
		mouse.LeftClick();
		return VerifyBankTabOpen(index);
	}
	return true;
}

//Checks whether a certain tab is open
bool InterfaceBank::VerifyBankTabOpen(int index)
{
	Area coords = GetBankTabCoords(index);
	return pix.VerifyPixelColor(ActiveTab._color, coords.x1 + ActiveTab._x, coords.y1 + ActiveTab._y);
}

//give x,y coords of the item in bank, 0 indexed, returns coordinates of said items
Area InterfaceBank::GetItemCoords(int indexX, int indexY)
{
	Area coord;
	coord.x1 = BankItem.x + ((BANK_ITEM_WIDTH + BANK_ITEM_SPACER_WIDTH)*indexX);
	coord.x2 = BankItem.x + ((BANK_ITEM_WIDTH + BANK_ITEM_SPACER_WIDTH)*indexX) + BANK_ITEM_WIDTH;
	coord.y1 = BankItem.y + ((BANK_ITEM_HEIGHT + BANK_ITEM_SPACER_HEIGHT)*indexY);
	coord.y2 = BankItem.y + ((BANK_ITEM_HEIGHT + BANK_ITEM_SPACER_HEIGHT)*indexY) + BANK_ITEM_HEIGHT;
	return coord;
}


//given a color and item position, check if item is there
bool InterfaceBank::VerifyItem(unsigned int color, int indexX, int indexY)
{
	Area Region = GetItemCoords(indexX, indexY);
	return pix.SearchPixelArea(color, Region.x1, Region.y1, Region.x2, Region.y2, 5);
}

//moves mouse to tab
void InterfaceBank::MoveToItem(int indexX, int indexY)
{
	Area coords = GetItemCoords(indexX, indexY);
	mouse.MouseMoveArea(coords.x1, coords.y1, coords.x2, coords.y2);
	return;
}

//withdraw an item given by menu option, USE THE WITHDRAW_XXX definitions!!!!
bool InterfaceBank::Withdraw(int indexX, int indexY, int menuOption)
{
	MoveToItem(indexX, indexY);
	Sleep(50);
	mouse.RightClick();
	ChooseMenuOption(menuOption);
	mouse.LeftClick();
	return true;
}

//withdraw an item given by menu option, USE THE WITHDRAW_XXX definitions!!!!
bool InterfaceBank::Withdraw(int indexX, int indexY, int menuOption, int Xamount)
{
	MoveToItem(indexX, indexY);
	Sleep(50);
	mouse.RightClick();
	ChooseMenuOption(menuOption);
	mouse.LeftClick();

	const int len = 20;
	char amount[len];
	//sprintf(amount, "%d", Xamount);
	key.Type(amount, len);
	return true;
}

void InterfaceBank::DepositBackpack() {
	mouse.MouseMove(SCREEN + 2801 - 1920, 827);
	mouse.LeftClick();
	Sleep(50);
}


//opens bank lol
bool InterfaceBank::OpenBank(Area region)
{
	while (bankTimeout < 20)
	{
		bool canFindBank = pix.SearchPixelArea(0x291B0000, region.x1 + SCREEN, region.y1, region.x2 + SCREEN, region.y2, 50);
		if (canFindBank && bankTimeout < 5) {
			POINT b = pix.SearchPixelAreaForPoint(0x291B0000, region.x1 + SCREEN, region.y1, region.x2 + SCREEN, region.y2, 50);
			mouse.MouseMove(b);
			printf("Found bank and moved mouse\n");
		}
		else {
			printf("Didn't find the bank... moving to that area\n");
			mouse.MouseMoveArea(region.x1 + SCREEN, region.y1, region.x2 + SCREEN, region.y2);
		}
		Sleep(200);

		if (VerifyTopLeftText(0x00DDDD00))
		{
			mouse.RightClick();
			Sleep(150);
			ChooseMenuOptionColorCheck(0, HOVER_ACTION); //todo: if this returns false, return.
			Sleep(30);
			mouse.LeftClick();
			printf("Waiting on bank to open\n");
			while (!VerifyBankOpen())
			{
				; //todo: throw error if timeout2 exceeds.
			}
			bankTimeout = 0;
			return true;
		}
		else if (VerifyTopLeftText(HOVER_NPC))
		{
			mouse.RightClick();
			Sleep(250);
			ChooseMenuOption(1);
			Sleep(30);
			mouse.LeftClick();
			printf("Waiting on bank to open\n");
			while (!VerifyBankOpen())
			{
				;
			}
			bankTimeout = 0;
			return true;
		}
		bankTimeout++;
		printf("Error opening bank, trying again: %i\n", bankTimeout);
	}
	return false;
}

//close bank
bool InterfaceBank::CloseBank()
{
	if (VerifyBankOpen())
	{
		mouse.MouseMoveArea(BankClose.x, BankClose.y, BankClose.x + BANK_CLOSE_WIDTH, BankClose.y + BANK_CLOSE_HEIGHT);
		Sleep(20);
		mouse.LeftClick();
		return true;
	}
	return false;
}