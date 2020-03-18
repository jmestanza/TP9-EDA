#include <stdio.h>
#include <stdlib.h>
#include "robots.h"
// el random de esta funcion debe ser semilleada y corregir el rand de abajo
robottype * createRobot(unsigned int cant) {
	robottype * robots = NULL;
	robots = (robottype*)malloc(sizeof(robottype)* cant);
	if (robots != NULL) {
		for (int i = 0; i < cant; i++) {
			robots[i].pos.x = rand();
			robots[i].pos.y = rand();
		}
	}
}

void moveRobot(robottype * robot, unsigned int  height, unsigned int width) {

}

void destroyRobots(robottype * robots) {
	free(robots);
}
