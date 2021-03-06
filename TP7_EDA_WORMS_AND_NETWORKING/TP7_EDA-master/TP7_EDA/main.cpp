// TP7_EDA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "worm.h"
#include "Output.h"
#include "fsm.h"
#include "network.h"
#include "event_generator.h"

using namespace std;

#define ERROR_RETURN -1

const float FPS = 24;
const int SCREEN_WIDTH = 1920/2;
const int SCREEN_HEIGHT = 696/2;

int main()
{
	cout << "-------------------------" << endl;
	cout << "| TP7 - Worms & Network |" << endl;
	cout << "-------------------------" << endl;
	cout << ">> Select mode (server/client, CASE SENSISTIVE!):" << endl;
	string mode;
	cin >> mode;

	network Net;

	char keysEvWorm[EVENTOS_PER_WORM] = { IZQ, DER, UP };
	char keysEvWorm_network[EVENTOS_PER_WORM] = { IZQ , DER ,UP };

	Physics physics;
	physics.gravity = 0.24 * 500;
	physics.jump_angle = 60;
	physics.jump_speed = 2.4 * 50;
	physics.speed = 27;
	physics.max_coordinates.x = 700; //1212
	physics.max_coordinates.y = 600; //800
	physics.min_coordinates.x = 50; //701
	physics.min_coordinates.y = 0; //0
	physics.TIME_PER_UPDATE = (1.0 / FPS);

	if (!al_init())
	{
		cout << "allegro failed \n";
		return ERROR_RETURN;
	}

	Worm worms[2];

	for (int i = 0; i < 2; i++) {
		worms[i].set_physics(physics);
		worms[i].set_id(i + 1);
		cout << worms[i].get_id() << endl;
	}
	worms[0].set_keys(keysEvWorm);
	worms[1].set_keys(keysEvWorm_network);

	viewer viewTest(SCREEN_WIDTH, SCREEN_HEIGHT, 1);

	if (!(viewTest.IsInitOK()))
	{
		cout << "error init test \n";
		return ERROR_RETURN;
	}

	controller contTest(keysEvWorm, viewTest.GetDisplay(), FPS);
	event_generator eg(worms, &viewTest, &contTest, &Net);
	fsm D;

	

	eg.mode_selected(mode);

	if (!mode.compare("server")) 
	{
		Net.start_as_server();
		Net.connect(); // Try to connect to client
	
		//generic_event* server_ready_ev = (generic_event*)new net_ready_event(700, 1);
		//Net.send_event(server_ready_ev); // I_AM_READY message
		//delete server_ready_ev;

		do
		{
			eg.search_for_events();

			if (eg.is_event())
			{
				D.dispatch(eg.getNextEvent(), &Net, mode, worms); // Net and mode are only used at start
			}
			//al_rest(0.01);
		} while (!eg.check_if_close());
	}
	else if (!mode.compare("client"))
	{
		Net.start_as_client("localhost");
		Net.connect();

		if (Net.is_error()) {
			cout << ">> Error while trying to connect to server" << endl;
			cout << ">> Program terminated.-" << endl;
		}else{
			worms[0].set_position(200, 200);
			generic_event* client_ready_ev = (generic_event*)new net_ready_event(200, 200); // my worm
			Net.send_event(client_ready_ev); // I_AM_READY message
			delete client_ready_ev;

			do
			{
				eg.search_for_events();

				if (eg.is_event())
				{
					D.dispatch(eg.getNextEvent(), &Net, mode,worms); // Net and mode are only used at start
				}
				//al_rest(0.01);
			} while (!eg.check_if_close());
		}
	}
	else
	{
		cout << ">> Invalid or wrong mode typed" << endl;
		cout << ">> You must enter 'server' or 'client' without quotes" << endl;
		cout << ">> Program terminated.-" << endl;
	}

    return 0;
}