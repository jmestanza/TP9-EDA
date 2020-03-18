#ifndef GENERIC_STATE_H
#define GENERIC_STATE_H
#include "generic_event.h"
#include "network.h"
#include "worm.h"

class generic_state
{
public:
	virtual generic_state* onEV_move_pressed() { return nullptr; };
	virtual generic_state* onEV_move_released() { return nullptr; };
	virtual generic_state* onEV_jump_pressed() { return nullptr; };
	virtual generic_state* onEV_jump_released() { return nullptr; };
	virtual generic_state* onEV_refresh() { return nullptr; };
	virtual generic_state* onEV_net_move() { return nullptr; };
	virtual generic_state* onEV_net_im_ready() { return nullptr; };
	virtual generic_state* onEV_net_quit() { return nullptr; };
	virtual generic_state* onEV_net_error() { return nullptr; };
	virtual generic_state* onEV_net_ack() { return nullptr; };
};

class ST_worm : public generic_state
{
public:
	virtual generic_state* onEV_move_pressed(move_pressed* p);
	virtual generic_state* onEV_move_released(move_released* p);
	virtual generic_state* onEV_jump_pressed(jump_pressed* p);
	virtual generic_state* onEV_jump_released(jump_released* p);
	virtual generic_state* onEV_refresh(refresh* p);
	virtual generic_state* onEV_net_move(net_move_event* nm_ev, Worm* p);
	virtual generic_state* onEV_net_im_ready(net_ready_event* nr_ev, network* net, string mode,Worm* worms);
	virtual generic_state* onEV_net_quit(net_quit_event* p, network* net);
	virtual generic_state* onEV_net_error(net_error_event* p);
	virtual generic_state* onEV_net_ack(net_ack_event* p);
};

#endif // GENERIC_EVENT_H








/*
class ST_monitor_move : public generic_state
{
	// depende del tiempo, tiene que pasar a move o volver a idle
	virtual generic_state* onEV_move_pressed(move_pressed* p); // definir para pasar a move
	virtual generic_state* onEV_move_released(jump_released* p); // definir para pasar a idle
	ST_monitor_move();
	~ST_monitor_move();
};

class ST_move : public generic_state
{
	// se mueve o vuelve a idle
	virtual generic_state* onEV_move_pressed(move_pressed *p); // definir para seguir en move
	virtual generic_state* onEV_move_released(jump_released* p); // definir para pasar a idle
	ST_move();
	~ST_move();
};

class ST_jump : public generic_state
{
	virtual generic_state* onEV_jump_released(jump_released* p); // salta y cuando termina vuelve a idle
	ST_jump();
	~ST_jump();
};
*/