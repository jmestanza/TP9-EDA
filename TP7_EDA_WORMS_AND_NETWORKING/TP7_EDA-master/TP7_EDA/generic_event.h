//#define _WIN32_WINNT 0x0501

#ifndef GENERIC_EVENT
#define GENERIC_EVENT

#include "worm.h"
#include "Cevent.h"
#include "Output.h"
#include <iostream>

using namespace std;

class generic_event
{
public:
	virtual eventType getEventValue() = 0;
	virtual void show() = 0;
};

class move_pressed : public generic_event
{
public:
	virtual eventType getEventValue();
	virtual void show() {
		cout << "{'move pressed'}";
	};
	void setKeyValue(char key_t);
	char getKeyValue();
	int worm_id;
	move_pressed(Worm *worm_src);
	Worm *worm_p;
private:
	char key;
};

class move_released : public generic_event
{
public:
	virtual eventType getEventValue();
	virtual void show() {
		cout << "{'move released'}";
	};
	void setKeyValue(char key_t);
	char getKeyValue();
	move_released(Worm *worm_src);
	Worm *worm_p;
	int worm_id;
private:
	char key;
};

class jump_pressed : public generic_event
{
public:
	virtual eventType getEventValue();
	virtual void show() {
		cout << "{'jump pressed'}";
	};
	void setKeyValue(char key_t);
	char getKeyValue();
	jump_pressed(Worm *worm_src);
	Worm *worm_p;
	int worm_id;
private:
	char key;
};

class jump_released : public generic_event
{
public:
	virtual eventType getEventValue();
	virtual void show() {
		cout << "{'jump released'}n";
	};
	void setKeyValue(char key_t);
	char getKeyValue();
	jump_released(Worm *worm_src);
	Worm *worm_p;
private:
	char key;
};

class refresh : public generic_event
{
public:
	virtual eventType getEventValue();
	virtual void show() {};
	refresh(Worm *worm_src, viewer *view_src);
	Worm *worm_p;
	viewer *view_p;
};

class net_move_event : public generic_event {
private:
	char move_type;
	char touched;
public:
	net_move_event(char move_t, char touched_t) { move_type = move_t; touched = touched_t; }
	virtual eventType getEventValue() {
		return NET_MOVE_EVENT;
	}
	virtual void show() {
		cout << "{'move' , (" << move_type << ")}";
	}
	void set_move_type(char move) {
		move_type = move;
	}
	char get_move_type() {
		return move_type;
	}
	void set_move_state(bool m_state) {
		touched = m_state;
	}
	char get_move_state() {
		return touched;
	}
};
class net_ready_event : public generic_event {
private:
	int n1, n2;
public:
	net_ready_event(int _n1, int _n2) : n1(_n1), n2(_n2) {}
	virtual eventType getEventValue() {
		return NET_I_AM_READY;
	}
	virtual void show() {
		cout << "{'ready' , " << n1 << "," << n2 << "}";
	}
	int get_n1() {
		return n1;
	}
	int get_n2() {
		return n2;
	}
};

class net_quit_event : public generic_event {
public:
	virtual eventType getEventValue() {
		return NET_QUIT;
	}
	virtual void show() {
		cout << "{'quit'}";
	}
};
class net_error_event : public generic_event {
public:
	virtual eventType getEventValue() {
		return NET_ERROR;
	}
	virtual void show() {
		cout << "{'net error'}";
	}
};
class net_ack_event : public generic_event {
public:
	virtual eventType getEventValue() {
		return NET_ACK;
	}
	virtual void show() {
		cout << "{'ack'}";
	}
};

ostream &operator<<(ostream &stream, generic_event* ev);

#endif