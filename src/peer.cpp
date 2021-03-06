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

//	https://en.wikipedia.org/wiki/Ascii85#ZeroMQ_Version_.28Z85.29
//	https://en.wikipedia.org/wiki/Binary-to-text_encoding
//	https://rfc.zeromq.org/spec:32/Z85/
//	https://github.com/zeromq/rfc/blob/master/src/spec_32.c
//	https://github.com/zeromq/libzmq
//	https://github.com/zeromq/libzmq/blob/master/include/zmq.h
//	https://github.com/zeromq/libzmq/blob/master/src/zmq.cpp

//	Because we do not care much about human reading of ID of an peer it is better to use another encoding like z85
//	It will be more compact 
//	However this prgram have about flaw about using unsigned char *  
//	It should be written as array of bytes of size_t hash of parameters


//	There are other more efficient encoding algorithms like 
//	http://blog.kevinalbs.com/base122
//	https://github.com/kevinAlbs/Base122
//	but it is not good because it contains non printing character
//	https://news.ycombinator.com/item?id=13049329

#include "yop2p/peer.hpp"
#include <functional>
#include <chrono>
#include <cassert>
#include <random>
#include <algorithm>
#include <iterator>

char peer::this_peer_id[peer::MAX_CHAR_PEER_ID]="";

void error_report(const char*msg){std::cerr<<msg<<std::endl;exit(0);}

//	https://en.wikipedia.org/wiki/Base58
//	http://lenschulwitz.com/base58
//	https://github.com/bitcoin/bitcoin/blob/master/src/base58.cpp
std::string EncodeBase58(const unsigned char* pbegin, const unsigned char* pend){
	static const char* pszBase58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
	// Skip & count leading zeroes.
	int length = 0;
	// Allocate enough space in big-endian base58 representation.
	int size = (pend - pbegin) * 138 / 100 + 1; // log(256) / log(58), rounded up.
	std::vector<unsigned char> b58(size);
	// Process the bytes.
	while (pbegin != pend) {
		int carry = *pbegin;
		int i = 0;
		// Apply "b58 = b58 * 256 + ch".
		for (std::vector<unsigned char>::reverse_iterator it = b58.rbegin(); (carry != 0 || i < length) && (it != b58.rend()); it++, i++) {
			carry += 256 * (*it);
			*it = carry % 58;
			carry /= 58;
		}
		assert(carry == 0);
		length = i;
		pbegin++;
	}
	// Skip leading zeroes in base58 result.
	std::vector<unsigned char>::iterator it = b58.begin() + (size - length);
	while (it != b58.end() && *it == 0)
		it++;
	// Translate the result into a string.
	std::string str;
	str.reserve(b58.end() - it);
	while (it != b58.end())
		str += pszBase58[*(it++)];
	return str;
}
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
	//	Getting BIOS info for most unix based systems http://www.nongnu.org/dmidecode/ 
	//	https://stackoverflow.com/questions/18328129/detect-hardware-information-in-c-application
	//	https://docs.microsoft.com/en-us/windows/desktop/sysinfo/getting-hardware-information
	//	http://win32easy.blogspot.com/2011/03/wmi-in-c-query-everyting-from-your-os.html
	//	https://stackoverflow.com/questions/5193625/howto-get-hardware-information-in-linux-using-c
	//	https://social.msdn.microsoft.com/Forums/en-US/75cb3bc2-e91c-463b-be59-ee4774f0b957/get-hardware-information
	//	Linux https://github.com/lpereira/hardinfo
	//	ryo-currency/src/crypto/cn_slow_hash.hpp  inline void cpuid(uint32_t eax, int32_t ecx, int32_t val[4])
	/*
	 * 	int32_t cpu_info[4];
		cpuid(1, 0, cpu_info);
		return (cpu_info[2] & (1 << 25)) != 0;
	*/
	//auto start_time = std::chrono::high_resolution_clock::now();
	auto start_time = std::chrono::system_clock::now();
	int i,j,k,n;
	size_t hashed_array[100];
	std::vector<size_t> v;
	//auto time_point = std::chrono::system_clock::now(); //https://stackoverflow.com/a/15778082/886607
	for(i=0;env[i]!=nullptr;i++)
		v.emplace_back(std::hash<const char*>{}(env[i]));
	n=i;
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(v.begin(), v.end(), g);//https://en.cppreference.com/w/cpp/algorithm/random_shuffle
	std::string strd1;
	i=j=0;//k=n<100?4:16;
	if(n>=8){
		for(auto m1: v){
			strd1 += std::to_string(m1);
			if(i++%(n/4) == (n/6) && j<10){
				hashed_array[j++]=std::hash<std::string>{}(strd1);
				strd1="";
			}
		}
		hashed_array[j++]=std::hash<std::string>{}(strd1);
	}
	else
		for(auto m1: v)
			hashed_array[j++]=std::hash<std::string>{}(std::to_string(m1));
	n=j;
	v.clear();
	//auto end_time = std::chrono::high_resolution_clock::now();
	//auto now_c = std::chrono::system_clock::to_time_t(std::chrono::time_point_cast<std::chrono::nanoseconds>(start_time));// https://stackoverflow.com/a/32556992/886607
	//auto now_c = std::chrono::system_clock::to_time_t(start_time);	std::cout	<<	std::ctime(&now_c) << std::endl;
	//char time_string[100];//https://www.programiz.com/cpp-programming/library-function/ctime/strftime
	//strftime(time_string, 50, "%y%m%d%H%M%S", std::localtime(&now_c));//http://www.cplusplus.com/reference/ctime/strftime/
	//std::cout<< time_string << " ff "<< start_time.time_since_epoch().count() << std::endl;
	std::string st1= std::to_string(start_time.time_since_epoch().count());//https://stackoverflow.com/a/42866624/886607
	//std::cout	<< st1 << "  " << st1.length() << " part1 "	<<	st1.substr(0,st1.length() /2)	
	//			<<	" part2 "	<<	st1.substr(st1.length() /2)	<<	std::endl;
	
	hashed_array[n++] = std::hash<std::string>{}(st1.substr(0,st1.length() /2));
	hashed_array[n++] = std::hash<std::string>{}(st1.substr(st1.length() /2));
	
	auto end_time = std::chrono::system_clock::now();
	//std::chrono::duration<long double> diff = end_time-start_time;
	hashed_array[n++] = std::hash<double>{}((end_time-start_time).count());

	std::uniform_real_distribution<> dis(0.0009, 999999.99);
	for(;n<12;n++)
		hashed_array[n]=std::hash<double>{}(dis(g));
	std::cout<< std::this_thread::get_id() << std::endl;
	std::cout<< std::thread::hardware_concurrency() << std::endl;
	//const unsigned char * str1 = static_cast<const unsigned char *>(yooUuid.c_str());
	//	https://stackoverflow.com/a/33523370/886607
	//const unsigned char * str1 = reinterpret_cast<unsigned char*>(const_cast<char*>(yooUuid.c_str()));
	const unsigned char * str1 = reinterpret_cast<unsigned char*>(const_cast<size_t*>(hashed_array));
	std::string yooUuid=EncodeBase58(str1,str1+(n*sizeof(size_t)-1));
	std::cout<<n<<"\t"<<yooUuid.length()<<"\t"<<yooUuid<<std::endl;
}
