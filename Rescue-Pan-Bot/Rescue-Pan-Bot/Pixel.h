#pragma once
#ifndef _PIXEL_H_
#define _PIXEL_H_

#include "Config.h"
#include"Windows.h"
#include "stdio.h"

class PixelHandler {
private:
	//HDC _hDC			//deprecated windows getpixel hdc
	HDC hdc, hdcTemp; //handle device connectivity
	BYTE* bitPointer;


public:
	//Constructor, set up for handling pixel operations
	PixelHandler();

	//Destructor used to free memory after object is deleted
	~PixelHandler();

	//Returns true if successfully initializes objects device controllers, else false
	bool Initialize();

	//searches a given area for a color [HEX - RGBA] - returns true or false if found
	bool SearchPixelArea(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY);

	//searches a given area for a color [HEX - RGBA] - returns true or false if found, within shade tolerance
	bool SearchPixelArea(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY, int tolerance);

	//searches a given area for a color [HEX - RGBA] - returns Point where color found
	POINT SearchPixelAreaForPoint(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY, int tolerance);


	//searches a given area for a color [HEX - RGBA] - returns Point where color found
	POINT SearchPixelAreaForPoint(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY);

	//returns pixel color of a coordinate on screen
	unsigned int GetPixelColor(int x, int y);

	//sameas a bove but with points
	unsigned int GetPixelColor(POINT coord);

	//checks the color [HEX - RGBA] of a single pixel.
	bool VerifyPixelColor(unsigned int color, int x, int y);

	//checks the color [HEX - RGBA] of a single pixel. within shade tolerance
	bool VerifyPixelColor(unsigned int color, int x, int y, int tolerance);

	//checks the color [HEX - RGBA] of a single pixel. (overloaded function for handling pixel struct)
	bool VerifyPixelColor(Pixel pix);

	//prints out color and (x,y) of mouse pointer. infinite while loop CTRL + C to leave.
	unsigned int DEBUG();

	/*====== DEPRECATED==========
	unsigned int WINGetPixelColor(int x, int y);
	*/


};

#endif