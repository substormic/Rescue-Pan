#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Pixel.h"
#include <iostream>


class InterfaceGeneral
{
protected: 
	Mouse mouse;
	PixelHandler pix;
	Keyboard key;

	int logTimeout;
	POINT MousePos;
	bool menu5Option = false;
	MSG msg; //for hotkey message recieving

public:
	InterfaceGeneral(); // constructor (sets hotkeys)

	~InterfaceGeneral(); // destructor (del hotkeys)

	//handles what hotkeys do
	virtual void HandleHotkeys();

	//verifies that hover text contains a specified color
	// !WARNING! hover text only works on items that have more than just a USE first option
	virtual bool VerifyHoverText(unsigned int color);

	//verifies the hover of something in the top-left corner is specified color
	virtual bool VerifyTopLeftText(unsigned int color);

	//checks the 3 static pixels are live
	virtual bool VerifyOSbuddy();

	//given an option choice(1,2,3...etc.), it will return the top left coordinate of that options hitbox
	//takes option choice on Zero Indexing (option 1 = optionChoice:0, option 2 = opChoice:1, etc.)
	virtual POINT GetMenuOptionCoords(int optionChoice);

	//takes option choice on Zero Indexing (option 1 = optionChoice:0, option 2 = opChoice:1, etc.)
	//!!WARNING!!! only handles 4 option items at bottom of the screen!
	void ChooseMenuOption(int optionChoice);

	//Chooses a menu option only if 'HOVER_xxx' exists on desired option
	bool ChooseMenuOptionColorCheck(int optionChoice, unsigned int color);

	//check if leveled up
	bool CheckLevelUp();

	//Does a super safe click. Scans region for color, tries a number of times.
	//only clicks if the color is there and if the hover and ultimately right click matches. 
	bool DefiniteClick(unsigned int itemColor, int tolerance, Area region, unsigned int hoverColor, unsigned int menuColor, int menuOption, int maxAttempts);

	//check if leveled up
	//modes ignore certain skill levels (like hp at mode 0)
	bool CheckLevelUp(int mode);

	//Given three pixels, checks the environment to see if those pixels are where they should be
	bool CheckLocation(Pixel pix1, Pixel pix2, Pixel pix3);

	//when all else fails. gtfo
	void Logout();

	//For when you just need to get out asap. keeps clicking until logged out
	void LogoutQuick();

	//checks for black background in health circle
	//amount should be some 1-100 value. 
	//returns true if health is there, false if not
	bool CheckHealthAmount(int amount);

	//clicks on the compass for normalization of the game view
	// mode: 0 - just clicks compass for northern view.
	// mode: 1 - clicks compass and holds up arrow to move camera up
	void NormalizeCompass(int mode);

	//sets mouse move speed 0.1-1.0 where lower is faster
	void SetMouseSpeed(float speed);

	void SetMouseDeviation(int Deviation);

	//checks if loggedout or in process of logging in.
	//if false, it is logged out
	//if true, it is logged in
	bool CheckIfInGame();

	//after 6 hours it logs you out. this logs you back innnn
	bool HandleAutoLogOut();

	//sends a message to connor or brandon, granted that one
	//of them is specified in password.h as "progname"
	bool pingMessage();

};