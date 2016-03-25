#pragma once
#include "Windows.h"
#include <stdio.h>


class Keyboard
{

public:
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
};