#include "controller.h"
#include <iostream>


void controller::dispatch(Cevent ev, vector<Worm>& Worms, graphic_movement& graphic_handler)
{
	switch (ev.eventType)
	{
	case PRESS_MOVE:
	{
		for (unsigned int i = 0; i < Worms.size(); i++)
		{
			Worms[i].uData = ev.ud;
			Worms[i].startMoving();
		}
	} break;
	case RELEASE_MOVE:
	{
		for (unsigned int i = 0; i < Worms.size(); i++)
		{
			Worms[i].uData = ev.ud;
			Worms[i].stopMoving();
		}
	} break;
	case PRESS_JUMP:
	{
		for (unsigned int i = 0; i < Worms.size(); i++)
		{
			Worms[i].uData = ev.ud;
			Worms[i].startJumping();
		}
	} break;
	case RELEASE_JUMP:
	{
		for (unsigned int i = 0; i < Worms.size(); i++)
		{
			Worms[i].uData = ev.ud;
			Worms[i].stopJumping();
		}
	} break;
	case REFRESH:
	{
		graphic_handler.flip_backgroundwoalpha();
		graphic_handler.flip_background();
		for (unsigned int i = 0; i < Worms.size(); i++)
		{
			Worms[i].update();
			//agregar fondo
			graphic_handler.do_walking_step(Worms[i]);
			graphic_handler.do_jumping_step(Worms[i]);

		}
		al_flip_display();
	} break;
	}
}

Cevent controller::getNextEvent(ALLEGRO_EVENT_QUEUE * event_queue)
{
	static Cevent events;
	if (al_get_next_event(event_queue, &evs))
	{

		switch (evs.type) {
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			events.eventType = QUIT;
			events.ud = 0;
			break;
		case ALLEGRO_EVENT_TIMER:
			events.eventType = REFRESH;
			events.ud = 0;
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			switch (evs.keyboard.keycode)
			{
			case P1_LEFT:
				events.eventType = PRESS_MOVE;
				events.ud = P1_LEFT;
				break;
			case P2_LEFT:
				events.eventType = PRESS_MOVE;
				events.ud = P2_LEFT;
				break;
			case P1_RIGHT:
				events.eventType = PRESS_MOVE;
				events.ud = P1_RIGHT;
				break;
			case P2_RIGHT:
				events.eventType = PRESS_MOVE;
				events.ud = P2_RIGHT;
				break;
			case P1_UP:
				events.eventType = PRESS_JUMP;
				events.ud = P1_UP;
				break;
			case P2_UP:
				events.eventType = PRESS_JUMP;
				events.ud = P2_UP;
				break;
			}
			break;
		case ALLEGRO_EVENT_KEY_UP:
			switch (evs.keyboard.keycode)
			{
			case P1_LEFT:
				events.eventType = RELEASE_MOVE;
				events.ud = P1_LEFT;
				break;
			case P2_LEFT:
				events.eventType = RELEASE_MOVE;
				events.ud = P2_LEFT;
				break;
			case P1_RIGHT:
				events.eventType = RELEASE_MOVE;
				events.ud = P1_RIGHT;
				break;
			case P2_RIGHT:
				events.eventType = RELEASE_MOVE;
				events.ud = P2_RIGHT;
				break;
			case P1_UP:
				events.eventType = RELEASE_JUMP;
				events.ud = P1_UP;
				break;
			case P2_UP:
				events.eventType = RELEASE_JUMP;
				events.ud = P2_UP;
				break;
			}
			break;
		}
	}
	else
	{
		events.eventType = NO_EVENT;
		events.ud = 0;
	}
	return events;
}
