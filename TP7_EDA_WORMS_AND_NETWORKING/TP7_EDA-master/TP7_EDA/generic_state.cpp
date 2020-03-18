#include "stdafx.h"
#include "generic_state.h"
#include "worm.h"
#include "event_generator.h"

generic_state * ST_worm::onEV_move_pressed(move_pressed *p)
{
	(p->worm_p[0]).start_moving(p->getKeyValue());
	return NULL;
}

generic_state * ST_worm::onEV_move_released(move_released *p)
{
	(p->worm_p[0]).stop_moving(p->getKeyValue());
	return NULL;
}

generic_state * ST_worm::onEV_jump_pressed(jump_pressed *p)
{
	(p->worm_p[0]).start_jumping(p->getKeyValue());
	return NULL;
}

generic_state * ST_worm::onEV_jump_released(jump_released *p)
{
	(p->worm_p[0]).stop_jumping(p->getKeyValue());
	
	return NULL;
}

generic_state * ST_worm::onEV_refresh(refresh * p)
{

	for (int i = 0; i < 2; i++) {
		(p->worm_p[i]).update();
		p->view_p->UpdateDisplay(p->worm_p, 2);
		p->view_p->flipViewer();
	}
	return NULL;
}

generic_state * ST_worm::onEV_net_move(net_move_event* nm_ev, Worm* p)
{
	//cout << "net move \n";
	switch (nm_ev->get_move_type()){
	case 'D':
		
		if (nm_ev->get_move_state() == TOUCHED_MOVEMENT){
			(p[1]).start_moving(DER);
		}
		else
		{
			(p[1]).stop_moving(DER);
		}
		break;
	case 'I':
		if (nm_ev->get_move_state() == TOUCHED_MOVEMENT)
		{
			(p[1]).start_moving(IZQ);
		}
		else
		{
			(p[1]).stop_moving(IZQ);
		}
		break;
	case 'J':
		if (nm_ev->get_move_state() == TOUCHED_JUMP)
		{
			(p[1]).start_jumping(UP);
		}
		else
		{
			(p[1]).stop_jumping(UP);
		}
		break;
	/*case 'T':
		if ((p->worm_p[1]).get_sentido() == RIGHT) {
			(p->worm_p[1]).set_sentido(LEFT);
		}
		else
		{
			(p->worm_p[1]).set_sentido(RIGHT);
		}
		break;*/
	default:
		break;
	}
	return nullptr;
}

generic_state * ST_worm::onEV_net_im_ready(net_ready_event* nr_ev, network* net, string mode,Worm *worms)
{
	if (!mode.compare("server"))
	{
		cout << "LLEGO EL I AM READY AL SERVER" << endl;
		cout << nr_ev << '\n';

		generic_event* server_ready_ev = (generic_event*)new net_ready_event(100, 200); // my worm
		worms[0].set_position(100, 200); // my worm
		worms[1].set_position(nr_ev->get_n1(), nr_ev->get_n2()); // client worm

		net->send_event(server_ready_ev); // I_AM_READY message
		delete server_ready_ev;

		/*generic_event* ack_ev = (generic_event*)new net_ack_event();
		net.send_event(ack_ev); // Server responses with ACK
		delete ack_ev;
		cout << "SE ENVIO EL ACK" << endl;*/
	}
	else if (!mode.compare("client"))
	{
		//cout << "llego un I am ready al client" << '\n';
		cout << "LLEGO EL I AM READY AL CLIENT" << endl;
		//cout << nr_ev << '\n';
		worms[1].set_position(nr_ev->get_n1(), nr_ev->get_n2());
		
		//worms[1].set_position(nr_ev->get_n1(), nr_ev->get_n2());

		//generic_event* client_ready_ev = (generic_event*)new net_ready_event(900, 1);
		//net.send_event(client_ready_ev); // Client responses with I_AM_READY
		//delete client_ready_ev;
	}

	return NULL;
}

generic_state * ST_worm::onEV_net_quit(net_quit_event * p, network* net)
{
	cout << ">> QUIT trig received" << endl;
	generic_event* ack_ev = (generic_event*)new net_ack_event;
	net->send_event(ack_ev);
	delete ack_ev;
	cout << ">> Closed conection" << endl;
	cout << ">> Program ended Ok.-" << endl;
	return NULL;
}

generic_state * ST_worm::onEV_net_error(net_error_event * p)
{
	cout << ">> Transmission error" << endl;
	cout << ">> Conection closed" << endl;
	cout << ">> Program terminated.-" << endl;
	return NULL;
}

generic_state * ST_worm::onEV_net_ack(net_ack_event * p)
{
	cout << ">> Server ACK received" << endl;
	cout << ">> Running Worms" << endl;
	//cout << p << '\n';

	return NULL;
}




