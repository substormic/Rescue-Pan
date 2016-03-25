#pragma once
#include "InterfaceInventory.h"
#include "InterfaceStats.h"

class BotMagicCurse
{
protected:
	int curseX = 4;
	int curseY = 1;
	POINT spell;
	POINT enemy;
	Mouse mouse;
	int spellTimeout = 15;

	InterfaceInventory inv;
	InterfaceStats stat;
	unsigned int PineapplePizza = 0xa5a30900;

public:
	void run()
	{
		//unsigned int VarrockEastGuardSkirt = 0xcc9c0c00;
		unsigned int VarrockEastGuardSkirt = 0xa1acad00;
		inv.SetMouseSpeed(0.4);
		stat.SetMouseSpeed(0.4);
		while (1)
		{
			inv.HandleHotkeys();
			inv.HandleAutoLogOut();

			if (spellTimeout <= 0) //gout out.
			{
				mouse.MouseMove(650, 40);
				Sleep(400);
				mouse.LeftClick();
				Sleep(400);
				mouse.LeftClick();
				Sleep(12000);
				inv.Logout();
				return;
			}

			if (!inv.AttemptToEatAtHp(PineapplePizza, 50))
			{
				mouse.MouseMove(650, 40);
				Sleep(400);
				mouse.LeftClick();
				Sleep(400);
				mouse.LeftClick();
				Sleep(12000);
				inv.Logout();
				return;
			}

			if (stat.CheckExperienceCircle(0x0006fe00, 1))
				spellTimeout = 15;

			//============== SELECT SPELL =================
			if (!stat.VerifyTopLeftText(HOVER_MAGIC) ||  (  (stat.VerifyTopLeftText(HOVER_MAGIC)) && (stat.VerifyTopLeftText(HOVER_NPC)) ) ) //if its not selected and not misinterpreting a level as a spell
			{
				/*
				spell = inv.GetSpellCoords(curseX, curseY); //get position of spell
				inv.VerifyActiveMagic(); //maake magic tab open if not already
				mouse.MouseMoveArea(spell.x, spell.y, spell.x + INV_MAGIC_SPELL_WIDTH, spell.y + INV_MAGIC_SPELL_HEIGHT); //grab spell
				*/
				inv.MoveToSpell(curseX, curseY);
				Sleep(200);
				//if (!stat.VerifyTopLeftText(HOVER_MAGIC)) //if its not selected
					mouse.LeftClick(); //select
			}

			 // ============== find enemy and cast ==================
			if (stat.VerifyTopLeftText(HOVER_MAGIC) && !(stat.VerifyTopLeftText(HOVER_NPC))) //if  spell is  selected, and not just finding a green level
			{
				if (stat.SearchEnemyBloom(VarrockEastGuardSkirt, 250, 150, 1300, 900))
				{
					Sleep(500);
					if (stat.ChooseMenuOptionColorCheck(0, HOVER_NPC))
					{
						mouse.LeftClick();
						Sleep(200);
					}
				}
			}
			spellTimeout--;
		}
		return;
	}

};