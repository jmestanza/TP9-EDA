#pragma once
#ifndef CEVENT_H_
#define CEVENT_H_

#define EVENTOS_PER_WORM 3 // move_izq, move_der, jump
// Modos de los eventos
#define PRESS 1
#define LEAVE 2
enum EVENTOS { EXIT = 1, HELP, REFRESH, LEAVE_MOVEMENT, TOUCHED_MOVEMENT, LEAVE_JUMP, TOUCHED_JUMP, NO_EVENT,
           	   NET_MOVE_EVENT, NET_I_AM_READY, NET_QUIT, NET_ERROR, NET_ACK
};

// Caracteres especiales para representar eventos

#define IZQ '1'
#define DER '2'
#define UP '3'

typedef int eventType;
typedef char userData;


class Cevent
{
public:
	Cevent(eventType type_ = NO_EVENT, userData ud_ = '\0');
	~Cevent();
	void setEvent(char ev, int modo);
	void setValidData(char evw2[EVENTOS_PER_WORM]);
	int getType(void);
	userData getUd(void);

private:
	eventType type;
	userData ud;
	//datos válidos
	char validData[2][EVENTOS_PER_WORM];
};


#endif // CEVENT_H_!

