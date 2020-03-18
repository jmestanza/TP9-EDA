#pragma once
#include <allegro5\allegro5.h>
#include <allegro5\allegro_image.h>
#include <iostream>
#include "imgs_catalog.h"
using namespace std;

class ev_handler
{
private:
	int disp_width;
	int disp_height;
	int FPS;
	unsigned int num_pressed;
	bool compress_finish;

	ALLEGRO_EVENT evs;
	ALLEGRO_DISPLAY * display;
	ALLEGRO_EVENT_QUEUE *	event_queue;
	ALLEGRO_TIMER * timer;
	bool exit;
	string key_pressed;
	bool toggle_key;
	bool flush_var;
public:
		
	ev_handler(int disp_w, int disp_h, int FPS_);
	void process_evs(imgs_catalog& cat);
	bool is_exit();
	bool get_next_allegro_ev();
	bool start_and_reg(void);
	~ev_handler();
};

