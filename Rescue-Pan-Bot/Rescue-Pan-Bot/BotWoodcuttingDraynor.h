#pragma once
#include"InterfaceGeneral.h"
#include"InterfaceInventory.h"
#include"InterfaceBank.h"


class BotWoodcuttingDraynor {
private:
	int negativeScreen = SCREEN - 1920;
	//if on screen right, this is zero (so I can input coords relative to SR)
	//if on screen left, it'll take SR coords and make them apply to SL

	unsigned int treeColorFromBank = 0x8A623200;
	unsigned int treeColorFromTree = 0x32391600;
	Area treeAreaFromBank = Area(negativeScreen + 3260, 350, negativeScreen + 3437, 670);
	Area treeAreaFromTree = Area(negativeScreen + 2840, 500, negativeScreen + 2950, 530);

	int logsBanked = 0;

	bool treeDead() {
		POINT a = mouse.GetPosition();
		if (a.x == negativeScreen + 2859 && a.y == 575) {
			//no
		}
		else {
			mouse.MouseMove(negativeScreen + 2859, 575);
		}
		Sleep(50);
		return !InterfaceGeneral().VerifyTopLeftText(0x00DDDD00);
	}
	bool treeDead(int rMax) {
		Area a;
		a.x1 = negativeScreen + 2857;
		a.y1 = 562;
		a.x2 = negativeScreen + 2862;
		a.y2 = 576;
		mouse.MouseMoveArea(a);
		Sleep(50);
		return !InterfaceGeneral().VerifyTopLeftText(0x00DDDD00);
	}


public:

	Mouse mouse;

	bool ClickTree(unsigned int treeColor, Area region, int walkSleepTime) {
		int timeOutTimer = 0;
		bool foundTree = PixelHandler().SearchPixelArea(treeColor, region.x1, region.y1, region.x2, region.y2, 20);
		while (!foundTree)
		{
			//todo: zero map up
			printf("Can't find tree! waiting 3 seconds\n");
			Sleep(3000);
			timeOutTimer++;
			if (timeOutTimer > 2) {
				return false;
			}
		}

		POINT destination = PixelHandler().SearchPixelAreaForPoint(treeColor, region.x1, region.y1, region.x2, region.y2, 20);
		mouse.MouseMove(destination);
		Sleep(40);
		/* can be commented >>> */
		timeOutTimer = 0;
		//printf("Testing top left for tree");
		while (!InterfaceGeneral().VerifyTopLeftText(0x00DDDD00)) {
			timeOutTimer++;
			Sleep(1000);
			destination = PixelHandler().SearchPixelAreaForPoint(treeColor, region.x1, region.y1, region.x2, region.y2, 20);
			mouse.MouseMove(destination);
			if (timeOutTimer > 20) {
				printf("Couldn't verify that the tree was really there :/ \n");
				return false;
			}
		}
		/*can be commented <<< */
		mouse.RightClick();
		Sleep(50);
		if (!InterfaceGeneral().ChooseMenuOptionColorCheck(0, 0x00FFFF00)) {
			printf("Tree wasn't where we thought. Restarting\n");
			ClickTree(treeColor, region, walkSleepTime);
			return true;
		}
		mouse.LeftClick();
		Sleep(walkSleepTime);
		return true;
	}


	bool chopTree() {
		InterfaceInventory inv;
		InterfaceGeneral gen;
		while (true)
		{
			int timeoutTimer = 0;
			gen.HandleAutoLogOut();
			inv.VerifyActiveInventory();

			//waiting for you to fell the tree
			while (!treeDead())
			{
				timeoutTimer++;
				if (inv.VerifyInventoryFull(0x7A572C00)) {
					printf("\nInventory is full.\n");
					return true;
				}
				if (timeoutTimer > 40) {
					printf("Tree's not dying, did you level up? Trying again\n");
					if (!ClickTree(treeColorFromTree, treeAreaFromTree, 0)) {
						printf("Tree somehow didn't die or something...?\n");
						return false;
					}
					else {
						timeoutTimer -= 20;
					}
				}
				Sleep(1000);
			}

			timeoutTimer = 0;
			printf("Tree down. ");
			//wait for tree to come back
			while (treeDead()) {
				Sleep(200);
				timeoutTimer++;
				gen.HandleAutoLogOut();
				//todo: zero map up
				if (timeoutTimer > 160) {
					printf("Not finding the tree... trying 2 more times\n");
					if (!treeDead(2)) {
						printf("Phew, found it.\n");
						break;
					}
					Sleep(8000);
					if (!treeDead(2)) {
						printf("ok that was really close.\n");
						break;
					}
					printf("Tree ain't comin' back \n");
					return false;
				}
			}
			/*mouse.RightClick();
			InterfaceGeneral().ChooseMenuOptionColorCheck(1, 0x00DDDD00);*/
			ClickTree(treeColorFromTree, treeAreaFromTree, 0);
		}
	}

	bool bankLogs() {
		InterfaceBank ban;
		int timeoutTimer = 0;

		Area a;
		int bankBoxSize = 50;
		a.x1 = (2162 - 1920) - bankBoxSize;
		a.y1 = 550 - bankBoxSize;
		a.x2 = (2162 - 1920) + bankBoxSize;
		a.y2 = 550 + bankBoxSize;

		if (!ban.OpenBank(a)) {
			return false;
		}
		ban.DepositBackpack();
		Sleep(100);
		ban.CloseBank();
		Sleep(300);
		return true;

	}

	bool runWithDrop() {
		InterfaceInventory inv;
		InterfaceGeneral gen;
		while (inv.VerifyInventoryOpen())
		{
			gen.HandleAutoLogOut();
			if (!(ClickTree(treeColorFromTree, treeAreaFromTree, 0))) {
				return false;
			}

			if (!chopTree()) {
				return false;
			}
			inv.DropAllItems();
			logsBanked += 28;
			printf("Ran an inventory. Cut %d logs\n", logsBanked);
		}
		return true;
	}

	bool runWithBank() {
		InterfaceInventory inv;
		InterfaceGeneral gen;
		while (inv.VerifyInventoryOpen())
		{
			gen.HandleAutoLogOut();
			if (!(ClickTree(treeColorFromBank, treeAreaFromBank, 5500))) {
				return false;
			}
			if (!chopTree()) {
				return false;
			}
			if (!bankLogs()) {
				return false;
			}
			logsBanked += 28;
			printf("Ran an inventory. Banked %d logs\n", logsBanked);
		}
	}

	void run() {
		printf("Started.\n");
		//runWithDrop();

		runWithBank();
		//fallback mode:
		runWithDrop();

		for (int i = 0; i < 20; i++) {
			printf("##############################################\n");
		}
	}
};