#include <iostream>
#include <ctime>
#include <vector>
#include <allegro5\allegro5.h>
#include <allegro5\allegro_image.h>

#include "Worm.h"
#include "Cevent.h"
#include "graphic_movement.h"
#include "controller.h"


const float FPS = 50.0;

using namespace std;

//TERMINAR
// calculos de fisica

int main(void)
{
	srand(time(NULL));
	controller control;

	//iniciliazacion de allegro

	ALLEGRO_DISPLAY * display = nullptr;
	ALLEGRO_EVENT_QUEUE *	event_queue = nullptr;
	ALLEGRO_TIMER * timer = nullptr;


	if (!al_init()) {
		return -1;
	}

	display = al_create_display(1920, 696);

	if (display == nullptr) {
		return -1;
	}

	if (!al_install_keyboard())
	{
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		return -1;
	}

	if (!al_init_image_addon()) { // ADDON necesario para manejo(no olvidar el freno de mano) de imagenes
		return -1;
	}

	event_queue = al_create_event_queue();
	if (!event_queue) {
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}

	al_start_timer(timer);

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	graphic_movement graphic_handler;

//	graphic_handler.create_images_arr();

	graphic_handler.init();
	graphic_handler.load_jump_imgs();
	graphic_handler.load_walk_imgs();
	graphic_handler.load_background();
	graphic_handler.flip_background();
	graphic_handler.load_backgroundwoalpha();

	// revisar cada al_draw_bitmap


	ALLEGRO_BITMAP * test=nullptr;

	//termino inicializacion de allegro

	vector<Worm> Worms(2);
	Worms[0].setKeys(P1_RIGHT, P1_LEFT, P1_UP);
	Worms[1].setKeys(P2_RIGHT, P2_LEFT, P2_UP); //inicializar con las def de allegro
	Cevent ev;
	do {
		ev = control.getNextEvent(event_queue);
		if (ev.eventType != NO_EVENT)
		{
			control.dispatch(ev, Worms, graphic_handler);
		}
	} while (ev.eventType != QUIT);

	//cierro allegro
	al_destroy_display(display);
	return 0;
}
