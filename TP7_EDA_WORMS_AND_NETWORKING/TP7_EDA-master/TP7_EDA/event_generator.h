#include "generic_event.h"
#include "Cevent.h"
#include "controller.h"
#include "network.h"
#include "worm.h"
#include "Output.h"
#include <string>
#include <list>
#include <allegro5\allegro.h>

#ifndef EVENT_GENERATOR_H
#define EVENT_GENERATOR_H

using namespace std;

class event_generator
{
public:
	bool search_for_events();
	void shaper_allegro_evs();
	void shaper_net_evs();
	bool is_event(void);
	bool check_if_close(void);
	void mode_selected(string mode_t);
	generic_event* getNextEvent();
	event_generator(Worm *worm, viewer *view, controller *ref_control, network *ref_net);
private:
	Cevent *ref_ev;
	Worm *worm_p;
	viewer *view_p;
	controller *ref_control_p;
	network *ref_net_p;
	string mode;

	move_pressed mp;
	move_released mr;
	jump_pressed jp;
	jump_released jr;
	refresh r;
	net_move_event nm_ev;
	net_ready_event *nr_ev;
	net_quit_event nq_ev;
	net_error_event ne_ev;
	net_ack_event nack_ev;
	bool event_flag;
	bool close_prog;
	bool quit_trig;
	queue <generic_event*>eventList; // Se implemente cola (FIFO)
	//list <generic_event*>::iterator it;
	generic_event* aux_ev_p; // Auxiliar para pasar evento
};

#endif