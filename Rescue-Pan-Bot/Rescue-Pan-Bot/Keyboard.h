#pragma once
#include "Windows.h"
#include <stdio.h>
#include <iostream>


class Keyboard
{
private:
	int pressTime = 10; //how long key is pressed in miliseconds

public:

	//Sets pressdown time
	void SetDelay(int ms)
	{
		pressTime = ms;
	}

	//generates a random key delay between range - INCLUDES BOTH ENDPOINTS
	void SetDelayRandRange(int RangeStart, int RangeEnd)
	{
		int range = RangeEnd - RangeStart;
		pressTime = rand() % range + RangeStart + 1;
	}

	//expects the key (as a capital letter), whether its held down, and whether to yield a capital letter. 
	// example sending ('A',false,false) yields 'a'
	// to achieve 'A', send ('A',false,true)
	void GenerateKey(int vk, BOOL bExtended, bool shift) 
	{

		KEYBDINPUT  kb = { 0 };
		INPUT       Input = { 0 };

		if (shift) // press the shift key
		{
			// Press shift key
			Input.type = INPUT_KEYBOARD;
			Input.ki.dwFlags = KEYEVENTF_SCANCODE;
			Input.ki.wScan = MapVirtualKey(VK_LSHIFT, 0);
			SendInput(1, &Input, sizeof(Input));

		}

		/* Generate a "key down" */
		if (bExtended) { kb.dwFlags = KEYEVENTF_EXTENDEDKEY; }
		kb.wVk = vk;
		Input.type = INPUT_KEYBOARD;
		Input.ki = kb;
		SendInput(1, &Input, sizeof(Input));

		Sleep(pressTime);

		/* Generate a "key up" */
		ZeroMemory(&kb, sizeof(KEYBDINPUT));
		ZeroMemory(&Input, sizeof(INPUT));
		kb.dwFlags = KEYEVENTF_KEYUP;
		if (bExtended) { kb.dwFlags |= KEYEVENTF_EXTENDEDKEY; }
		kb.wVk = vk;
		Input.type = INPUT_KEYBOARD;
		Input.ki = kb;
		SendInput(1, &Input, sizeof(Input));

		if (shift) //release shift key if pressed
		{
			// Release shift key
			Input.type = INPUT_KEYBOARD;
			Input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			Input.ki.wScan = MapVirtualKey(VK_LSHIFT, 0);
			SendInput(1, &Input, sizeof(Input));

		}

		return;
	}

	void Type(char* phrase, int length)
	{
		bool Uppercase = false;
		char letter;
		for (int i = 0; i < length; i++)
		{
			if ((phrase[i] >= 65) && (phrase[i] <= 90)) //if in the uppercase range
				Uppercase = true;
			else
				Uppercase = false;

			letter = phrase[i];

			GenerateKey(toupper(letter), false, Uppercase);
			Sleep(100);
		}
		return;
	}

	//	1 = left
	//	2 = right
	//	3 = up
	//	4 = down
	void Arrow(int arrow, int delay)
	{
		SetDelay(delay);
		switch (arrow)
		{
				//left
			case 1:	GenerateKey(VK_LEFT, false, false); break;
				//left
			case 2:	GenerateKey(VK_RIGHT, false, false); break;
				//left
			case 3:	GenerateKey(VK_UP, false, false); break;
				//left
			case 4:	GenerateKey(VK_DOWN, false, false); break;
		}
	}

	void SendEnter()
	{
		GenerateKey(VK_RETURN, false, false); //send enter
		return;
	}

	void TypeNum(int number)
	{
		char numBuffer[20];
		sprintf_s(numBuffer, "%ld", number);
		Type(numBuffer, 20);
		SendEnter();
	}
};