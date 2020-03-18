#include "stdafx.h"
#include "Cevent.h"
#include <iostream>
using namespace std;

Cevent::Cevent(eventType type_, userData ud_)
{
	type = type_;
	ud = ud_;

}

void Cevent::setEvent(char ev, int modo)
{
	bool foundedEv = false;
	
	if ((ev == EXIT) || (ev == HELP) || (ev == REFRESH) || (ev == NO_EVENT))
	{
		type = (int)ev;
	}
	else
	{
	//	cout << ev << endl;
		for (int i = 0; i < EVENTOS_PER_WORM && (!foundedEv); i++)  //recorro los eventos validos posibles
		{
			for (int j = 0; j < 2 && (!foundedEv); j++) //j se limita por el numero de worms(en este caso 2)
			{
				
				if (ev == validData[j][i])
				{
					foundedEv = true;
					//cout << "founded event" << endl;
					switch (i) //i me dice que evento fue, 0 representa izquierda, 1 representa derecha, 2 representa saltar
					{
					case 0:					//si se agregaran mas eventos posibles, solo bastaria con agregar mas casos al switch
					case 1:
						if (modo == PRESS)
						{
							type = TOUCHED_MOVEMENT;
						}
						else if (modo == LEAVE)
						{
							type = LEAVE_MOVEMENT;
						}
						break;
					case 2:
						if (modo == PRESS)
						{
							type = TOUCHED_JUMP;
						}
						else if (modo == LEAVE)
						{
							type = LEAVE_JUMP;
						}
						break;
					}
				}
			}

		}
		if (foundedEv)
		{
			ud = ev;
			//cout << "founded event!" << endl;

		}
		else
		{
			//cout << "reseteo ev.type" << endl;
			ud = '\0'; //significa que no hay eventos
			type = NO_EVENT;
		}
	}
}
void Cevent::setValidData(char evw2[EVENTOS_PER_WORM])
{
	for (int i = 0; i < EVENTOS_PER_WORM; i++) //relleno validData
	{
		validData[0][i] = evw2[i];
	}
}
int Cevent::getType(void)
{
	return type;
}
userData Cevent::getUd(void)
{
	return ud;
}

Cevent::~Cevent()
{
}
