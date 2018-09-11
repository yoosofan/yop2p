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

#include "yop2p/peer.hpp"
#include <functional>
#include <chrono>

char peer::this_peer_id[peer::MAX_CHAR_PEER_ID]="";

initialize_peers::initialize_peers(const char*env[], const char*file_name_of_peers_trackers){
	this->file_name_of_peers_trackers=file_name_of_peers_trackers;
	std::fstream peers_tracker_files;
	peers_tracker_files.open(file_name_of_peers_trackers,std::ios::in|std::ios::binary);
	bool content_of_file_is_loaded = false;
	if(!peers_tracker_files.is_open()){
		peers_tracker_files.open(file_name_of_peers_trackers,std::ios::out|std::ios::binary);
		if(!peers_tracker_files.is_open()){
			std::cout<< "error in opening peers_tracker_files"<<std::endl;
		}
		else
			peers_tracker_files.close();
	}else{
		//peers_tracker_files.read()
		peers_tracker_files.close();
	}
	peer::create_peer_id(env);
}

void peer::create_peer_id(const char*env[]){
	//	hash of start time + combine hash of env +  some random generator +
	//	hash mac address + hash ip address + hash pid + hash thread id + hash of diff time (start-end)

	auto start_time = std::chrono::high_resolution_clock::now();
	//auto time_point = std::chrono::system_clock::now(); //https://stackoverflow.com/a/15778082/886607
	for(int i=0;env[i]!=nullptr;i++)std::cout<<env[i]<<std::endl;
	auto end_time = std::chrono::high_resolution_clock::now();
	auto now_c = std::chrono::high_resolution_clock::to_time_t(std::chrono::time_point_cast<std::chrono::nanoseconds>(start_time));
	std::chrono::duration<long double> diff = end_time-start_time;
	std::cout	<< end_time.time_since_epoch().count() <<" d "<< diff.count() << "\ttime\t"<<std::ctime(&now_c) << std::endl;
	char time_string[100];//https://www.programiz.com/cpp-programming/library-function/ctime/strftime
	strftime(time_string, 50, "%y%m%d%H%M%S", std::localtime(&now_c));//http://www.cplusplus.com/reference/ctime/strftime/
	std::cout<< time_string << " ff "<< start_time.time_since_epoch().count() << std::endl;
	std::string st1= std::to_string(start_time.time_since_epoch().count());
	std::cout<< st1 <<std::endl;
}
