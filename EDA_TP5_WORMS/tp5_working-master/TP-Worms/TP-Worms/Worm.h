#pragma once

#define MOV 9
#include "Physics.h"

enum { IDLE, MOVING, MONITOR_MOVING, END_MOVEMENT, JUMPING, END_JUMPING };

struct position
{
	double x;
	double y;
};

class Worm
{
public:
	Worm();
	void setKeys(char right_,char left_, char up_);
	void startMoving(void);
	void stopMoving(void);
	void startJumping(void);
	void stopJumping(void);
	void update(void);
	char uData;
	position getPos(void);
	void setPos(position newPos);
	unsigned int getState(void);
	unsigned int getFrameCount(void);
	bool getSentido(void);
	const bool left = false;
	const bool right = true;
	bool key_down;


private:
	Physics physics_data;
	position pos;
	position old_pos;
	bool sentido;
	unsigned int frameCount;
	unsigned int state;
	const double xMax = 1212;
	const double xMin = 707;

	struct data
	{
		char up;
		char left;
		char right;
	};

	data keys;

};

