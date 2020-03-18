// robot.cpp: define el punto de entrada de la aplicación de consola.
//

//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "robots.h"

typedef struct {
	bool * baldosa;
	unsigned int height;
	unsigned int width;
}pisotype;

typedef struct {
	robottype *robots;
	unsigned int robotcount;
	pisotype * piso;
	unsigned int width;
	unsigned int height;
	unsigned long tickcount;
}simtype;


int main()
{

    return 0;
}



/*añadir create floor, get robot */


