#include "stdafx.h"
#include "NAT.h"

NAT::NAT():
m_devcount(0), m_service_mutex(new std::mutex()),
m_target(-1)
{
}

NAT::~NAT()
{
	Uninstall();

	delete m_service_mutex;
	m_service_mutex = NULL;
}

ULONG NAT::Setup(NetInfo adapter)
{
	if (m_devcount > 0)return ERROR_SERVICE_ALREADY_RUNNING;

	char dev_name[MAX_ADAPTER_NAME_LENGTH + 30];
	// 用于发送数据包的适配器(名称)
	{
		USES_CONVERSION;
		sprintf_s(dev_name, sizeof(dev_name), "rpcap://\\Device\\NPF_%s", T2A(adapter.sAdapterName));
	}

	// 打开所有适配器
	char filter[] = "ip broadcast";
	struct bpf_program fcode;

	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_if_t * alldevs;

	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	{
		//fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", errbuf);
		CString err;
		err.Format(_T("Error in pcap_findalldevs_ex: %s\n"), CString(errbuf));
		MessageBox(NULL, err, NULL, MB_ICONERROR);
		return ERROR_CANT_READ_ALLDEV;
	}

	m_target = -1;

	for (pcap_if_t * d = alldevs; d; d = d->next)
	{
		//printf("%s\n    ", d->name);
		//if (d->description)
		//	printf(" (%s)\n", d->description);
		//else
		//	printf(" (No description available)\n");
		pcap_t * adhandle;
		if ((adhandle = pcap_open(d->name,
			65536,
			PCAP_OPENFLAG_PROMISCUOUS,
			-1,
			NULL,
			errbuf)) == NULL)
		{
			//printf("    **** FAIL ****\n");
			continue;
		}
		//printf("    opened\n");

		if (pcap_compile(adhandle, &fcode, filter, 1, 0xffffff) < 0)
		{
			pcap_close(adhandle);
			printf("Filter compile failed\n");
			continue;
		}
		if (pcap_setfilter(adhandle, &fcode) < 0)
		{
			pcap_close(adhandle);
			printf("SetFilter failed\n");
			continue;
		}

		if (strcmp(d->name, dev_name) == 0)
		{
			m_target = m_devcount;
			//printf("    ^^^ Sending interface found\n");
		}
		m_opened_dev[m_devcount] = adhandle;
		++m_devcount;
	}

	pcap_freealldevs(alldevs);

	if (m_devcount <= 0) return ERROR_NO_DEV;

	if (m_target < 0){
		for (int i = 0; i < m_devcount; i++){
			pcap_close(m_opened_dev[i]);
		}
		m_devcount = 0;

		return ERROR_INTERFACE_NOT_FOUND;
	}

	netInfo = adapter;

	//Create thread
	//std::thread && a = std::thread(NATThread, *this);
	m_service_thread = new std::thread(NATThread, *this);

	return NO_ERROR;
}

BOOL NAT::Uninstall()
{
	if (m_devcount <= 0)return FALSE;

	m_service_mutex->lock();

	for (int i = 0; i < m_devcount; i++){
		pcap_close(m_opened_dev[i]);
	}
	m_devcount = 0;
	m_target = -1;

	m_service_mutex->unlock();

	m_service_thread->join();

	delete m_service_thread;
	m_service_thread = NULL;

	return TRUE;
}

void FillCheckSum(u_char * data, bpf_u_int32 len, bpf_u_int32 pos)
{
	data[pos] = 0;
	data[pos + 1] = 0;
	bpf_u_int32 sum = 0;
	for (bpf_u_int32 i = 0; i < len; i += 2)
	{
		if (i + 1 < len)
			sum += (data[i] << 8) + data[i + 1];
		else
			sum += (data[i]) << 8;
	}
	sum = ~((sum >> 16) + (sum & 0xffff));
	data[pos] = (sum >> 8);
	data[pos + 1] = (sum & 0xff);
}

void broadcast(NAT& nat, pcap_t * fp, bpf_u_int32 len, const u_char * data)
{
	u_char * d = (u_char*)malloc(len);
	memcpy(d, data, len);
	memcpy(d, nat.netInfo.gatewayMac, 6);

	DWORD num_ip = 1;
	DWORD mask_t = SwapEndian(nat.netInfo.localMask);
	UINT8 mask_length;
	ConvertIpv4MaskToLength(nat.netInfo.localMask, &mask_length);
	mask_length = 32 - mask_length;
	if (mask_length > 10)
	{
		// Something must be wrong
		mask_length = 8;
	}
	for (int i = 0; i < mask_length; i++)
		num_ip <<= 1;
	DWORD local_ip = SwapEndian(nat.netInfo.localIp);
	DWORD ip_start = (SwapEndian(nat.netInfo.localIp) & SwapEndian(nat.netInfo.localMask)) + 1;
	DWORD ip_end = (SwapEndian(nat.netInfo.localIp) & SwapEndian(nat.netInfo.localMask)) + num_ip - 1;
	for (DWORD ip = ip_start; ip < ip_end; ip++)
	{
		if (SwapEndian(ip) == nat.netInfo.localIp)
			continue;
		d[26] = (local_ip >> 24) & 0xff;
		d[27] = (local_ip >> 16) & 0xff;
		d[28] = (local_ip >> 8) & 0xff;
		d[29] = (local_ip)& 0xff;
		d[30] = ip >> 24;
		d[31] = (ip >> 16) & 0xff;
		d[32] = (ip >> 8) & 0xff;
		d[33] = ip & 0xff;
		FillCheckSum(d + 14, 20, 10);
		// FillCheckSum(d + 14, len - 14, 26);
		d[40] = 0;
		d[41] = 0;
		if (pcap_sendpacket(fp, d, len) != 0)
		{
			fprintf(stderr, "\nError sending the packet: %s\n", pcap_geterr(fp));
		}
	}

	free(d);
}

void NATThread(NAT& nat)
{
	struct pcap_pkthdr *header;
	const u_char *pkt_data;

	while (1)
	{
		nat.m_service_mutex->lock();

		if (nat.m_devcount <= 0){
			nat.m_service_mutex->unlock();
			return;
		}

		for (int i = 0; i < nat.m_devcount; i++){
			int res = pcap_next_ex(nat.m_opened_dev[i], &header, &pkt_data);
			if (res > 0){

			}
		}

		nat.m_service_mutex->unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
