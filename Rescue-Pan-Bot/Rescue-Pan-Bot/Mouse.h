#pragma once
#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "Windows.h" //mouse things
#include "stdlib.h" //srand and rand
#include "Config.h" //Area
#include <math.h> //sin function

class Mouse {
private:
	float MouseSpeed = 1.0; //Speed range: 0.1 -> 1.0
	int deviation = 250; //amount of arc path deviation  (E.g. 250pixels  will deviate the path in an arc randomly maxing at (-250 to 250)
public:
	//Moves mouse to an random pixel within area
	bool MouseMoveArea(int x1, int y1, int x2, int y2);

	//Moves mouse to an random pixel within area
	bool MouseMoveArea(Area region);

	//Deviates Randomly!! mouse move - recieves pixel coordinates. change mouse position to those coords EXACTLY (!WARNING! SAME PIXEL CLICK DANGER)
	bool MouseMove(int x, int y);

	//same as above, just takes in a point
	bool MouseMove(POINT coord);


	//same as above, just takes in a point
	bool MouseMoveRoughly(POINT coord, int variance);

	//same as above, just takes in a point
	bool MouseMoveRoughly(int x, int y, int variance);


	//STRAIGHT LINE DISTANCE mouse move - recieves pixel coordinates. change mouse position to those coords
	bool MouseMoveSLD(int x, int y);

	//right clicks lol
	void RightClick();

	//left clicks lol
	void LeftClick();

	//returns x,y of mouse
	POINT GetPosition();

	//changes mouse speed
	void ChangeSpeed(float speed);

	//changes path deviations. Deviation Min is minimum amount deviated, deviation is the random fluctuations from that minimum point. 
	//E.G. deviationMin = 250, deviation = 500. The path will deviate the path from (-250,250) pixels
	void SetDeviation(int dev);
};
#endif