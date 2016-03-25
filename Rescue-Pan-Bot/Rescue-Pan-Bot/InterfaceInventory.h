#pragma once
#include "InterfaceGeneral.h"

class InterfaceInventory : public InterfaceGeneral
{
protected:
	 POINT InvStart; //inventory with relation to the relative inventory pixel
	 POINT SpellStart; //where the spells start relative to inv pixel

public:
	InterfaceInventory(); //constructor

	//Check inventory open
	bool VerifyInventoryOpen();

	//checks inventory is open, if not, then opens it.
	void VerifyActiveInventory();

	//check if magics is open
	bool VerifyMagicOpen();

	//checks Magic tab is open, if not, then opens it.
	void VerifyActiveMagic();

	//check if combatstyle is open
	bool VerifyCombatStyleOpen();

	//checks Magic tab is open, if not, then opens it.
	void VerifyActiveCombatStyle();

	//give the x,y coord of spell, get coords of that spell. Zero Indexed - 1st spell (0,0) 2nd @ (1,0) ...
	POINT GetSpellCoords(int spellX, int spellY);

	//Gets Top Left coordinates of any inventory space ZERO INDEXED
	POINT GetItemCoords(int itemIndex);

	//given book x,y zero indexed position of spell. moves mouse there
	void MoveToSpell(int spellX, int spellY);

	//moves to any pixel on an item given its index
	void MoveToItem(int itemIndex);

	//moves to any pixel on an item given its color
	bool MoveToItem(unsigned int color);

	//given an index and a color, verify item is there
	bool VerifyItem(unsigned int color, int index);

	//check if spell has runes. Given index of spell and a color to verify
	bool CheckSpellRunes(unsigned int color, int spellX, int spellY);

	//given a pixel containing the active color of a spell, and its spell coords, set up said spell
	bool PrepareSpell(Pixel spell);

	//Drop an item smartly
	bool DropItem(int itemIndex);

	//drop items within range, Drops all items between and including start and end
	void DropItemRange(int start, int end);

	//drops every item in inventory
	void DropAllItems();

	//given a color, drop all instances colored that
	void DropItemsColored(unsigned int color);

	// checks slot 28 to see if inventory is full
	bool VerifyInventoryFull();

	// checks slot 28 against a color to see if inventory is full
	bool VerifyInventoryFull(unsigned int color);

	//give color, and index, returns true if found in index, false if not
	bool SearchIndexForColor(int invindex, unsigned int color);

	//given color, returns the item index of the item. returns -1 if no item.
	int InterfaceInventory::SearchInventoryForItem(unsigned int color);

	//attempts to eat an item of given color. returns false if thers is no such item
	bool AttemptToEat(unsigned int color);

	//attempts to eat an item of given color after some HP marker has been hit. returns false if thers is no such food
	bool AttemptToEatAtHp(unsigned int color, int HpAmount);

};