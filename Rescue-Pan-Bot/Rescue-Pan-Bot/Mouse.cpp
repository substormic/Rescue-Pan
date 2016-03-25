#include "Mouse.h"


bool Mouse::MouseMoveArea(int x1, int y1, int x2, int y2)
{
	//check that the given coords are top left and bottom right
	if ((x1 > x2) || (y1 > y2))
		return false;

	int validX, validY, finalX, finalY;
	validX = x2 - x1;
	validY = y2 - y1;

	//choose random coordinates within the box
	finalX = rand() % validX + x1;
	finalY = rand() % validY + y1;

	if (MouseMove(finalX, finalY))
		return true;
	return false;
}

bool Mouse::MouseMoveArea(Area region)
{
	return MouseMoveArea(region.x1, region.y1, region.x2, region.y2);
}


//Deviates PATH Randomly!! mouse move - recieves pixel coordinates. change mouse position to those coords: DEVIATION CHOOSING
bool Mouse::MouseMove(int x, int y)
{
	POINT curMouse;
	float stepX, stepY, interimX, interimY, progress;
	bool getMouse = GetCursorPos(&curMouse);
	if (!getMouse)
		return false;
	int deviatePathY = rand() % (deviation*2) - deviation; //deviate -250 to 250 (max 250 pixels from path) or specified deviation
	int deviatePathx = rand() % (deviation*2) - deviation; //deviate -250 to 250 (max 250 pixels from path) or specified deviation

														  //change below to alter the speed of the movement
	int stepSize = rand() % 150 + 70; //generate random number between 70 and 220
	float speed = MouseSpeed*stepSize;
	stepSize = (int)(speed);

	stepX = (x - curMouse.x) / (float)stepSize;
	stepY = (y - curMouse.y) / (float)stepSize;

	for (int i = 1; i <= stepSize; i++)
	{
		progress = (i / (float)stepSize) * 3.1415;
		interimX = (curMouse.x + (i*stepX)) + (deviatePathx * sin(progress));
		interimY = (curMouse.y + (i*stepY)) + (deviatePathY * sin(progress));
		//printf("itterim: %.0f, %.0f\n", round(interimX), round(interimY));
		SetCursorPos(round(interimX), round(interimY));
		Sleep(1);
	}
	//printf("Final: %i,%i\n", x, y);
	//SetCursorPos(x, y);
	return true;
}

//Deviates PATH Randomly!! mouse move - recieves pixel coordinates. change mouse position to those coords: DEVIATION CHOOSING
bool Mouse::MouseMove(POINT coord)
{
	MouseMove(coord.x, coord.y);
	return true;
}

//STRAIGHT LINE DISTANCE mouse move - recieves pixel coordinates. change mouse position to those coords
bool Mouse::MouseMoveSLD(int x, int y)
{
	POINT curMouse;
	float stepX, stepY, interimX, interimY, progress;
	bool getMouse = GetCursorPos(&curMouse);
	if (!getMouse)
		return false;

	//change below to alter the speed of the movement
	int stepSize = rand() % 170 + 70; //generate random number between 70 and 240
	stepX = (x - curMouse.x) / (float)stepSize;
	stepY = (y - curMouse.y) / (float)stepSize;

	for (int i = 1; i <= stepSize; i++)
	{
		progress = i / stepSize;
		interimX = (curMouse.x + (i*stepX));
		interimY = (curMouse.y + (i*stepY));
		SetCursorPos(round(interimX), round(interimY));
		Sleep(1);
	}
	//printf("Final: %i,%i\n", x, y);
	//SetCursorPos(x, y);
	return true;
}

void Mouse::RightClick()
{
	INPUT    Input = { 0 };
	// right down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	// right up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

void Mouse::LeftClick()
{
	INPUT    Input = { 0 };
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	// left up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}


POINT Mouse::GetPosition() 
{
	POINT curMouse;
	GetCursorPos(&curMouse);
	return curMouse;
}

void Mouse::ChangeSpeed(float speed)
{
	MouseSpeed = speed;
}

void Mouse::SetDeviation(int dev)
{
	deviation = dev;
}