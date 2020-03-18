#include "stdafx.h"
#include "event_generator.h"

bool event_generator::search_for_events()
{
	bool ret = false;
	bool finish = false;
	//while (!finish) 
	//{
		event_flag = false;
		 // Update allegro events
		
		ref_ev = ref_control_p->getNextEvent();
		
		if (ref_ev != NULL) {
			shaper_allegro_evs();
			event_flag = true;
		}
		ref_net_p->update();
		aux_ev_p = ref_net_p->get_next_event_in_queue(); // Get event from net queue
		if ((aux_ev_p != NULL) && (aux_ev_p->getEventValue() == NET_ACK))
		{
			cout << "LLEGO EL ACK AL CLIENT SEARCH" << endl;
		}
		
		if (aux_ev_p != NULL)
		{
			shaper_net_evs();
			event_flag = true;

		}
		
		if(!event_flag)
		{
			finish = true;
		}
	//}

	return ret;
}

void event_generator::shaper_allegro_evs()
{
	switch (ref_ev->getType())
	{
	case TOUCHED_MOVEMENT:
		{
			mp.setKeyValue(ref_ev->getUd());
		
			eventList.push(&mp);

			break;
		}
	case LEAVE_MOVEMENT:
		mr.setKeyValue(ref_ev->getUd());
		eventList.push(&mr);
		break;
	case TOUCHED_JUMP:
		jp.setKeyValue(ref_ev->getUd());
		eventList.push(&jp);
		break;
	case LEAVE_JUMP:
		jr.setKeyValue(ref_ev->getUd());
		eventList.push(&jr);
		break;
	case REFRESH:
		eventList.push(&r);
		break;
	case EXIT: // Creates quit event
		generic_event* quit_ev = (generic_event*)new net_quit_event;
		ref_net_p->send_event(quit_ev);
		delete quit_ev;
		quit_trig = true;
		break;
	}
}

void event_generator::shaper_net_evs()
{
	switch (aux_ev_p->getEventValue())
	{
	case NET_MOVE_EVENT:
		nm_ev.set_move_type(((net_move_event*)aux_ev_p)->get_move_type());
		nm_ev.set_move_state(((net_move_event*)aux_ev_p)->get_move_state());
		eventList.push((net_ready_event*)aux_ev_p);
		break;
	case NET_I_AM_READY:
		eventList.push((net_ready_event*)aux_ev_p);
		break;
	case NET_QUIT: // Receives quit trig
		eventList.push((net_ready_event*)aux_ev_p);
		close_prog = true;
		break;;
	case NET_ERROR: // Receives error
		eventList.push((net_ready_event*)aux_ev_p);
		break;
	case NET_ACK:
		if (quit_trig)
		{
			close_prog = true;
		}
		else
		{
			cout << "ack !" << '\n';
			eventList.push((net_ready_event*)aux_ev_p);
		}
		break;
	default: // Creates error event
		generic_event* net_error = (generic_event*)new net_error_event;
		ref_net_p->send_event(net_error);
		delete net_error;
		close_prog = true;
		break;
	}
}

bool event_generator::is_event(void)
{
	return (event_flag);
}

bool event_generator::check_if_close(void)
{
	return close_prog;
}

void event_generator::mode_selected(string mode_t)
{
	mode = mode_t;
}

generic_event* event_generator::getNextEvent()
{
	/*it = eventList.begin();
	aux_ev_p = *it;
	eventList.pop_front();*/
	if (eventList.empty()) {

		event_flag = false;
		return NULL;
	}
	generic_event* top_element = eventList.front();
	eventList.pop();

	//cout << eventList.size() << '\n';
	return top_element;
}

event_generator::event_generator(Worm *worm, viewer *view, controller *ref_control, network *ref_net)
	: mp(worm), mr(worm), jp(worm), jr(worm), r(worm, view), nm_ev(IZQ, 0) {
	worm_p = worm;
	view_p = view;
	event_flag = false;
	close_prog = false;
	quit_trig = false;
	ref_control_p = ref_control;
	ref_net_p = ref_net;
	aux_ev_p = nullptr;
}


