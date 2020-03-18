#pragma once
#define PI 3.14159265358979323846
#define JUMP_VEL (4.5)

class Physics
{
private:
	
	
public:
	double gravity;
	double jumpAngle;
	double velocity;
	double Xmax;
	double Xmin;


	Physics(double gravity_ = 0.24, double jumpAngle_ = ((60.0*PI) / 180.0), double velocity_ = 27, double Xmax =1212, double Xmin = 701);
	double getGravity(void);
	double getJumpAngle(void);
	double getVelocity(void);
	double getxMax(void);
	double getxMin(void);
	//void updateWalk(Worm& worm);
	//void updateJump(Worm& worm);
};

