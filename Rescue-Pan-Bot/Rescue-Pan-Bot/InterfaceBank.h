#pragma once
#include "InterfaceGeneral.h"
#include "InterfaceInventory.h"
#include <sstream> //for to_string
#include <string.h> // for strings

class InterfaceBank : public InterfaceGeneral
{
protected:
	const unsigned int bankText = 0xff981f00;
	Pixel ActiveTab;
	POINT BankTab; //The starting top left coordinate of the tabs
	POINT BankItem; //The starting coordinates of top left item in bank
	POINT BankClose; //Starting coords of the close bank option
	int bankTimeout;

public:
	InterfaceBank(); //constructor

	//Check bank open
	bool VerifyBankOpen();

	//returns Area or coords that contains tab, ZERO indexed
	Area GetBankTabCoords(int index);

	//moves mouse to tab
	void MoveToTab(int index);

	//opens a tab, returns true if successful
	bool OpenTab(int index);

	//Checks whether a certain tab is open
	bool VerifyBankTabOpen(int index);

	//give x,y coords of the item in bank, 0 indexed, returns coordinates of said items
	Area GetItemCoords(int indexX, int indexY);

	//given a color and item position, check if item is there
	bool VerifyItem(unsigned int color, int indexX, int indexY);

	//moves mouse to tab
	void MoveToItem(int indexX, int indexY);

	//withdraw an item given by menu option
	bool Withdraw(int indexX, int indexY, int menuOption);

	//withdraw an item given by menu option, gives an X amount to withdraw
	bool Withdraw(int indexX, int indexY, int menuOption, int Xamount);

	//empties backpack into current tab of bank.
	void DepositBackpack();

	//opens bank lol
	bool InterfaceBank::OpenBank(Area region, int timeout);

	//opens bank lol
	bool InterfaceBank::OpenBank(Area region);

	//opens bank lol
	bool InterfaceBank::OpenBank(Area region, bool onlyNPC, int timeout);

	//Closes bank
	bool CloseBank();

	//opens edgeville bank by tele-ing to house.
	bool openEdgevilleBank(int teleIndex);

	//RL section
	//uses findBank to find and open the bank. waits for it to be open, dumps inv. 
	bool dumpInventoryRL(Pregion findBank);

	void waitForBankRL();

	void closeBankRL();

/*
	//checks if bank is open to a specific tab. If not, opens thab.
	void VerifyTabOpen(int tabNumber);

	//check if inventory is empty
	bool VerifyInventoryEmpty();

	//Withdraws x items, typing in the exact amount to set the "x" in game

	void setWithdrawX(int bankPosition, int x);

	void withdraw1(int bankPosition);

	void withdraw5(int bankPosition);

	void withdraw10(int bankPosition);

	void withdrawX(int bankPosition);

	void withdrawABO(int bankPosition);
	//todo: add overloads that take a color for safe checking


	//Deposits X items, setting the exact amount 
	void setDepositX(int invPosition, int x);

	void deposit1(int invPosition);

	void deposit5(int invPosition);

	void deposit10(int invPosition);

	void depositX(int invPosition);
	//todo: add overloads to take a color for safe checking.

	*/
};