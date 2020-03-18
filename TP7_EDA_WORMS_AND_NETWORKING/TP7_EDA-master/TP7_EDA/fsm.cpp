#include "stdafx.h"
#include "fsm.h"
#include "worm.h"
#include "event_generator.h"

fsm::fsm()
{
	state = new ST_worm(); 
}

int key_convert(char value) {
	if (value == DER) {
		return 'D';
	}
	if (value == IZQ) {
		return 'I';
	}
	if (value == UP) {
		return 'J';
	}
}

void fsm::dispatch(generic_event* ev, network* net, string mode,Worm *worms)
{
	//cout << ev << '\n';
	if (ev == NULL || ev == nullptr) return;
	

	switch (ev->getEventValue()) 
	{
	case TOUCHED_MOVEMENT:
	{
		move_pressed *mv = (move_pressed*)ev;

		//cout << "touched " << mv->getKeyValue() << ' ' << mv->getEventValue() << '\n';

		generic_event *ev = (generic_event*)new net_move_event(key_convert(mv->getKeyValue()),TOUCHED_MOVEMENT );
		net->send_event(ev);
		delete ev;

		state->onEV_move_pressed(mv);
		break;
	}
	case LEAVE_MOVEMENT:
		//cout << "leave " << ev << '\n';
	{
		move_released *mv = (move_released*)ev;
		generic_event *ev = (generic_event*)new net_move_event(key_convert(mv->getKeyValue()), LEAVE_MOVEMENT);
		net->send_event(ev);
		delete ev;

		state->onEV_move_released(mv);

		break;
	}
	case TOUCHED_JUMP:
	{
		jump_pressed *mv = (jump_pressed*)ev;
		generic_event *ev = (generic_event*)new net_move_event(key_convert(mv->getKeyValue()), TOUCHED_JUMP);
		net->send_event(ev);
		delete ev;

		state->onEV_jump_pressed(mv);
		break;
	}
	case LEAVE_JUMP:
	{
		jump_released *mv = (jump_released*)ev;

		generic_event *ev = (generic_event*)new net_move_event(key_convert(mv->getKeyValue()), LEAVE_JUMP);
		net->send_event(ev);
		delete ev;

		state->onEV_jump_released(mv);

		break;
	}
	case NET_ACK:
		state->onEV_net_ack((net_ack_event*)ev);
		break;
	case NET_ERROR:
		state->onEV_net_error((net_error_event*)ev);
		break;
	case NET_I_AM_READY:
		//cout << "dispatch = " << ev << '\n';
		state->onEV_net_im_ready((net_ready_event*)ev, net, mode,worms);
		break;
	case NET_MOVE_EVENT:

		//cout << "move = " << ev << '\n';
		//state->onEV_move_pressed(mv);
		state->onEV_net_move((net_move_event*)ev, worms);
		
		break;
	case NET_QUIT:
		state->onEV_net_quit((net_quit_event*)ev, net);
		break;
	case REFRESH:
		
		state->onEV_refresh((refresh*)ev);
		break;
	case EXIT:
		break;
	}
}

fsm::~fsm()
{
	delete state;
}
