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

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <ctime>

#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#endif /* _WIN32 */

//#elif defined(__linux__) || defined(__unix__)
//#elif defined(__APPLE__)

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
	initialize_peers(const char*env[],const char*file_name_of_peers_trackers="peers_trakers.b");
	~initialize_peers()=default;

};

class peer{  // base class. It starts two threads internally -> send_thread and recv_thread
private:
	static constexpr int MAX_CHAR_PEER_ID=128;
	static char this_peer_id[MAX_CHAR_PEER_ID];
	char connected_peer[MAX_CHAR_PEER_ID];
protected:
	void send_thread(void){}
	void recv_thread(void){}
public:
	static void create_peer_id(const char*env[]);
};

class in_peer_factory{ // factory class that listens on the sockets and creates peer class when it accepts
private:
	std::vector<std::unique_ptr<std::thread>> peer_list;
protected:
	bool run(void){
		peer_list.emplace_back(new std::thread());
		return true;
	}
};

class out_peer_factory{// factory class that connects to an address and creates peer class on successful connection
private:
	std::vector<std::unique_ptr<std::thread>> peer_list;
protected:
	bool run(void){
		peer_list.emplace_back(new std::thread());
		return true;
	}
};

class address{// - container class handling ipv4 or ipv6 address and DNS
private:
	bool ipv4=true;
	struct sockaddr_in  ipv4_address;
	struct sockaddr_in6 ipv6_address;
public:
	bool is_ipv4(void){return ipv4;}
	address(){}
};
// use synchronous sockets
