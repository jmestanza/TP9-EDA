#include "ev_handler.h"

ev_handler::ev_handler(int disp_w,int disp_h, int FPS_)
{
	toggle_key = 0;
	flush_var = 0;
	key_pressed = "INIT";
	num_pressed = -1;
	compress_finish = false;
	disp_width = disp_w;
	disp_height = disp_h;
	FPS = FPS_;

	int exit = 0;
	ALLEGRO_DISPLAY * display = nullptr;
	ALLEGRO_EVENT_QUEUE *	event_queue = nullptr;
	ALLEGRO_TIMER * timer = nullptr;
}

void ev_handler::process_evs(imgs_catalog& cat)
{
	switch (evs.type) {
	case ALLEGRO_EVENT_DISPLAY_CLOSE:
		exit = 1;
		break;
	case ALLEGRO_EVENT_KEY_UP:
		
		if (toggle_key == 1) {
			toggle_key = 0;
			num_pressed = -1;
			key_pressed = "RESET";
		}
		break;
	case ALLEGRO_EVENT_KEY_DOWN:
		if (toggle_key == 0) {
			key_pressed = al_keycode_to_name(evs.keyboard.keycode);
			toggle_key = 1;
			flush_var = 1;
			if ((evs.keyboard.keycode >= ALLEGRO_KEY_1) && (evs.keyboard.keycode <= ALLEGRO_KEY_9))
			{
				num_pressed = (ALLEGRO_KEY_9 - ALLEGRO_KEY_0) - (ALLEGRO_KEY_9 - evs.keyboard.keycode);
			}
		}
		break;
	}	
	if (flush_var == 1) {
		if (!compress_finish)
		{
			if (!key_pressed.compare("RIGHT"))
			{
				cat.next_page();
			}
			if (!key_pressed.compare("LEFT"))
			{
				cat.previus_page();
			}
			if (!key_pressed.compare("ENTER"))
			{
				cat.compress_all();
				compress_finish = true;
			}
			if (!key_pressed.compare("a") || !key_pressed.compare("A"))
			{
				cat.select_page();
			}
			if (!key_pressed.compare("n") || !key_pressed.compare("N"))
			{
				cat.un_select_page();
			}

			if (num_pressed != -1)
			{
				cat.toogle_img_state(num_pressed);
			}
		}

		flush_var = 0;
	}
}

bool ev_handler::is_exit()
{
	return exit;
}

bool ev_handler::get_next_allegro_ev()
{
	return al_get_next_event(event_queue, &evs);
}

bool ev_handler::start_and_reg(void)
{
	display = al_create_display(disp_width, disp_height);
	if (display == nullptr) {
		return 1;
	}
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		return 1;
	}
	event_queue = al_create_event_queue();
	if (!event_queue) {
		al_destroy_display(display);
		al_destroy_timer(timer);
		return 1;
	}

	al_start_timer(timer);
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	return false;
}


ev_handler::~ev_handler()
{
	al_destroy_display(display);
	al_destroy_timer(timer);
}
