#include "stdafx.h"
#include "controller.h"
#include <iostream>
using namespace std;


controller:: controller(char evw2[EVENTOS_PER_WORM], ALLEGRO_DISPLAY * display, int fps_)
{
	fps = fps_;
	timer = al_create_timer(1.0 / fps);
	ev.setValidData(evw2);
	al_install_mouse();
	al_install_keyboard();
	cola = al_create_event_queue();
	al_register_event_source(cola, al_get_display_event_source(display));
	al_register_event_source(cola, al_get_keyboard_event_source());
	al_register_event_source(cola, al_get_timer_event_source(timer));
	al_start_timer(timer);

}

Cevent* controller::getNextEvent(void)
{
	if (al_get_next_event(cola, &al_ev))
	{
		if (al_ev.type == ALLEGRO_EVENT_TIMER)
		{
			ev.setEvent(REFRESH, 0);
		}
		else if (al_ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
		//	cout << "voy a poner exit, quieren cerrar el display" << endl;
			ev.setEvent(EXIT, 0);
		}
		else if (al_ev.type == ALLEGRO_EVENT_KEY_UP)
		{

			//cout << "key released \n";
			if (al_ev.keyboard.keycode == ALLEGRO_KEY_LEFT)
			{
				ev.setEvent(IZQ, LEAVE);
			}
			else if (al_ev.keyboard.keycode == ALLEGRO_KEY_RIGHT)
			{
				ev.setEvent(DER, LEAVE);
			}
			else if (al_ev.keyboard.keycode == ALLEGRO_KEY_UP)
			{
				ev.setEvent(UP, LEAVE);
			}
			else
			{
				ev.setEvent(al_ev.keyboard.keycode + 'a'-1, LEAVE);
			}
		}
		else if (al_ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			//cout << "key pressed \n";
			if (al_ev.keyboard.keycode == ALLEGRO_KEY_H)
			{
				ev.setEvent(HELP, 0);
			}
			else if (al_ev.keyboard.keycode == ALLEGRO_KEY_LEFT)
			{
				ev.setEvent(IZQ, PRESS);
			}
			else if (al_ev.keyboard.keycode == ALLEGRO_KEY_RIGHT)
			{
				ev.setEvent(DER, PRESS);
			}
			else if (al_ev.keyboard.keycode == ALLEGRO_KEY_UP)
			{
				
				ev.setEvent(UP, PRESS);
			}
			else
			{
				ev.setEvent(al_ev.keyboard.keycode + 'a'-1, PRESS);
			}
			
		}
		else
		{
			//cout << "reseteo event desde controller" << endl;
			ev.setEvent(NO_EVENT, 0);
		}
	}
	else
	{
		ev.setEvent(NO_EVENT, 0);
	}

	return &ev;
}

controller::~controller()
{
	al_destroy_timer(timer);
	al_destroy_event_queue(cola);
}
