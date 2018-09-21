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

#pragma once

/*
 * Based on https://cplusplus.github.io/networking-ts/draft.pdf
 * and other resources it seems the future standard version of C++
 * will use boost.asio for standard C++ network
 * Therefore, it is better to use this library right now
 * Instead of writing a new handcrafted network packages
 * https://en.cppreference.com/w/cpp/experimental
 * https://www.meetup.com/gbgcpp/events/247503186/
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4656.pdf
 * https://isocpp.org/std/status
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <ctime>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif /* _WIN32 */

//#elif defined(__linux__) || defined(__unix__)
//#elif defined(__APPLE__)

// This function must be changed
void error_report(const char*msg);

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
	//
	//sockaddr_in6 ipv6_address={0}; //https://stackoverflow.com/a/11684260/886607
	int port=19987;
	std::string ip="127.0.0.1";
	int sockfd=-1;
	//sockaddr *sa;//https://stackoverflow.com/a/18579605/886607
public:

	int get_socket(void){return sockfd;}

	bool is_ipv4(void){return ipv4;}
	
	// github cpp-sockets
	address(struct sockaddr_in addr) {
		this->port = addr.sin_port;

		char ip1[INET_ADDRSTRLEN];
		inet_ntop(addr.sin_family, &(addr.sin_addr), ip1, INET_ADDRSTRLEN);

		ip = std::string(ip1);
	}

	address(std::string ip, int port) {
		this->ip = ip;
		this->port = port;
	}
	
	address()
	{
		this->ipv4=true;
		//
		//
		this->port=19987;
		this->ip="127.0.0.1";
		this->sockfd=-1;
	}
	
	~address()
	{
		if(this->sockfd>=0)
			close(this->sockfd);
		this->sockfd=-1;
	}
	struct sockaddr_in get_struct() {
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof addr);

		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);

		inet_aton(ip.c_str(), &addr.sin_addr);

		return addr;
	}
	int get_port() {
		return port;
	}

	std::string get_ip() {
		return ip;
	}
	
	bool create_socket(void){
		this->sockfd =  socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd<0){
			error_report("Error: Cannot open socket");
			return false;
		}
		return true;
	}

	bool bind_socket(void){
		if(ipv4){
			sockaddr_in  ipv4_address={0};
			//std::memset(ipv4_address.sin_zero,'\0',sizeof(ipv4_address.sin_zero));
			std::memset((void*)&ipv4_address,'\0',sizeof(ipv4_address));
			// memset
			//https://stackoverflow.com/questions/17096990/why-use-bzero-over-memset/17097088
			//https://www.quora.com/In-C-what-is-the-difference-between-bzero-and-memset
			//http://maintainablecode.logdown.com/posts/159916-memcpy-memmove-and-memset-are-deprecated
			//http://developers-club.com/posts/118716/
			//https://en.cppreference.com/w/cpp/algorithm/fill
			//http://www.cplusplus.com/reference/algorithm/fill/
			ipv4_address.sin_family = AF_INET;
			ipv4_address.sin_addr.s_addr = INADDR_ANY;
			ipv4_address.sin_port = htons(port);
			//https://www.bogotobogo.com/cplusplus/sockets_server_client.php
			//https://cboard.cprogramming.com/networking-device-communication/91189-cplusplus-sockets.html
			//https://codereview.stackexchange.com/questions/98775/tcp-socket-wrapper
			if (bind(sockfd, (struct sockaddr *) &ipv4_address, sizeof(ipv4_address)) < 0)
				  error_report("ERROR on binding");
		}
		else
		{
			sockaddr_in6 ipv6_address={0}; //https://stackoverflow.com/a/11684260/886607
		}
		//std::cout<<"heere"<<std::endl;
		return true;
	}
};
// use synchronous sockets
