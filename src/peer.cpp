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

#include<yop2p/peer.hpp>
 
initialize_peers::initialize_peers(const char*file_name_of_peers_trackers){
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
}
