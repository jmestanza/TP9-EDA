#include <stdio.h>
#include <stdint.h>
#include <cstdint>
#include "model.h"
#include "graphic_simulation.h"
#include "allegro5\allegro.h"
#define ALLEGRO_STATICLINK
#include "allegro5\allegro_primitives.h"
#include "histogram.h"
#include "ParserFunc.h"
#include "utils.h"
#include "allegro5/allegro_image.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>
#include "uData.h"
#define _USE_MATH_DEFINES

enum{DEMO,REALTIME,FULLSIM};
#define ON 1	//Flag states.
#define OFF 0
#define ERR -1

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FPS = 40;
const int PERIOD = 10;

void set_positions(model_t *model, graphic_simluation_t *sim);
void update_simulation_from_model(model_t *model, graphic_simluation_t *sim);

int 
real_time_sim(userDataType* uData, ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer);
/**
*	real_time_sim - RobotCleaner - Luciano Castrogiovanni 
*		This function is the motor for mode 1 of the cleaner robots program.		
*
**/

int 
full_simulation(userDataType* uData, ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_TIMER *timer);
/**
*	Callback - RobotCleaner - Luciano Castrogiovanni 
*		This function is the motor for mode 2 of the cleaner robots program.
*
**/

int
Callback(char *key, char *value, void *userData);

/**
*	Callback - RobotCleaner - Luciano Castrogiovanni 
*		This function recieves arguments sorted as a -key with a value by a parser and interprets
*	them passing the value to the respectful owner as well as stopping single parameters or
*   duplicated keys or values with a respectful error message and exit.
**/

int
CheckParameters(userDataType *uData);

/**
*	Check Parameters - RobotCleaner - Luciano Castrogiovanni 
*		This function recieves the user defined values structure after it has been filled by the
*	Callback function and validates each value on its range and respective fractal types checking
*	for incompatibilities. If found, the function exits with a respective error message.
**/