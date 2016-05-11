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
bool InterfaceBank::OpenBank(Area region) {
	return InterfaceBank::OpenBank(region, false, 400);
}

bool InterfaceBank::OpenBank(Area region, int timeout) {
	return InterfaceBank::OpenBank(region, false, timeout);
}

//opens bank lol
bool InterfaceBank::OpenBank(Area region, bool onlyNPC, int timeout)
{
	int bankTimeout = 0;
	while (bankTimeout < 40)
	{
		bool canFindBank = pix.SearchPixelArea(0x291B0000, region.x1 + SCREEN, region.y1, region.x2 + SCREEN, region.y2, 50);
		if (canFindBank && bankTimeout < 5) {
			POINT b = pix.SearchPixelAreaForPoint(0x291B0000, region.x1 + SCREEN, region.y1, region.x2 + SCREEN, region.y2, 50);
			mouse.MouseMove(b);
		}
		else {
			mouse.MouseMoveArea(region.x1 + SCREEN, region.y1, region.x2 + SCREEN, region.y2);
		}
		Sleep(120); //careful

		if (VerifyTopLeftText(0x00DDDD00,2019-1920,-1) && !onlyNPC)
		{
			mouse.RightClick();
			Sleep(120); //careful
			if (!ChooseMenuOptionColorCheck(0, HOVER_ACTION)) {
				Sleep(100);
				if (!ChooseMenuOptionColorCheck(0, HOVER_ACTION)) {
					POINT pp = mouse.GetPosition();
					pp.y -= rand() % 20;
					mouse.MouseMove(pp);
					bankTimeout++;
					continue;
				}
			}
			Sleep(30);
			mouse.LeftClick();
			int btime = 0;
			while (!VerifyBankOpen())
			{
				btime++;
				Sleep(50);
				if (btime > timeout) {
					return false;
				}
			}
			bankTimeout = 0;
			return true;
		}
		else if (VerifyTopLeftText(HOVER_NPC))
		{
			mouse.RightClick();
			Sleep(150); //careful
			if (!ChooseMenuOptionColorCheck(1, HOVER_NPC)) {
				Sleep(100);
				if (!ChooseMenuOptionColorCheck(1, HOVER_NPC)) {
					POINT pp = mouse.GetPosition();
					pp.y -= rand() % 20;
					mouse.MouseMove(pp);
					bankTimeout++;
					continue;
				}
			} 
			Sleep(30);
			mouse.LeftClick();
			int btime = 0;
			while (!VerifyBankOpen())
			{
				btime++;
				Sleep(50);
				if (btime > timeout) {
					return false;
				}
			}
			bankTimeout = 0;
			return true;
		}
		bankTimeout++;
	}
	printf("Can't find or open the bank\n");
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



bool InterfaceBank::openEdgevilleBank(int teleIndex) {
	InterfaceInventory inv;
	inv.VerifyActiveInventory();
	inv.MoveToItem(teleIndex);
	Sleep(50);
	mouse.LeftClick();
	Pixel a, b;
	a.Set(0x00000000, 2518 - 1920 + SCREEN, 788);
	b.Set(0x00, 3166 - 1920 + SCREEN, 866);
	int timeout = 0;
	while (!pix.VerifyPixelColor(a) && !pix.VerifyPixelColor(b)) {
		HandleHotkeys();
		Sleep(5);
		timeout++;
		if (timeout > 100) {
			printf("Never got to house\n");
			return false;
		}
	}
	timeout = 0;
	while (pix.VerifyPixelColor(a) && pix.VerifyPixelColor(b)) {
		HandleHotkeys();
		Sleep(5);
		timeout++;
		if (timeout > 100) {
			printf("House never loaded\n");
			return false;
		}
	}
	DefiniteClick(0x7209a000, 2, areaBox(2455 - 1920 + SCREEN, 399, 50), HOVER_ACTION, HOVER_ACTION, 0, 20, 147, -1);
	Sleep(6000);
	ChangeCompassDegrees(180);
	bool r = OpenBank(areaBox(3049 - 1920, 142, 20, 40));
	if (!r) {
		r |= OpenBank(areaBox(2976-1920, 135, 20));
	}
	NormalizeCompass(UP);
	return r;
}