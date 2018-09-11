#pragma once
/*!
 * Copy from
 * https://raw.githubusercontent.com/fireice-uk/xmr-stak/master/xmrstak/net/jpsock.hpp
 * 2018/08/30/ 11:11:11
 */

#include "msgstruct.hpp"

#include <mutex>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <string>

class base_socket;

class jpsock{
public:
	jpsock(size_t id, const char* sAddr, bool tls, const char* tls_fp);
	~jpsock();

	bool connect(std::string& sConnectError);
	void disconnect(bool quiet = false);

	static bool hex2bin(const char* in, unsigned int len, unsigned char* out);
	static void bin2hex(const unsigned char* in, unsigned int len, char* out);

	inline size_t can_connect() { return get_timestamp() != connect_time; }
	inline bool is_running() { return bRunning; }
	inline size_t get_pool_id() { return pool_id; }
	inline bool get_disconnects(size_t& att, size_t& time) { att = connect_attempts; time = disconnect_time != 0 ? get_timestamp() - disconnect_time + 1 : 0; return pool && usr_login[0]; }
	inline const char* get_pool_addr() { return net_addr.c_str(); }
	inline const char* get_tls_fp() { return tls_fp.c_str(); }
	inline bool is_nicehash() { return nicehash; }

	bool get_pool_motd(std::string& strin);

	std::string&& get_call_error();
	bool have_call_error() { return call_error; }
	bool have_sock_error() { return bHaveSocketError; }

	inline static uint64_t t32_to_t64(uint32_t t) { return 0xFFFFFFFFFFFFFFFFULL / (0xFFFFFFFFULL / ((uint64_t)t)); }
	inline static uint64_t t64_to_diff(uint64_t t) { return 0xFFFFFFFFFFFFFFFFULL / t; }
	inline static uint64_t diff_to_t64(uint64_t d) { return 0xFFFFFFFFFFFFFFFFULL / d; }

	inline uint64_t get_current_diff() { return iJobDiff; }

	void save_nonce(uint32_t nonce);
	bool get_current_job(pool_job& job);

	bool set_socket_error(const char* a);
	bool set_socket_error(const char* a, const char* b);
	bool set_socket_error(const char* a, size_t len);
	bool set_socket_error_strerr(const char* a);
	bool set_socket_error_strerr(const char* a, int res);

private:
	std::string net_addr;
	std::string tls_fp;

	size_t peer_id;

	size_t connect_time = 0;
	std::atomic<size_t> connect_attempts;
	std::atomic<size_t> disconnect_time;

	std::atomic<bool> bRunning;
	std::atomic<bool> quiet_close;
	std::atomic<bool> call_error;

	uint8_t* bJsonRecvMem;
	uint8_t* bJsonParseMem;
	uint8_t* bJsonCallMem;

	static constexpr size_t iJsonMemSize = 4096;
	static constexpr size_t iSockBufferSize = 4096;

	struct call_rsp;
	struct opaque_private;
	struct opq_json_val;

	void jpsock_thread();
	bool jpsock_thd_main();
	bool process_line(char* line, size_t len);
	bool process_pool_job(const opq_json_val* params, const uint64_t messageId);
	bool cmd_ret_wait(const char* sPacket, opq_json_val& poResult, uint64_t& messageId);

	std::string sSocketError;
	std::atomic<bool> bHaveSocketError;

	std::mutex call_mutex;
	std::condition_variable call_cond;
	std::thread* oRecvThd;

	std::mutex job_mutex;
	pool_job oCurrentJob;

	opaque_private* prv;
	base_socket* sck;

	uint64_t iMessageCnt = 0;
	uint64_t iLastMessageId = 0;
};
