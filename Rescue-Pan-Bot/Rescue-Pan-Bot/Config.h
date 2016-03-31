#pragma once
#include "Password.h" //runescape password for logging in

/*
/		Coordinates assume running OSBuddy runscape client FULLSCREEN on a 1920x1080 monitor
/
/		!WARNING! - all derived classes' coordinates are derived from the three Coordinates declared below (pixStats, pixChat, pixInv)
/		MAKE SURE THESE COORDINATES ARE VALID!!
/
/		Additionally, for multiple monitor use set your SCREEN variable as either LEFT_SCREEN or RIGHT_SCREEN
/		If only using one monitor, set SCREEN to LEFT_SCREEN.
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_



//HOTKEY THINGS
const int ESCAPE = 0x1b; //escape hotkey in hex  
#define PAUSE_PROGRAM 1
#define EXIT_PROGRAM 2

struct fPoint {
	float x;
	float y;
};

struct Area {
	int x1;
	int x2;
	int y1;
	int y2;

	Area()
	{
		x1 = x1 = y1 = y2 = 0;
	}
	Area(int _x1, int _y1, int _x2, int _y2)
	{
		x1 = _x1;
		x2 = _x2;
		y1 = _y1;
		y2 = _y2;
	}
};

struct Pixel {
	unsigned int _color;   //assign colors in HEX (RR GG BB AA)  -  RGBA format (typically alpha is 00)
	int _x;
	int _y;

	Pixel()
	{
		_x = _y = _color = 0;
	}
	Pixel(unsigned int color, int x, int y)
	{
		_x = x;
		_y = y;
		_color = color;
	}
	void Set(unsigned int color, int x, int y)
	{
		_x = x;
		_y = y;
		_color = color;
	}
}; // ============= MAKE SURE THESE ARE VALID ===========
// pixStats(0xf1f5f600,1483 + SCREEN,55), pixChat(0x4a5a6000,25 + SCREEN,877), pixInv(0xeaeafb00,1489 + SCREEN,720);
static const Pixel pixStats(0xf6f5f100, 1483 + SCREEN, 55);
static const Pixel pixChat(0x605a4a00, 25 + SCREEN, 877); 
static const Pixel pixInv(0xfbeaea00, 1489 + SCREEN, 720);
static const Pixel compass(0x9f0c0e00, 1483 + SCREEN, 55); //color of furthermost red pixel on arm, and the pixel coords of center of compass
static const Pixel MiniMapCenter(0xffffff00, 1561+SCREEN, 117);


//Create STATS pixel to verify window (uses center white pixel on compass)
//Create INVENTORY pixel to verify inv. (uses center whtie pixel on quests icon)
//Create CHAT pixel to verify chat window (uses dark pixel on stud in top left corner arrow on chat window - hard to see)

#define PI 3.14159265
#define COMPASS_ARM 6 //length of the compass arm to furthermost red pixel

#define HOVER_NPC  0xffff0000//monster hover text color - YELLOW
#define HOVER_ITEM 0xff904000 //item hover text color - ORANGE
#define HOVER_ACTION  0x00ffff00//action hover text color - BLUE
#define HOVER_MAGIC 0x00ff0000//Magic Spell hover text color - GREEN

#define MENU_HEADER 20//pixel height of "choose option"
#define MENU_OPTION 15//pixel height of each option
#define MENU_MINWIDTH 50//minimum pixel width of half the menu. mouse starts in center above choose option

#define INV_ITEM_HEIGHT 32//height of an item
#define INV_ITEM_WIDTH 32 //width of an item
#define INV_ITEM_HORIZ_SPACER 10//pizel difference horizontally between items
#define INV_ITEM_VERT_SPACER 4//pixel difference vertical between items
#define INV_MAGIC_SPELL_HEIGHT 24 //height of a spell
#define INV_MAGIC_SPELL_WIDTH 24 //height of a spell

#define MAGIC_TELE 0x8e8df300 //color of teleport ringsi n every tele spell
#define MAGIC_ALCH 0xbcc2c000 //color of green bottle in high alch
#define MAGIC_HEAT 0xb0100200 //color of red fire part in superheat spell
#define MAGIC_ENTANGLE 0x04af0400 //color of entangle

#define BANK_CLOSE_WIDTH 25 //width of close button
#define BANK_CLOSE_HEIGHT 22 //height of close button
#define BANK_TAB_WIDTH 35 //width of the bank tabs
#define BANK_TAB_HEIGHT 31 //height of the bank tabs
#define BANK_TAB_SPACER 5 //pixel width of dead space between tabs
#define BANK_ITEM_WIDTH 35 //pixel height of item
#define BANK_ITEM_HEIGHT 31 //pixel height of item
#define BANK_ITEM_SPACER_WIDTH 13 //pixel dead spce between items
#define BANK_ITEM_SPACER_HEIGHT 5 //pixel dead space between items

#define WITHDRAW_1 0 //withdraw 1 item menu option 0
#define WITHDRAW_5 1 //withdraw 5 item menu option 1
#define WITHDRAW_10 2 //withdraw 10 item menu option 2
#define WITHDRAW_LASTX 3 //withdraw last x item menu option 3
#define WITHDRAW_X 4 //withdraw x item menu option 4
#define WITHDRAW_ALL 5 //withdraw all item menu option 5
#define WITHDRAW_ALLBUTONE 6 //withdraw all but 1 item menu option 6

#define LEFT 1		//arrow keys for the keyboard arrow function
#define RIGHT 2		//arrow keys for the keyboard arrow function
#define UP 3		//arrow keys for the keyboard arrow function
#define DOWN 4		//arrow keys for the keyboard arrow function

#define DOT_PLAYER 0xececec00 //color of player dot in minimap

static const Pixel SUPERHEAT(MAGIC_HEAT, 4, 3); //creates the spell using its active color as well as its spellbook coords
static const Pixel TELEBLOCK(MAGIC_TELE, 5, 8); //creates a spell for teleblock
static const Pixel ENTANGLE(MAGIC_ENTANGLE, 1, 8); //create a spell entangle

const int GAME_CENTER_X = 821 + SCREEN; //center of gameplay window X coord
const int GAME_CENTER_Y = 504 + SCREEN; //center of gameplay window Y coord

static const Area MiniMap(1484 + SCREEN, 40, 1637 + SCREEN, 194);

#endif
