#include "InterfaceGeneral.h"



InterfaceGeneral::InterfaceGeneral() // constructor (sets hotkeys)
{
	//arguments Window, HK-ID, SPECIAL-KEYS, HOTKEY
	//hotkeys can be found on the following link
	// https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
	RegisterHotKey(NULL, 1, 0, ESCAPE); //Pause the program, (needs enter to resume)
	RegisterHotKey(NULL, 2, MOD_SHIFT, ESCAPE);

	logTimeout = 4;

	//PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
	//GetMessage(&msg, 0, 0, 0); //set to receive

}


InterfaceGeneral::~InterfaceGeneral() // Destructor (delets hotkeys on program exit)
{
	UnregisterHotKey(NULL, 1);
	UnregisterHotKey(NULL,2);
	//HandleHotkeys();
}

//handles what hotkeys do
void InterfaceGeneral::HandleHotkeys()
{
	PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	if (msg.message == WM_HOTKEY)
	{
		if (msg.wParam == 1) //1st hotkey
		{
			printf("================ Program Paused - Press ENTER to continue ==================\n");
			std::cin.ignore(); //pause until enter is pressed
		}

		if (msg.wParam == 2) //2nd hotkey
		{
			printf("================ Program EXITING ==================\n");
			exit(0); //exit porgram
		}

		msg.message = 0; //reset the message so that it resume;
		return;
	}
}


//verifies that hover text contains a specified color
bool InterfaceGeneral::VerifyHoverText(unsigned int color) 
{
	int hoverTLX, hoverTLY, hoverBRX, hoverBRY;
	POINT cursor;
	cursor = mouse.GetPosition();

	//hover box starts [Top Left] (18,3) pixels from the cursor
	//hover box ends [Bottom Right] (63,18) pixels from its start
	hoverTLX = cursor.x + 18;
	hoverTLY = cursor.y + 3;
	hoverBRX = hoverTLX + 100;
	hoverBRY = hoverTLY + 18;
	bool result = pix.SearchPixelArea(color, hoverTLX, hoverTLY, hoverBRX, hoverBRY);
	//if cant find box on the right side
	if (!result)  //the hover box flips sides when far right side of osbuddy, depends on text box size though...
	{
		hoverTLX = cursor.x - 100;
		hoverTLY = cursor.y + 3;
		hoverBRX = cursor.x - 18;
		hoverBRY = hoverTLY + 18;
		result = pix.SearchPixelArea(color, hoverTLX, hoverTLY, hoverBRX, hoverBRY);
	}
	return result;
}

//checks top left
bool InterfaceGeneral::VerifyHoverTextRL(unsigned int color)
{
	Sleep(20);
	Area HoverArea = { 3, 23, 313, 40 };
	return pix.SearchPixelArea(color, HoverArea,30);
}

//can trigger on "Level Color" of both monsters and players. Extremely recommend use in conjunction with verifyhovertext.
bool InterfaceGeneral::VerifyTopLeftText(unsigned int color)
{
	POINT topLeft;
	topLeft.x = 2 + SCREEN;
	topLeft.y = 33;
	//uses a shade tolerance of 55. this is important cause top-left hover text is semi transparent and changes given background. 55 is good.
	return pix.SearchPixelArea(color, topLeft.x, topLeft.y, topLeft.x+180, topLeft.y+18,65);
}

//Searches top left text between "beyond" and "until" for a color.
//searchBeyond==-1 means search from the farthest left, and searchUntil==-1 means search until we normally stop looking.
bool InterfaceGeneral::VerifyTopLeftText(unsigned int color, int searchBeyond, int searchUntil)
{
	POINT topLeft;
	topLeft.x = 2 + SCREEN;
	topLeft.y = 33;
	if (searchBeyond != -1) {
		topLeft.x = searchBeyond + SCREEN;
	}
	int until = SCREEN + 182;
	if (searchUntil != -1) {
		until = searchUntil + SCREEN;
	}
	//uses a shade tolerance of 55. this is important cause top-left hover text is semi transparent and changes given background. 55 is good.
	return pix.SearchPixelArea(color, topLeft.x, topLeft.y, until, topLeft.y + 18, 65);
}

bool InterfaceGeneral::VerifyOSbuddy()
{
	bool OSbuddy = false;

	OSbuddy = pix.VerifyPixelColor(pixStats);
	//OSbuddy &= pix.VerifyPixelColor(pixChat);
	OSbuddy &= pix.VerifyPixelColor(pixInv);

	return OSbuddy;
}

bool InterfaceGeneral::VerifyRuneLite()
{
	bool Runelite = false;

	printf("Waiting until runelite is up.\n");

	//wait until runelite is up
	WaitUntilEitherPixel(HPRL, expHPRL);

	Runelite = pix.VerifyRoughPixel(HPRL);
	//OSbuddy &= pix.VerifyPixelColor(pixChat);
	Runelite &= pix.VerifyRoughPixel(PrayRL);

	//maybe skill tracking menu open, trying with menu expanded coordinates
	if (!Runelite)
	{
		Runelite = pix.VerifyRoughPixel(expHPRL);
		//OSbuddy &= pix.VerifyPixelColor(pixChat);
		Runelite &= pix.VerifyRoughPixel(expPrayRL);
	}

	return Runelite;
}

//takes option choice on Zero Indexing (option 1 = optionChoice:0, option 2 = opChoice:1, etc.)
POINT InterfaceGeneral::GetMenuOptionCoords(int optionChoice)
{
	int offset;
	POINT cursor, Menu;
	cursor = mouse.GetPosition();

	Menu.x = cursor.x - (MENU_MINWIDTH/2);
	Menu.y = cursor.y + MENU_HEADER + (MENU_OPTION*optionChoice);

	//HANDLE WHEN MENU IS AT BOTTOM OF THE SCREEEEEN
	if (menu5Option)
		offset = cursor.y + MENU_HEADER + (MENU_OPTION * 5); //ONLY HANDLES MENUS WITH 5 OPTIONS
	else
		offset = cursor.y + MENU_HEADER + (MENU_OPTION * 4); //ONLY HANDLES MENUS WITH 4 OPTIONS

	offset = offset - 1038; //1037 is bottom of the screen where teh menu can maximally exist
	if (offset > 0)
	{// changes and realigns the menu box
		Menu.x = cursor.x - MENU_MINWIDTH;
		Menu.y = cursor.y-offset + MENU_HEADER + (MENU_OPTION*optionChoice);
		//printf("Menu: (%4i, %4i)", Menu.x, Menu.y);
	}
	return Menu;
}

//takes option choice on Zero Indexing (option 1 = optionChoice:0, option 2 = opChoice:1, etc.)
void InterfaceGeneral::ChooseMenuOption(int optionChoice)
{
	POINT option = GetMenuOptionCoords(optionChoice);
	//uses custom deviation as to not close the menu by moving mouse too far
	mouse.SetDeviation(15);//smaller deviation going from menu to option
	mouse.MouseMoveArea(option.x, option.y+5, option.x + MENU_MINWIDTH, option.y + MENU_OPTION - 5); //the 3's are buffer to accouunt for error
	mouse.SetDeviation(250); //reset deviation
	return;
}

//Chooses a menu option only if 'HOVER_xxx' exists on the desired option, Zero indexing
bool InterfaceGeneral::ChooseMenuOptionColorCheck(int optionChoice, unsigned int color)
{
	POINT option = GetMenuOptionCoords(optionChoice);
	//uses custom deviation as to not close the menu by moving mouse too far
	if (!pix.SearchPixelArea(color, option.x, option.y + 5, option.x + MENU_MINWIDTH+64, option.y + MENU_OPTION - 3))
		return false;
	mouse.SetDeviation(7);//smaller deviation going from menu to option
	mouse.MouseMoveArea(option.x, option.y + 5, option.x + MENU_MINWIDTH, option.y + MENU_OPTION - 3); //the 3's are buffer to accouunt for error
	mouse.SetDeviation(250); //reset deviation
	return true;
}

bool InterfaceGeneral::ChooseMenuOptionDoubleColorCheck(int optionChoice, unsigned int color, unsigned int color2)
{
	POINT option = GetMenuOptionCoords(optionChoice);
	//uses custom deviation as to not close the menu by moving mouse too far
	if (!pix.SearchPixelArea(color, option.x, option.y + 5, option.x + MENU_MINWIDTH + 64, option.y + MENU_OPTION - 3))
		return false;
	if (!pix.SearchPixelArea(color2, option.x, option.y + 5, option.x + MENU_MINWIDTH + 84, option.y + MENU_OPTION - 3))
		return false;
	mouse.SetDeviation(7);//smaller deviation going from menu to option
	mouse.MouseMoveArea(option.x, option.y + 5, option.x + MENU_MINWIDTH, option.y + MENU_OPTION - 3); //the 3's are buffer to accouunt for error
	mouse.SetDeviation(250); //reset deviation
	return true;
}

//Check if maybe a level up perhaps
bool InterfaceGeneral::CheckLevelUp()
{
	bool check = false;
	check = pix.SearchPixelArea(0x00008000, 290 + SCREEN, 908, 466 + SCREEN, 935, 10);
	if (check)
	{
		printf("LEVELED uppppp\n");
		
		for (int i = 0; i < 10; i++) {
			mouse.MouseMoveArea(160 + SCREEN, 970, 340 + SCREEN, 990);
			mouse.LeftClick();
			Sleep(800);
		}
		Keyboard().SendEnter();
	}
	return check;
}

bool InterfaceGeneral::DefiniteClick(unsigned int itemColor, int tolerance, Area region, unsigned int hoverColor, unsigned int menuColor, int menuOption, int maxAttempts)
{
	
	int tries = 0;
	while (tries < maxAttempts) {
		InterfaceGeneral::HandleHotkeys();
		tries++;
		if (pix.SearchPixelArea(itemColor, region.x1, region.y1, region.x2, region.y2,tolerance)) {
			POINT p = pix.SearchPixelAreaForPoint(itemColor, region.x1, region.y1, region.x2, region.y2, tolerance);
			mouse.MouseMove(p);
			Sleep(120);
			if (InterfaceGeneral::VerifyTopLeftText(hoverColor)) {
				mouse.RightClick();
				Sleep(30);
				if (InterfaceGeneral::ChooseMenuOptionColorCheck(menuOption, menuColor)) {
					Sleep(30);
					mouse.LeftClick();
					return true;
				}
			}
		}
	}

	tries = 0;
	while (tries < maxAttempts) {
		InterfaceGeneral::HandleHotkeys();
		tries++;
		mouse.MouseMoveArea(region);
		Sleep(120);
		if (InterfaceGeneral::VerifyTopLeftText(hoverColor)) {
			mouse.RightClick();
			Sleep(30);
			if (InterfaceGeneral::ChooseMenuOptionColorCheck(menuOption, menuColor)) {
				mouse.LeftClick();
				return true;
			}
		}
	}
	return false;
}

bool InterfaceGeneral::DefiniteClick(unsigned int itemColor, int tolerance, Area region, unsigned int hoverColor, unsigned int menuColor, int menuOption, int maxAttempts, int leftFrom, int leftTo)
{

	int tries = 0;
	while (tries < maxAttempts) {
		InterfaceGeneral::HandleHotkeys();
		tries++;
		if (pix.SearchPixelArea(itemColor, region.x1, region.y1, region.x2, region.y2, tolerance)) {
			POINT p = pix.SearchPixelAreaForPoint(itemColor, region.x1, region.y1, region.x2, region.y2, tolerance);
			mouse.MouseMove(p);
			Sleep(120);
			if (InterfaceGeneral::VerifyTopLeftText(hoverColor,leftFrom,leftTo)) {
				mouse.RightClick();
				Sleep(30);
				if (InterfaceGeneral::ChooseMenuOptionColorCheck(menuOption, menuColor)) {
					Sleep(30);
					mouse.LeftClick();
					return true;
				}
			}
		}
	}

	tries = 0;
	while (tries < maxAttempts) {
		InterfaceGeneral::HandleHotkeys();
		tries++;
		mouse.MouseMoveArea(region);
		Sleep(120);
		if (InterfaceGeneral::VerifyTopLeftText(hoverColor,leftFrom,leftTo)) {
			mouse.RightClick();
			Sleep(30);
			if (InterfaceGeneral::ChooseMenuOptionColorCheck(menuOption, menuColor)) {
				mouse.LeftClick();
				return true;
			}
		}
	}
	return false;
}

//Check if maybe a level up perhaps
// mode '0' ignores hitpoints levels
bool InterfaceGeneral::CheckLevelUp(int mode)
{
	bool check = false;
	check = pix.SearchPixelArea(0x00008000, 290, 908, 466, 935, 10);
	if (check)
	{
		if (mode == 0) //skip hp levels
		{
			if (pix.SearchPixelArea(0xb6145000, 37, 895, 180, 980, 20)) //if found hp
				check = false;
		}
		printf("LEVELED uppppp\n");
		mouse.MouseMoveArea(160, 970, 340, 990);
		mouse.LeftClick();
	}
	return check;
}


//Given three pixels, checks the environment to see if those pixels are where they should be
bool InterfaceGeneral::CheckLocation(Pixel pix1, Pixel pix2, Pixel pix3)
{
	bool result = true;
	result &= pix.VerifyRoughPixel(pix1);
	result &= pix.VerifyRoughPixel(pix2);
	result &= pix.VerifyRoughPixel(pix3);
	return result;
}

//Given a vector of however many pixels, check that all are present on screen at locations and shade expected at same time.
bool InterfaceGeneral::CheckExactLocation(std::vector<Pixel> Pixels)
{
	bool result = true;
	for (auto pixel : Pixels) {
		result &= pix.VerifyPixelColor(pixel);
	}
	return result;
}

//Given a vector of however many pixels, check that all are present on screen at rough locations expected at same time.
bool InterfaceGeneral::CheckRoughLocation(std::vector<Pixel> Pixels)
{
	bool result = true;
	for (auto pixel : Pixels) {
		result &= pix.VerifyRoughPixel(pixel);
	}
	if (result)
		printf("All Pixels found.\n");
	return result;
}

//waits until a pixel is seen
void InterfaceGeneral::WaitUntilPixel(Pixel pixel)
{
	while (!pix.VerifyRoughPixel(pixel))
	{
		Sleep(15);
	}
	return;
}

//waits until a pixel is seen
void InterfaceGeneral::WaitUntilPixels(std::vector<Pixel> pixels)
{
	while (!CheckRoughLocation(pixels))
	{
		Sleep(15);
	}
	return;
}

//waits until either pixel is seen - pix1 = true, pix2 = false
bool InterfaceGeneral::WaitUntilEitherPixel(Pixel pix1, Pixel pix2)
{
	while (!pix.VerifyRoughPixel(pix1))
	{
		if (!pix.VerifyRoughPixel(pix2))
		{
			Sleep(15);
			continue;
		}
		return false;
	}
	return true;
}

//waits until all sets of either pixel is seen. Runs indefinitely until all pixels met for set 1 or 2. True for pix1, False for pix2.
bool InterfaceGeneral::WaitUntilEitherPixel(std::vector<Pixel> pix1, std::vector<Pixel> pix2)
{
	while (true)
	{
		bool result = true;
		for (auto i : pix1)
		{
			result &= pix.VerifyRoughPixel(i);
		}
		if (result)
			return true;

		//reset and check second set of pixels.
		result = true;
		for (auto i : pix2)
		{
			result &= pix.VerifyRoughPixel(i);
		}
		if (result)
			return false;
	}
}

//Sleeps until either pixel is seen, returns true for 1st pixel, false for second pixel
bool InterfaceGeneral::WaitUntilEitherPixelArea(unsigned int pix1, unsigned int pix2, Area zone)
{
	while (!pix.SearchPixelArea(pix1, zone))
	{
		if (!pix.SearchPixelArea(pix2, zone))
		{
			Sleep(15);
			continue;
		}
		return false;
	}
	return true;
}

//Sleeps until either pixel is seen, returns true for 1st pixel, false for second pixel
bool InterfaceGeneral::WaitUntilEitherPixelArea(unsigned int pix1, unsigned int pix2, Area zone, int tolerance)
{
	while (!pix.SearchPixelArea(pix1, zone, tolerance))
	{
		if (!pix.SearchPixelArea(pix2, zone, tolerance))
		{
			Sleep(15);
			continue;
		}
		return false;
	}
	return true;
}


//Sleeps until either pixel is seen, returns true for 1st pixel, false for second pixel
bool InterfaceGeneral::WaitUntilEitherPixelArea(Pixel pix1, Pixel pix2, Area zone)
{
	while (!pix.SearchPixelArea(pix1._color, zone))
	{
		if (!pix.SearchPixelArea(pix2._color, zone))
		{
			Sleep(15);
			continue;
		}
		return false;
	}
	return true;
}

//Sleeps until either pixel is seen, returns true for 1st pixel, false for second pixel
bool InterfaceGeneral::WaitUntilEitherPixelArea(Pixel pix1, Pixel pix2, Area zone, int tolerance)
{
	while (!pix.SearchPixelArea(pix1._color, zone, tolerance))
	{
		if (!pix.SearchPixelArea(pix2._color, zone, tolerance))
		{
			Sleep(15);
			continue;
		}
		return false;
	}
	return true;
}

//Sleeps until either pixel is seen, returns true for 1st pixel, false for second pixel
bool InterfaceGeneral::WaitUntilEitherPregion(Pregion one, Pregion two)
{
	while (!pix.SearchPixelArea(one.pix._color, one.area))
	{
		if (!pix.SearchPixelArea(two.pix._color, two.area))
		{
			Sleep(15);
			continue;
		}
		return false;
	}
	return true;
}

//waits until a pixel is seen
void InterfaceGeneral::WaitUntilPregion(Pregion pregion)
{
	while (!pix.SearchPixelArea(pregion.pix._color, pregion.area))
	{
		Sleep(15);
	}
	return;
}

bool InterfaceGeneral::NavigateToPregion(Pregion reg, unsigned int tolerance)
{
	//first attempt to navigate to roughly the pixel, if a valid pixel was given
	if (!(reg.pix._x <= 0) && !(reg.pix._y <= 0))
	{
		if (pix.VerifyPixelColor(reg.pix))
		{
			POINT exact;
			exact.x = reg.pix._x; exact.y = reg.pix._y;
			mouse.MouseMoveRoughly(exact, tolerance);
			Sleep(20);
			mouse.LeftClick();
			return true;
		}
	}

	POINT p = pix.SearchPixelAreaForPoint(reg.pix._color, reg.area);
	//check that p is valid point
	if (p.x != -1 && p.y != -1)
	{
		mouse.MouseMoveRoughly(p, tolerance);
		Sleep(20);
		mouse.LeftClick();
		return true;
	}

	return false;
}


//Defaults to blue action hover text
bool InterfaceGeneral::NavigateToPregionWithHover(Pregion reg, unsigned int hover, unsigned int tolerance)
{
	//in case where a valid pixel is given check first that pixel
	//first attempt to navigate to roughly the pixel, if a valid pixel was given
	if (!(reg.pix._x <= 0) && !(reg.pix._y <= 0))
	{
		printf( "Exact pixel given. ");
		if (pix.VerifyPixelColor(reg.pix))
		{
			POINT exact;
			exact.x = reg.pix._x; exact.y = reg.pix._y;

			//try three times
			for (auto i = 0; i< 3; i++)
			{
				mouse.MouseMoveRoughly(exact, tolerance);
				Sleep(500);
				if (VerifyHoverTextRL(hover))
				{
					printf(" Exact pixel - Hover text found.");
					mouse.LeftClick();
					return true;
				}
			}

		}
	}


	printf(" Searching for nav pixel.\n");
	POINT p = pix.SearchPixelAreaForPoint(reg.pix._color, reg.area);
	//Use back up color if first one failed
	if (p.x == -1 && p.y == -1 && reg.altColor != 0xFFFFFFFF)
	{
		printf(" Using backup color. ");
		p = pix.SearchPixelAreaForPoint(reg.altColor, reg.area);
	}
	if (p.x != -1 && p.y != -1)
	{
		//try three times
		for (auto i = 0; i < 3; i++)
		{
			printf(" Attempt %d. ", i);
			mouse.MouseMoveRoughly(p, tolerance);
			Sleep(500);
			if (VerifyHoverTextRL(hover))
			{
				printf(" Pixel Search - Hover text found.");
				mouse.LeftClick();
				return true;
			}
		}
	}

	return false;
}

//when all else fails. gtfo
void InterfaceGeneral::Logout()
{
	Area doorButton(1521 + SCREEN, 1004, 1544 + SCREEN, 1033);
	Area Logout(1461 + SCREEN, 942, 1584 + SCREEN, 965);
	mouse.MouseMoveArea(doorButton);
	Sleep(400);
	mouse.LeftClick();
	Sleep(400);
	mouse.MouseMoveArea(Logout);
	Sleep(400);
	mouse.LeftClick();
	return;
}

//when all else fails. gtfo. quick
void InterfaceGeneral::LogoutQuick()
{
	BlockInput(true);
	mouse.ChangeSpeed(0.3);
	Area doorButton(1521 + SCREEN, 1004, 1544 + SCREEN, 1033);
	Area Logout(1461 + SCREEN, 942, 1584 + SCREEN, 965);
	mouse.MouseMoveArea(doorButton);
	Sleep(40);
	mouse.LeftClick();
	Sleep(40);
	mouse.MouseMoveArea(Logout);
	Sleep(40);
	mouse.LeftClick();
	Sleep(50);
	mouse.LeftClick();
	BlockInput(false);
	POINT logCurs = mouse.GetPosition(); //get logout button position
	POINT curs = mouse.GetPosition();
	while (VerifyOSbuddy() && (curs.x == logCurs.x) && (curs.y == logCurs.y)) //while not logged out yet OR mouse movement, keep clicking logout
	{
		printf("Clicking Logout!\n");
		curs= mouse.GetPosition();
		Sleep(50);
		mouse.LeftClick();
	}
	return;
}

//checks for black background in health circle
//amount should be some 1-100 value. 
//returns true if health is there
bool InterfaceGeneral::CheckHealthAmount(int amount)
{
	bool result = true; //assumes you ahve the health
	unsigned int HealthBGBlack1 = 0x13131300; //colors of the background blacki in teh circles
	unsigned int HealthBGBlack2 = 0x0b0b0b00;

	if (amount >= 90)
	{
		if ((pix.VerifyPixelColor(HealthBGBlack1, 1471 + SCREEN, 85)) || (pix.VerifyPixelColor(HealthBGBlack2, 1471 + SCREEN, 85))) //90 % pixel
			result = false;
	}

	else if (amount >= 70 && amount < 90)
	{
		if ((pix.VerifyPixelColor(HealthBGBlack1, 1462 + SCREEN, 90)) || (pix.VerifyPixelColor(HealthBGBlack2, 1462 + SCREEN, 90)))
			result = false;
	}

	else if (amount >= 50 && amount < 70)
	{
		if ((pix.VerifyPixelColor(HealthBGBlack1, 1462 + SCREEN, 95)) || (pix.VerifyPixelColor(HealthBGBlack2, 1462 + SCREEN, 95)))
			result = false;
	}

	else if (amount >= 35 && amount < 50)
	{
		if ((pix.VerifyPixelColor(HealthBGBlack1, 1464 + SCREEN, 100)) || (pix.VerifyPixelColor(HealthBGBlack2, 1464 + SCREEN, 100)))
			result = false;
	}
	else if (amount >= 25 && amount < 35)
	{
		if ((pix.VerifyPixelColor(HealthBGBlack1, 1464 + SCREEN, 103)) || (pix.VerifyPixelColor(HealthBGBlack2, 1464 + SCREEN, 103)))
			result = false;
	}



	return result;
}


//clicks on the compass for normalization of the game view
// mode: 0 - just clicks compass for northern view.
// mode: 1 - clicks compass and holds up arrow to move camera up
void InterfaceGeneral::NormalizeCompass(int mode)
{
	mouse.MouseMoveArea(1470+SCREEN,40,1495+SCREEN,70);
	Sleep(30);
	mouse.LeftClick();
	Sleep(40);

	switch (mode)
	{
	case 1: key.Arrow(LEFT, key.SetDelayRandRange(50,400)); break; //Normalize and random LEFT the camera
	case 2: key.Arrow(RIGHT, key.SetDelayRandRange(50, 400)); break; //Normalize and RIGHT the camera
	case 3: key.Arrow(UP, 1000); break; //Normalize and UP the camera
	case 4: key.Arrow(DOWN, 1000); break; //Normalize and DOWN the camera
	default: break;
	}

}


//sets mouse move speed 0.1-1.0 where lower is faster
void InterfaceGeneral::SetMouseSpeed(float speed)
{
	mouse.ChangeSpeed(speed);
	return;
}


//sets mouse move speed 0.1-1.0 where lower is faster
void InterfaceGeneral::SetMouseDeviation(int Deviation)
{
	mouse.SetDeviation(Deviation);
	return;
}

//checks if loggedout or in process of logging in.
//if false, it is logged out
//if true, it is logged in
bool InterfaceGeneral::CheckIfInGame()
{
	unsigned int blackBorder = 0x00000000;
	bool Check = true;

	//checks left side
	Check &= pix.VerifyPixelColor(blackBorder, 300 + SCREEN, 50);
	Check &= pix.VerifyPixelColor(blackBorder, 300 + SCREEN, 350);
	Check &= pix.VerifyPixelColor(blackBorder, 300 + SCREEN, 550);

	//bottom section
	Check &= pix.VerifyPixelColor(blackBorder, 700 + SCREEN, 700);

	//checks right side
	Check &= pix.VerifyPixelColor(blackBorder, 1250 + SCREEN, 50);
	Check &= pix.VerifyPixelColor(blackBorder, 1250 + SCREEN, 350);
	Check &= pix.VerifyPixelColor(blackBorder, 1250 + SCREEN, 550);

	//inverse as currently its checking for the black
	return !Check;
}

//after 6 hours it logs you out. this logs you back innnn
bool InterfaceGeneral::HandleAutoLogOut()
{
	const int passwordLen = sizeof(password);
	char pass[passwordLen];
	//strncpy_s(pass, password, passwordLen);

	if (!CheckIfInGame()) //confirmed logout
	{

		if (logTimeout <= 0)
			exit(0);

		printf("======================= Auto Logout Detected ==============================\n");
		if (pix.VerifyPixelColor(0xf1f2a000, 700 + SCREEN, 130) || pix.VerifyPixelColor(0xf1f2a000, 706 + SCREEN, 130)) //yellow sword crossguard
		{
			printf("Logout screen sword glint\n");
			mouse.MouseMoveArea(795+SCREEN, 302, 810+SCREEN, 307); //move login
			Sleep(100);
			mouse.LeftClick();
			key.Type(pass,passwordLen);//type dat password yo
			key.GenerateKey(VK_RETURN, false, false); //send enter
			
			int to = 0;
			while (!pix.VerifyPixelColor(0xab837f00, 719 + SCREEN, 326)) //waits for the red button to appear
			{
				to++;
				if (to > 10000) {
					printf("Login timeout\n");
					exit(1);
				}
				
			}

		}

		if (pix.VerifyPixelColor(0xab837f00, 719 + SCREEN, 326)) //red buttons border highlight
		{
			printf("Loggin screen red button\n");
			mouse.MouseMoveArea(719+SCREEN,327,930+SCREEN, 400); //move to red button
			Sleep(100);
			mouse.LeftClick();
			Sleep(3000);
			logTimeout--;
		}
	}

	return false;
}

//returns an Area struct including a box of radius R around x and y
Area InterfaceGeneral::areaBox(int x, int y, int r) {
	Area t;
	t.x1 = x - r;
	t.y1 = y - r;
	t.x2 = x + r;
	t.y2 = y + r;
	return t;
}

//returns an Area struct including a box of radius R around x and y
Area InterfaceGeneral::areaBox(POINT p, int r) {
	return areaBox(p.x, p.y, r);

}

//returns an Area struct including a box of radius R around x and y
Area InterfaceGeneral::areaBox(int x, int y, int height, int width) {
	Area t;
	t.x1 = x - width;
	t.y1 = y - height;
	t.x2 = x + width;
	t.y2 = y + height;
	return t;
}

//returns an Area struct including a box of radius R around x and y
Area InterfaceGeneral::areaBox(POINT p, int height, int width) {
	return areaBox(p.x, p.y, height, width);
}

bool InterfaceGeneral::pingMessage()
{
	if (progname) {
		system(progname);
		return true;
	}
	return false;
}

//computes the point on a circle given radius and degrees about pt (0,0)
fPoint InterfaceGeneral::getCircleCoord(int radius, float degrees)
{
	fPoint coord;
	coord.x = radius*(cos(degrees*PI / 180));
	coord.y = radius*(sin(degrees*PI / 180));
	return coord;
}

//Given a center point to rotate around
// A radius away from the center the pixel should rotate by
// and the degrees wished to rotate, compute the pixel coords
// returns pix coordinates
POINT InterfaceGeneral::rotatePixAboutCenter(POINT center, int radius, float degrees)
{
	fPoint circlePt = getCircleCoord(radius, degrees);
	circlePt.y *= -1; // reverse Y, as up is negative in pixels, and down is positive in pixels
	POINT amount;
	POINT coord;
	amount.x = (int)circlePt.x;
	amount.y = (int)circlePt.y;
	coord.x = amount.x + center.x;
	coord.y = amount.y + center.y;
	return coord;
}

POINT InterfaceGeneral::rotatePixAboutCenter(Pixel center, int radius, float degrees)
{
	POINT cent, result;
	cent.x = center._x;
	cent.y = center._y;
	
	result = rotatePixAboutCenter(cent, radius, degrees);
	return result;
}

//changes the compass to a set degree
bool InterfaceGeneral::ChangeCompassDegrees(int degrees)
{
	POINT tip = rotatePixAboutCenter(compass, COMPASS_ARM, degrees);
	int Timeout = 100;
	while (!pix.VerifyPixelColor(compass._color, tip.x, tip.y) && (Timeout > 0))
	{
		Timeout--;
		key.Arrow(LEFT, 100);
	}
	if (Timeout == 0)
		printf("=========== Somethings gone wrong, Compass couldnt be set to %i degrees!", degrees);
	return true;
}

