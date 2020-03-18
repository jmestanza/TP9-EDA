#pragma once

#include "Cevent.h"
#include "graphic_movement.h"
#include "Worm.h"

#include <vector>

#define  P1_LEFT	ALLEGRO_KEY_A
#define P1_RIGHT	ALLEGRO_KEY_D
#define P1_UP		ALLEGRO_KEY_W
#define P2_LEFT		ALLEGRO_KEY_LEFT
#define P2_RIGHT	ALLEGRO_KEY_RIGHT
#define P2_UP		ALLEGRO_KEY_UP

class controller
{
public:
	void dispatch(Cevent ev, vector<Worm>& Worms, graphic_movement& graphic_handler);
	Cevent getNextEvent(ALLEGRO_EVENT_QUEUE * event_queue);
private:
	ALLEGRO_EVENT evs;
};

