//! First sample
/*!
 * 
 * 
 * \author Ahmad Yoosofan <yoosofan@mail.com>
 * \version 0.0.1
 * \date 2018
 * \copyright GPL v3 http://www.gnu.org/licenses/gpl.html
 * This file is part of yop2p project
 */

#include<iostream>
#include<fstream>
#include<vector>
#import<boost/asio>
 
class initialize_peers{
	private:
	static constexpr char const *  list_of_hardcoded_trackers[]={
		"http://yoosofan.github.io/trackers.b",
		"http://yoosofan.kashanu.ac.ir/trackers.b"
	};
	static constexpr char const * list_of_hardcoded_full_nodes_ip[]={
		""
	};
	static constexpr char const * list_of_hardcoded_routers_and_trackers[]={
		""
	};

	std::vector<std::string> list_of_read_from_file_trackers;
	std::vector<std::string> list_of_read_from_file_full_nodes_ip;
	std::vector<std::string> list_of_read_from_file_routers_and_trackers;

	std::string file_name_of_peers_trackers;

	public:
	initialize_peers(const char*file_name_of_peers_trackers="peers_trakers.b");
	~initialize_peers()=default;

};

boost::asio::io_context io_context;


class peer{  // base class. It starts two threads internally -> send_thread and recv_thread
private:
protected:
	send_thread(){}
	recv_thread(){}
};
	
class in_peer_factory{ // factory class that listens on the sockets and creates peer class when it accepts
private:
protected:
	send_thread(){}
	recv_thread(){}
};

class out_peer_factory{// factory class that connects to an address and creates peer class on successful connection
private:
protected:
	send_thread(){}
	recv_thread(){}
};

class address{// - container class handling ipv4 or ipv6 address and DNS
private:
	boost::asio::ip::address address,
public:

};
// use synchronous sockets
/*
 * That's why you will need two threads

You might want to look int thdq.hpp for a thread-safe queue
Use raw socket interfaces, but wrap then so they work on both linux and windows
You mean two set of socket for windows and linux
You can just #define so that you have a common interface between windows and linux
See how I did it in xmr-stak for reference, but you will need to extend it since you need to both accept and listen
*/
