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
int main(const int argc , const char *argv[], const char *env[]){
	initialize_peers ipeers(env);
	std::cout<<"hello world"<<std::endl;
	return 0;
}
