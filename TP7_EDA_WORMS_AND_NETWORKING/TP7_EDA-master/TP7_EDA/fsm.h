#include "generic_state.h"
#include "network.h"

#ifndef FSM_H
#define FSM_H

class fsm
{
public:
	fsm();
	void dispatch(generic_event* ev, network* net, string mode,Worm *worms); // Se define lo del switch y toda la cosa
	~fsm();
private:
	ST_worm* state;
};

#endif

