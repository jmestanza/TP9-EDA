#include "Physics.h"
#include "Worm.h"

#include <cmath>

#define MOV 9

Physics::Physics(double gravity_, double jumpAngle_, double velocity_, double Xmax_ , double Xmin_ )
{
	gravity = gravity_;
	jumpAngle = jumpAngle_;
	velocity = velocity_;
	Xmax = Xmax_;
	Xmin = Xmin_;

}

double Physics::getGravity(void)
{
	return gravity;
}

double Physics::getJumpAngle(void)
{
	return jumpAngle;
}

double Physics::getVelocity(void)
{
	return velocity;
}

double Physics::getxMax(void)
{
	return Xmax;
}

double Physics::getxMin(void)
{
	return Xmin;
}

//void Physics::updateWalk(Worm& worm)
//{
//	position newPos;
//	if (worm.getSentido() == worm.left)
//	{
//		newPos.x = worm.getPos().x - MOV;
//	}
//	else
//	{
//		newPos.y = worm.getPos().y + MOV;
//	}
//	worm.setPos(newPos);
//}
//
//void Physics::updateJump(Worm& worm)
//{
//	position newPos;
//	newPos.x = worm.getPos().x + JUMP_VEL*cos(jumpAngle)*((double)worm.getFrameCount()); 
//	newPos.y = worm.getPos().y - JUMP_VEL*sin(jumpAngle)*((double)worm.getFrameCount()) + (0.5)*(gravity)*pow((worm.getFrameCount()), 2);
//	worm.setPos(newPos);
//}

