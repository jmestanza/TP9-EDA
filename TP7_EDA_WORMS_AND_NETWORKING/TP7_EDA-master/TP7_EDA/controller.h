
#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include "Cevent.h"
#include <allegro5\allegro.h>
class controller
{
public:
	controller( char evw2[EVENTOS_PER_WORM], ALLEGRO_DISPLAY * display, int fps_= 50);
	~controller();
	Cevent*  getNextEvent(void);

private:
	Cevent ev;
	int fps;
	ALLEGRO_EVENT_QUEUE * cola;
	ALLEGRO_EVENT al_ev;
	ALLEGRO_TIMER * timer;
};

#endif //CONTROLLER_H_!