#pragma once

#include <thread>
#include <mutex>

#define ERROR_CANT_READ_ALLDEV 16000L
#define ERROR_NO_DEV 16001L
#define ERROR_INTERFACE_NOT_FOUND 16002L

class NAT
{
private:
	int m_devcount;
	pcap_t * m_opened_dev[100];
	int m_target;
	std::thread * m_service_thread;
	std::mutex m_service_mutex;
	NetInfo netInfo;

	friend void NATThread(NAT* nat);
	friend void broadcast(NAT* nat, pcap_t * fp, bpf_u_int32 len, const u_char * data);
public:
	NAT();
	~NAT();
	ULONG Setup(NetInfo adapter);
	BOOL Uninstall();
};
