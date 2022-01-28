#include "Pixel.h"

//Constructor, set up for handling pixel operations
PixelHandler::PixelHandler()
{
	if (!Initialize())
		printf("!ERROR! could not initialize Pixel Checking!");
}

//destructor
PixelHandler::~PixelHandler()
{
	DeleteDC(hdc);
}

bool PixelHandler::Initialize()
{
	//_hDC = GetDC(NULL);      			//deprecated windows getpixel hdc   
	hdc = GetDC(HWND_DESKTOP);
	if (hdc == NULL)
		return false;
	return true;
}


/* INPUT: color - HEX (RGBA) color to be searched
upper left boundary of search box
bottom right boundary of search box
OUTPUT:	returns TRUE if it finds color in specified area
!warning! returns true on first instance found
reading top left to bottom right - horizontally
otherwise returns FALSE  */
bool PixelHandler::SearchPixelArea(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY)
{
	int red, green, blue, alpha;
	unsigned int _color = 0;

	int MAX_WIDTH = (BottomRightX - UpperLeftX) + 1; //area to be searched
	int MAX_HEIGHT = (BottomRightY - UpperLeftY) + 1; //area to be searched

	hdcTemp = CreateCompatibleDC(hdc);
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = MAX_WIDTH;
	bitmap.bmiHeader.biHeight = MAX_HEIGHT;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = MAX_WIDTH * 4 * MAX_HEIGHT;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;
	HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
	SelectObject(hdcTemp, hBitmap2);
	BitBlt(hdcTemp, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc, UpperLeftX, UpperLeftY, SRCCOPY);

	//Check the bitmap for color iteratively
	for (int i = 0; i<(MAX_WIDTH * 4 * MAX_HEIGHT); i += 4)
	{
		_color = 0;
		blue = (int)bitPointer[i];
		green = (int)bitPointer[i + 1];
		red = (int)bitPointer[i + 2];
		alpha = (int)bitPointer[i + 3];
		_color |= red << 24;
		_color |= green << 16;
		_color |= blue << 8;

		if (color == _color)//check if color matches some pixel in area
		{
			DeleteDC(hdcTemp);
			DeleteObject(hBitmap2); //release memory
			return true;
		}
	}
	DeleteDC(hdcTemp);
	DeleteObject(hBitmap2);
	return false;
}


//searches a given area for a color [HEX - RGBA] - returns true or false if found
bool PixelHandler::SearchPixelArea(unsigned int color, Area region)
{
	return SearchPixelArea(color, region.x1, region.y1, region.x2, region.y2);
}

/* INPUT: color - HEX (RGBA) color to be searched
upper left boundary of search box
bottom right boundary of search box
OUTPUT:	returns TRUE if it finds color in specified area
!warning! returns true on first instance found
reading top left to bottom right - horizontally
otherwise returns FALSE  */
bool PixelHandler::SearchPixelArea(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY, int tolerance)
{
	int red, green, blue, alpha;
	unsigned char _red, _blue, _green;
	unsigned int _color = 0;

	int MAX_WIDTH = (BottomRightX - UpperLeftX) + 1; //area to be searched
	int MAX_HEIGHT = (BottomRightY - UpperLeftY) + 1; //area to be searched

	hdcTemp = CreateCompatibleDC(hdc);
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = MAX_WIDTH;
	bitmap.bmiHeader.biHeight = MAX_HEIGHT;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = MAX_WIDTH * 4 * MAX_HEIGHT;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;
	HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
	SelectObject(hdcTemp, hBitmap2);
	BitBlt(hdcTemp, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc, UpperLeftX, UpperLeftY, SRCCOPY);

	//Check the bitmap for color iteratively
	for (int i = 0; i<(MAX_WIDTH * 4 * MAX_HEIGHT); i += 4)
	{
		_color = 0;
		blue = (int)bitPointer[i];
		green = (int)bitPointer[i + 1];
		red = (int)bitPointer[i + 2];
		alpha = (int)bitPointer[i + 3];

		_red = (color & 0xff000000) >> 24;
		_green = (color & 0x00ff0000) >> 16;
		_blue = (color & 0x0000ff00) >> 8;

		if (((_red < (red + tolerance)) && (_red >(red - tolerance))) && ((_green < (green + tolerance)) && (_green >(green - tolerance))) && ((_blue < (blue + tolerance)) && (_blue >(blue - tolerance)))) //check if color matches some pixel in area
		{
			DeleteDC(hdcTemp);
			DeleteObject(hBitmap2); //release memory
			return true;
		}
	}
	DeleteDC(hdcTemp);
	DeleteObject(hBitmap2);
	return false;
}

bool PixelHandler::SearchPixelArea(unsigned int color, Area region, int tolerance)
{
	return SearchPixelArea(color, region.x1, region.y1, region.x2, region.y2, tolerance);
}


//returns coordinates by value not reference. Values = -1 if not found
POINT PixelHandler::SearchPixelAreaForPoint(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY, int tolerance)
{
	POINT pix; //coordinates where the color is found
	int red, green, blue, alpha;
	unsigned char _red, _green,_blue;
	unsigned int _color = 0;

	int MAX_WIDTH = (BottomRightX - UpperLeftX) + 1; //area to be searched
	int MAX_HEIGHT = (BottomRightY - UpperLeftY) + 1; //area to be searched

	hdcTemp = CreateCompatibleDC(hdc);
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = MAX_WIDTH;
	bitmap.bmiHeader.biHeight = MAX_HEIGHT;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = MAX_WIDTH * 4 * MAX_HEIGHT;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;
	HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
	SelectObject(hdcTemp, hBitmap2);
	BitBlt(hdcTemp, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc, UpperLeftX, UpperLeftY, SRCCOPY);

	//Check the bitmap for color iteratively
	for (int i = 0; i<(MAX_WIDTH * 4 * MAX_HEIGHT); i += 4)
	{
		_color = 0;
		blue = (int)bitPointer[i];
		green = (int)bitPointer[i + 1];
		red = (int)bitPointer[i + 2];
		alpha = (int)bitPointer[i + 3];
		_color |= red << 24;
		_color |= green << 16;
		_color |= blue << 8;

		_red = (color & 0xff000000) >> 24;
		_green = (color & 0x00ff0000) >> 16;
		_blue = (color & 0x0000ff00) >> 8;

		if (((_red < (red + tolerance)) && (_red >(red - tolerance))) && ((_green < (green + tolerance)) && (_green >(green - tolerance))) && ((_blue < (blue + tolerance)) && (_blue >(blue - tolerance)))) //check if color matches some pixel in area
		{
			// compute coords found
			int BotPos = i / 4; //current position built from bottom up
			int RowRemain = (MAX_WIDTH - (BotPos % MAX_WIDTH)) % MAX_WIDTH;//grabs remaining pixels in row
			float antiRows = (BotPos / (MAX_WIDTH));					  //Grab the rows under position
			if (RowRemain == 0)
				antiRows = ((BotPos - 1) / (MAX_WIDTH));
			int result = (MAX_WIDTH*MAX_HEIGHT) - ((int)antiRows*MAX_WIDTH + RowRemain); //it builds pixels left to right, BOTTOM UP. so this ludicrous algorithm is necessary

			pix.x = (i % (4 * MAX_WIDTH)) / 4;
			pix.y = (int)(result / MAX_WIDTH);
			pix.x += UpperLeftX;
			pix.y += UpperLeftY;


			DeleteDC(hdcTemp);
			DeleteObject(hBitmap2); //release memory
			return pix;
		}
	}
	DeleteDC(hdcTemp);
	DeleteObject(hBitmap2);
	pix.x = -1;
	pix.y = -1;
	return pix;
}

POINT PixelHandler::SearchPixelAreaForPoint(unsigned int color, Area region, int tolerance)
{
	return SearchPixelAreaForPoint(color, region.x1, region.y1, region.x2, region.y2, tolerance);
}

//returns coordinates by value not reference. Values = -1 if not found
POINT PixelHandler::SearchPixelAreaForPoint(unsigned int color, int UpperLeftX, int UpperLeftY, int BottomRightX, int BottomRightY)
{
	POINT pix; //coordinates where the color is found
	int red, green, blue, alpha;
	unsigned int _color = 0;

	int MAX_WIDTH = (BottomRightX - UpperLeftX) + 1; //area to be searched
	int MAX_HEIGHT = (BottomRightY - UpperLeftY) + 1; //area to be searched

	hdcTemp = CreateCompatibleDC(hdc);
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = MAX_WIDTH;
	bitmap.bmiHeader.biHeight = MAX_HEIGHT;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = MAX_WIDTH * 4 * MAX_HEIGHT;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;
	HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
	SelectObject(hdcTemp, hBitmap2);
	BitBlt(hdcTemp, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc, UpperLeftX, UpperLeftY, SRCCOPY);

	//Check the bitmap for color iteratively
	for (int i = 0; i<(MAX_WIDTH * 4 * MAX_HEIGHT); i += 4)
	{
		_color = 0;
		blue = (int)bitPointer[i];
		green = (int)bitPointer[i + 1];
		red = (int)bitPointer[i + 2];
		alpha = (int)bitPointer[i + 3];
		_color |= red << 24;
		_color |= green << 16;
		_color |= blue << 8;

		if (color == _color)//check if color matches some pixel in area
		{
			// compute coords found
			int BotPos = i / 4; //current position built from bottom up
			int RowRemain = (MAX_WIDTH - (BotPos % MAX_WIDTH)) % MAX_WIDTH;//grabs remaining pixels in row
			float antiRows = (BotPos / (MAX_WIDTH));					  //Grab the rows under position
			if (RowRemain == 0)
				antiRows = ((BotPos - 1) / (MAX_WIDTH));
			int result = (MAX_WIDTH*MAX_HEIGHT) - ((int)antiRows*MAX_WIDTH + RowRemain); //it builds pixels left to right, BOTTOM UP. so this ludicrous algorithm is necessary

			pix.x = (i % (4 * MAX_WIDTH)) / 4;
			pix.y = (int)(result / MAX_WIDTH);
			pix.x += UpperLeftX;
			pix.y += UpperLeftY;

			DeleteDC(hdcTemp);
			DeleteObject(hBitmap2); //release memory
			return pix;
		}
	}
	DeleteDC(hdcTemp);
	DeleteObject(hBitmap2);
	pix.x = -1;
	pix.y = -1;
	return pix;
}

POINT PixelHandler::SearchPixelAreaForPoint(unsigned int color, Area region)
{
	return SearchPixelAreaForPoint(color, region.x1, region.y1, region.x2, region.y2);
}

/* INPUT:	(x,y) coordinates of pixel to be checked
OUTPUT:	returns the color at said coordinate */
unsigned int PixelHandler::GetPixelColor(int x, int y)
{
	int i = 0;
	int red, green, blue, alpha;
	unsigned int _color = 0;

	int MAX_WIDTH = 1; //area to be searched
	int MAX_HEIGHT = 1; //area to be searched

	hdcTemp = CreateCompatibleDC(hdc);
	BITMAPINFO bitmap;
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = MAX_WIDTH;
	bitmap.bmiHeader.biHeight = MAX_HEIGHT;
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;
	bitmap.bmiHeader.biSizeImage = MAX_WIDTH * 4 * MAX_HEIGHT;
	bitmap.bmiHeader.biClrUsed = 0;
	bitmap.bmiHeader.biClrImportant = 0;
	HBITMAP hBitmap2 = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, (void**)(&bitPointer), NULL, NULL);
	SelectObject(hdcTemp, hBitmap2);
	BitBlt(hdcTemp, 0, 0, MAX_WIDTH, MAX_HEIGHT, hdc, x, y, SRCCOPY);

	_color = 0;
	blue = (int)bitPointer[i];
	green = (int)bitPointer[i + 1];
	red = (int)bitPointer[i + 2];
	alpha = (int)bitPointer[i + 3];
	_color |= red << 24;
	_color |= green << 16;
	_color |= blue << 8;
	//printf("R:%4i, G:%4i, B:%4i  ... (%4i,%4i)  %08x\n", red, green, blue, x, y, _color);

	DeleteDC(hdcTemp);
	DeleteObject(hBitmap2); //release memory
	return _color;

}

/* INPUT:	(x,y) coordinates of pixel to be checked
OUTPUT:	returns the color at said coordinate */
unsigned int PixelHandler::GetPixelColor(POINT coord)
{
	unsigned int color = GetPixelColor(coord.x, coord.y);
	return color;

}


bool PixelHandler::VerifyPixelColor(unsigned int color, int x, int y)
{
	unsigned int pixColor = GetPixelColor(x, y);
	if (pixColor == color)
		return true;
	return false;
}

bool PixelHandler::VerifyPixelColor(unsigned int color, int x, int y, int tolerance)
{
	unsigned char red, green, blue, _red, _blue, _green;
	unsigned int pixColor = GetPixelColor(x, y);
	
	_red = (pixColor & 0xff000000) >> 24;
	_green = (pixColor & 0x00ff0000) >> 16;
	_blue = (pixColor & 0x0000ff00) >> 8;

	red = (color & 0xff000000)>>24;
	green = (color & 0x00ff0000)>>16;
	blue = (color & 0x0000ff00)>>8;
	//printf("color: %08x, pixColor: %08x\n", color, pixColor);
	//printf("Red:%3i, _Red:%3i\n", red, _red);
	if (((red < (_red + tolerance)) && (red >(_red - tolerance))) && ((green < (_green + tolerance)) && (green >(_green - tolerance))) && ((blue < (_blue + tolerance)) && (blue >(_blue - tolerance))))
		return true;
	return false;

}

bool PixelHandler::VerifyPixelColor(Pixel pix)
{
	unsigned int color;
	color = GetPixelColor(pix._x, pix._y);
	if (color == pix._color)
		return true;
	return false;
}

// Searches just around a certain pixel with a tolerance of 10 for a certain pixel
bool PixelHandler::VerifyRoughPixel(Pixel pix)
{
	return SearchPixelArea(pix._color, pix._x - 8, pix._y - 8, pix._x + 8, pix._y + 8, 15);
}

// Searches just around a certain pixel with a tolerance of 10 for a certain pixel
bool PixelHandler::VerifyRoughPixel(Pixel pix, int shade = 15)
{
	return SearchPixelArea(pix._color, pix._x - 8, pix._y - 8, pix._x + 8, pix._y + 8, shade);
}

//USED to find pixels on screen and get informations
unsigned int PixelHandler::DEBUG()
{
	POINT curMouse;
	COLORREF color;
	while (1)
	{
		GetCursorPos(&curMouse);
		color = GetPixelColor(curMouse.x, curMouse.y);
		printf("Color: %08x, found at (%4i, %4i)\n", color, curMouse.x, curMouse.y);
	}
}

/*====== DEPRECATED==========
unsigned int PixelHandler::WINGetPixelColor(int x, int y)
{
//stock windows function
return GetPixel(_hDC, x, y);
}
*/
