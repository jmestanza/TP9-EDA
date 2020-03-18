#include "stdafx.h"
#include "network.h"
#include <iostream>
using namespace std;

typedef unsigned char uchar;

void process_event(char *buf, generic_event *ev,int *size) {
	if (ev->getEventValue() == NET_MOVE_EVENT) {
		buf[0] = 0x30;
		buf[1] = ((net_move_event*)ev)->get_move_type();
		buf[2] = ((net_move_event*)ev)->get_move_state();
		*size = 3;
	} else if (ev->getEventValue() == NET_I_AM_READY) {
		buf[0] = 0x20;
		int n1 = ((net_ready_event*)ev)->get_n1();
		int n2 = ((net_ready_event*)ev)->get_n2();
		//cout << "n1 = " << n1 << ' ' << "n2 = " << n2 << '\n';
		buf[1] = uchar(n1 / 256);
		buf[2] = uchar(n1 % 256);
		buf[3] = uchar(n2 / 256);
		buf[4] = uchar(n2 % 256);
		*size = 5;
	} else if (ev->getEventValue() == NET_QUIT) {
		buf[0] = 0x0E;
		buf[1] = '\0';
		*size = 2;
	} else if (ev->getEventValue() == NET_ERROR) {
		buf[0] = 0x0F;
		buf[1] = '\0';
		*size = 2;
	}else if(ev->getEventValue() == NET_ACK){
		buf[0] = 0x01;
		buf[1] = '\0';
		*size = 2;
	} else {
		cout << "invalid event detected, error !";
	}
}

generic_event *create_event(char *buf) {
	if (buf[0] == 0x30) {
		return (generic_event*)(new net_move_event(buf[1], buf[2]));
	} else if (buf[0] == 0x20) {
		int n1 = uchar(buf[1]) * 256 + uchar(buf[2]);
		int n2 = uchar(buf[3]) * 256 + uchar(buf[4]);
		return (generic_event*)(new net_ready_event(n1, n2));
	} else if (buf[0] == 0x0E) {
		return (generic_event*)(new net_quit_event());
	} else if (buf[0] == 0x0F) {
		return (generic_event*)(new net_error_event());
	} else if (buf[0] == 0x01) {
		return (generic_event*)(new net_ack_event());
	} else {
		cout << "invalid event was found ! \n";
		return NULL;
	}

}

network::network() {
	this->flag_mode = NO_SELECTED;
	this->err = 0;
}
int network::is_connected() {
	return this->connect_status;
}
void network::start_as_client(string host) {
	if (this->flag_mode != NO_SELECTED) {
		cout << "fatal error, trying to set as client when the mode is already set \n";
		this->err = 1;
		return; 
	}
	this->connect_status = 0;
	this->flag_mode = CLIENT;
	this->host = host;
	IO_handler = new boost::asio::io_service();
	socket = new boost::asio::ip::tcp::socket(*IO_handler);
	resolver = new boost::asio::ip::tcp::resolver(*IO_handler);

	cout << "client started \n";
}
void network::start_as_server() {
	if (this->flag_mode != NO_SELECTED) {
		cout << "fatal error, trying to set as server when the mode is already set \n";
		this->err = 1;
		return;
	}
	this->flag_mode = SERVER;
	IO_handler = new boost::asio::io_service();
	socket = new boost::asio::ip::tcp::socket(*IO_handler);
	acceptor = new boost::asio::ip::tcp::acceptor(*IO_handler,
	boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), DEFAULT_PORT));
	cout << "server started on " << DEFAULT_PORT << " created" << std::endl;
}
void network::connect() {
	if (this->flag_mode == CLIENT) {
		this->endpoint = resolver->resolve(
			boost::asio::ip::tcp::resolver::query(host.c_str(), HELLO_PORT_STR));
		try {
			boost::asio::connect(*this->socket, this->endpoint);
		} catch (std::exception const& ex) {
			cout << "could not connect" << '\n';
			this->connect_status = 0;
			return;
		}
		socket->non_blocking(true);
		this->connect_status = 1;
	} else if (this->flag_mode == SERVER) {
		acceptor->accept(*this->socket);
		socket->non_blocking(true);
	} else {
		cout << "error, flag mode not set, but connect function called \n";
	}
}
void network::send_event(generic_event *ev) {
	 if (this->flag_mode == CLIENT || this->flag_mode == SERVER){
		char buf[512];
		int buf_len = 0;
		process_event(buf, ev , &buf_len);
		//cout << "sending ";
		
		/*cout << "buf_len = " << buf_len << '\n';
		for (int i = 0; i < buf_len; i++) {
			cout << int(buf[i]) << ' ';
		}
		cout << '\n';*/
		//cout << "send = " << ev << '\n';
		size_t len;
		boost::system::error_code error;
		do {
			len = socket->write_some(boost::asio::buffer(buf,buf_len), error);
			buf[len] = '\0';
		} while ((error.value() == WSAEWOULDBLOCK));
		if (error) {
			cout << "error while trying to get information \n";
			this->err = 1;
		}
	} else {
		cout << "error , flag mode not set but, send event function called \n";
	}
}
void network::update() {
	if (this->flag_mode == CLIENT || this->flag_mode == SERVER) {
		// client waiting for messages

		boost::system::error_code error;
		char buf[512];
		size_t len = 0;

		
		len = socket->read_some(boost::asio::buffer(buf), error);
		buf[len] = '\0';

		if (!error && buf) {
			/*cout << "msg recv : ";
			size_t buf_len = len;
			for (int i = 0; i < buf_len; i++) {
				cout << int(buf[i]) << ' ';
			}
			cout << '\n';*/
			generic_event *ev = create_event(buf);
			//cout << "recv = " << ev << '\n';
			this->event_q.push(ev);
		}
	}
}
generic_event* network::get_next_event_in_queue() {
	if (this->event_q.empty()) return nullptr;
	generic_event* ev = this->event_q.front();
	//cout << "event = " <<  ev << '\n';
	this->event_q.pop();
	return ev;
}
network::~network() {
	if (this->flag_mode == NO_SELECTED) {
		// nothing to delete
	} else if (this->flag_mode == CLIENT) {
		delete this->IO_handler;
	//	delete this->socket;
		delete this->resolver;
		this->flag_mode = NO_SELECTED;
	} else if (this->flag_mode == SERVER) {
		delete this->IO_handler;
//		delete this->socket;
//		delete this->acceptor;
		this->flag_mode = NO_SELECTED;
	}
}

int network::is_error() {
	return this->err;
}