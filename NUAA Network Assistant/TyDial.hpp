//天翼chap拨号补丁 by YeahO_O Yuan
//Modified by Noisyfox
#pragma once

#ifndef TYDIAL_HPP_
#define TYDIAL_HPP_

#include "md5.h"
#include "pcap.h"
#include <ctime>

#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <winsock.h>
#endif

class TyDial
{
public:
	bool flag_cancel;

	TyDial()
	{
		fp = NULL;
	}

	~TyDial()
	{
		Clean();
	}

	bool Init(char * pcap_dev_name)
	{
		if (fp != NULL)return false;

		// 打开 interface

		if ((fp = pcap_open(pcap_dev_name, 
			65536, 
			PCAP_OPENFLAG_PROMISCUOUS, 
			1, // read_timeout
			NULL,	// auth
			errbuf)
			) == NULL)
		{
			return false;
		}

		// 编译应用过滤条件

		struct bpf_program fcode;
		if (pcap_compile(fp, 
			&fcode, 
			"pppoes",	// 过滤条件
			1,	// 开启优化
			0)	// 掩码，由于这个过滤条件和广播无关，可以用 0 代替
			< 0)
		{
			printf("Compile error: %s\n", pcap_geterr(fp));
			Clean();
			return false;
		}
		if (pcap_setfilter(fp, &fcode) < 0)
		{
			printf("Set filter error: %s\n", pcap_geterr(fp));
			Clean();
			return false;
		}

		return true;
	}

	bool Clean()
	{
		if (fp == NULL) return true;

		pcap_close(fp);
		fp = NULL;

		return true;
	}

	// Calculate the Standard RFC 1994 chap
	static void ChapOrigin(u_char identifier, int challenge_len, const u_char * challenge, const char * pwd, u_char resp[16])
	{
		md5_state_t m;
		md5_init(&m);
		md5_append(&m, &identifier, 1);
		md5_append(&m, (u_char*)pwd, strlen(pwd));
		md5_append(&m, challenge, challenge_len);
		md5_finish(&m, resp);
	}

	/* 
		Tianyi's SecondMd5 v1.7
		Update chap with salted challenge
	*/
	static void ChapSecondMd5(int challenge_len, u_char * challenge, u_char chap[16])
	{
		int i, j;
		u_char salt_t[] = {
			0x38, 0xf2, 0xf8, 0xf8, 0x88, 0xe3, 0xe8, 0x99,
			0x76, 0x12, 0xd4, 0x22, 0xa7, 0x87, 0x65, 0x23
		};
		md5_state_s m;
		md5_init(&m);
		md5_append(&m, chap, 16);
		for (j = 0; j < challenge_len; j += 16)
		{
			for (i = 0; i < 16 && i + j < challenge_len; i++)
				chap[i] = challenge[j + i] + salt_t[i];
			md5_append(&m, chap, i);
		}
		md5_finish(&m, chap);
	}

	static void _PrintHex(const u_char * hex, int size)
	{
		int i;
		for (i = 0; i != size; i++)
			printf("%02x ", hex[i]);
		printf("\n");
	}

	/*
		天翼认证补丁
		name	(可选)用户名
		pwd	(可选)密码

		如果提供用户名和密码，则在收到 Challenge 包后马上发出 Response；
		否则在抓到本机发出的 Response 包时，发出正确的 Response。(貌似慢了0.0005秒)
	*/
	void PatchAuth(const time_t timeout = -1, const char * name = NULL, const char * pwd = NULL)
	{
		int ret;
		struct pcap_pkthdr *header;
		const u_char *pkt_data;

		time_t start_time, curr_time;
		start_time = curr_time = time(NULL);

		flag_cancel = false;
		printf("Listening...\n");
		for (; !flag_cancel && (timeout <= 0 || curr_time - start_time < timeout);
			curr_time = time(NULL))
		{
			ret = pcap_next_ex(fp, &header, &pkt_data);

			if (ret > 0)
			{
				if (pkt_data[0x14] == 0xc2 && pkt_data[0x15] == 0x23)	// CHAP
				{
					if (pkt_data[0x16] == 1)	// Challenge packet
					{
						TakeDownChallenge(pkt_data);
						if (name && pwd)
						{
							AuthWithNamePwd(pkt_data, name, pwd);
							break;
						}
					}
					else if (pkt_data[0x16] == 2)	// Response packet
					{
						AuthWithChap(pkt_data);
						break;
					}
				}
				continue;
			}
			if (ret == 0)
			{
				// Timeout
				continue;
			}
			else
			{
				// Error
				break;
			}

		}
	}

	void Cancel()
	{
		flag_cancel = true;
	}

	void TakeDownChallenge(const u_char * data)
	{
		identifier = data[23];
		challenge_len = data[26];
		memcpy(challenge_buf, data + 27, challenge_len);
	}

	void BuildResponseHeader(u_char * dst, const u_char * src, bool swap_dst_src)
	{
		// Ethernet II
		if (swap_dst_src)
		{
			memcpy(dst, src + 6, 6);
			memcpy(dst + 6, src, 6);
		}
		else
		{
			memcpy(dst, src, 6);
			memcpy(dst + 6, src + 6, 6);
		}
		memcpy(dst + 12, src + 12, 12);
		dst[22] = 2;	// Code: Response
	}

	void AuthWithNamePwd(const u_char * data, const char * name, const char * pwd)
	{
		int name_len = strlen(name);
		int pkt_len = 43 + 3 + name_len;
		u_char * pkt = (u_char*)malloc(pkt_len);
		BuildResponseHeader(pkt, data, true);
		ChapOrigin(identifier, challenge_len, challenge_buf, pwd, pkt + 27);
		ChapSecondMd5(challenge_len, challenge_buf, pkt + 27);
		if (name[0] == '^' && name[1] == '~')
		{
			memcpy(pkt + 43, name, name_len);
		}
		else
		{
			pkt[43] = '^'; pkt[44] = '~'; pkt[45] = '2';
			memcpy(pkt + 46, name, name_len);
			name_len += 3;
		}
		UpdateLengthAndSend(pkt, name_len);
		_PrintHex(pkt + 27, 16);
		free(pkt);
	}
	
	void AuthWithChap(const u_char * data)
	{
		int name_len = (data[24] << 8) + data[25] - data[26] - 5;
		u_char * pkt = (u_char*)malloc(43 + 3 + name_len);
		memcpy(pkt, data, 43);
		ChapSecondMd5(challenge_len, challenge_buf, pkt + 27);
		if (data[43] != '^' && data[44] != '~')
		{
			name_len += 3;
			pkt[43] = '^'; pkt[44] = '~'; pkt[45] = '2';
			memcpy(pkt + 46, data + 43, name_len);
		}
		UpdateLengthAndSend(pkt, name_len);
		free(pkt);
	}

	void UpdateLengthAndSend(u_char * data, int name_len)
	{
		data[26] = 16;
		name_len += 21;	// PPP CHAP header(4 bytes) + data_len(1 byte) + MD5 response(16 bytes) + name_len
		data[25] = name_len & 0xff;
		data[24] = (name_len >> 8) & 0xff;
		name_len += 2;	// PPP Protocal header(2 bytes)
		data[19] = name_len & 0xff;
		data[18] = (name_len >> 8) & 0xff;
		name_len += 20;
		int ret = pcap_sendpacket(fp, data, name_len);
	}

	void Flood()
	{
		int pack_len = 256, count = 256;
		u_char * data = (u_char*)malloc(pack_len);
		memset(data, 0, pack_len);
		for (int i = 0; i < count; i++)
		{
			pcap_sendpacket(fp, data, pack_len);
		}
	}

private:
	pcap_t * fp;
	char errbuf[PCAP_ERRBUF_SIZE];

	u_char identifier;
	int challenge_len;
	u_char challenge_buf[32];
};


#endif