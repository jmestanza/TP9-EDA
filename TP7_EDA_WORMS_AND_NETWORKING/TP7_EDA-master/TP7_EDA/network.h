//#define _WIN32_WINNT 0x0501

#include "generic_event.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <queue>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>

using namespace std;

#ifndef NETWORK_H
#define NETWORK_H

#define DEFAULT_PORT 15667
#define HELLO_PORT_STR "15667"
enum {NO_SELECTED,CLIENT , SERVER};

/** network is able to work as a client or as a server **/

class network {
	public:
		network();
		~network();
		void start_as_server();
		void start_as_client(string host);
		void connect();
		void update();
		void send_event(generic_event *ev);
		generic_event* get_next_event_in_queue(); // events must be destroyed manually after used by user

		int is_error();
		int is_connected();
	private:
		int connect_status;
		int flag_mode; // configuration of mode
		int err;
		string host;

		/// boost variables
		boost::asio::io_service* IO_handler;
		boost::asio::ip::tcp::socket* socket;
		boost::asio::ip::tcp::resolver* resolver;
		boost::asio::ip::tcp::resolver::iterator endpoint;
		boost::asio::ip::tcp::acceptor* acceptor;

		queue <generic_event*> event_q;
};

#endif
