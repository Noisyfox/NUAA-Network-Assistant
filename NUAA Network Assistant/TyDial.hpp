//天翼chap拨号补丁 by YeahO_O Yuan
//Modified by Noisyfox
#pragma once

#ifndef TYDIAL_HPP_
#define TYDIAL_HPP_

#include "md5.h"
#include "pcap.h"
#include <ctime>
#include <cstring>
#include <cstdint>

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
		Cancel();
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
	__inline static void ChapOrigin(u_char identifier, int challenge_len, const u_char * challenge, const char * pwd, const int pwdLen, u_char resp[16])
	{
		md5_state_t m;
		md5_init(&m);
		md5_append(&m, &identifier, 1);
		md5_append(&m, (u_char*)pwd, pwdLen);
		md5_append(&m, challenge, challenge_len);
		md5_finish(&m, resp);
	}

	__inline static uint32_t uint8to32(const uint8_t *v) {
		return (v[3] << 24) | (v[2] << 16) | (v[1] << 8) | v[0];
	}

	__inline static void uint32to8(uint32_t v, uint8_t *d) {
		d[3] = (v >> 24) & 0xff;
		d[2] = (v >> 16) & 0xff;
		d[1] = (v >> 8) & 0xff;
		d[0] = v & 0xff;
	}

	__inline static void tean_encrypt(const uint8_t k[16], int rounds, uint8_t *d, size_t len) {
		const uint32_t key[4] = {
			uint8to32(k),
			uint8to32(k + 4),
			uint8to32(k + 8),
			uint8to32(k + 12)
		};
		uint8_t *eod = d + len;
		uint32_t v0, v1;
		uint32_t delta = 0x9e3779b9;
		uint32_t sum;
		int i;

		for (d; d < eod; d += 8) {
			v0 = uint8to32(d);
			v1 = uint8to32(d + 4);

			sum = 0;
			for (i = 0; i != rounds; ++i) {
				v0 += (v1 ^ sum) + key[sum & 3] + ((v1 << 4) ^ (v1 >> 5));
				sum += delta;
				v1 += (v0 ^ sum) + key[(sum >> 11) & 3] + ((v0 << 4) ^ (v0 >> 5));
			}

			uint32to8(v0, d);
			uint32to8(v1, d + 4);
		}
	}

	__inline static void tean_decrypt(const uint8_t k[16], int rounds, uint8_t *d, size_t len) {
		const uint32_t key[4] = {
			uint8to32(k),
			uint8to32(k + 4),
			uint8to32(k + 8),
			uint8to32(k + 12)
		};
		uint8_t *eod = d + len;
		uint32_t v0, v1;
		uint32_t delta = 0x9e3779b9;
		uint32_t sum;
		int i;

		for (d; d < eod; d += 8) {
			v0 = uint8to32(d);
			v1 = uint8to32(d + 4);

			sum = rounds * delta;
			for (i = 0; i != rounds; ++i) {
				v1 -= (v0 ^ sum) + key[(sum >> 11) & 3] + ((v0 << 4) ^ (v0 >> 5));
				sum -= delta;
				v0 -= (v1 ^ sum) + key[sum & 3] + ((v1 << 4) ^ (v1 >> 5));
			}

			uint32to8(v0, d);
			uint32to8(v1, d + 4);
		}
	}

	__inline static void encrypt4(const uint8_t *salt, uint8_t *resp2) {
		uint8_t v15[256], tmp, r, tp;
		size_t i, j;

		for (i = 0; i < 256; i++) {
			v15[i] = i;
		}

		tp = 0;
		for (i = 0; i < 256; i++) {
			tp = (tp + salt[i & 0xf] + v15[i]) & 0xff;
			tmp = v15[i];
			v15[i] = v15[tp];
			v15[tp] = tmp;
		}

		tp = 0;
		for (i = 0; i < 16; i++) {
			j = (i + 1) & 0xff;
			tp = (tp + v15[j]) & 0xff;
			tmp = v15[j];
			v15[j] = v15[tp];
			v15[tp] = tmp;
			r = v15[(tmp + v15[j]) & 0xff];
			resp2[i] ^= r;
		}
	}

	__inline static void do_tyEncrypt(const uint8_t *salt, uint8_t *data) {
		switch (data[0] % 5) {
		case 0:
			tean_encrypt(salt, 16, data, 16);
			break;
		case 1:
			tean_decrypt(salt, 16, data, 16);
			break;
		case 2:
			tean_encrypt(salt, 32, data, 16);
			break;
		case 3:
			tean_decrypt(salt, 32, data, 16);
			break;
		case 4:
			encrypt4(salt, data);
			break;
		}
	}

	/*
	Tianyi's SecondMd5 v2.0
	New encrypt digest
	*/
	__inline static void NewChapSecondMd5(u_char chap[16])
	{
		// 在这里我们要通过pdext获得salt
		static uint8_t salt[] = {
			0x48, 0x6c, 0xbc, 0x84, 0xa3, 0x78, 0x2e, 0x98,
			0xba, 0xa7, 0xfc, 0xec, 0x0e, 0x14, 0x8a, 0xc0
		};

		do_tyEncrypt(salt, chap);
	}

	static void _PrintHex(const u_char * hex, int size)
	{
		int i;
		for (i = 0; i != size; i++)
			printf("%02x ", hex[i]);
		printf("\n");
	}

	void PatchSetup(const time_t timeout, const char * name, const char * pwd){
		// 检查前缀
			if (strstr(name, "^#") == name){
				strcpy(mAuthName, name);
			}
			else {
				mAuthName[0] = '^';
				mAuthName[1] = '#';
				mAuthName[2] = '0';
				mAuthName[3] = '2';
				strcpy(mAuthName + 4, name);
			}

		strcpy(mAuthPwd, pwd);
		mNameLen = strlen(mAuthName);
		mPwdLen = strlen(mAuthPwd);

		mTimeout = timeout;
	}

	/*
		天翼认证补丁
		name	(可选)用户名
		pwd	(可选)密码

		如果提供用户名和密码，则在收到 Challenge 包后马上发出 Response；
		否则在抓到本机发出的 Response 包时，发出正确的 Response。(貌似慢了0.0005秒)
		*/
	void PatchAuth()
	{
		int ret;
		struct pcap_pkthdr *header;
		const u_char *pkt_data;

		time_t start_time, curr_time;
		start_time = curr_time = time(NULL);

		flag_cancel = false;
		//printf("Listening...\n");
		for (; !flag_cancel && (mTimeout <= 0 || (curr_time = time(NULL), curr_time - start_time < mTimeout));)
		{
			ret = pcap_next_ex(fp, &header, &pkt_data);

			if (ret > 0)
			{
				if (pkt_data[0x14] == 0xc2 && pkt_data[0x15] == 0x23)	// CHAP
				{
					if (pkt_data[0x16] == 1)	// Challenge packet
					{
						TakeDownChallenge(pkt_data);
						AuthWithNamePwd_new(pkt_data);
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

	inline void TakeDownChallenge(const u_char * data)
	{
		identifier = data[23];
		challenge_len = data[26];
		memcpy(challenge_buf, data + 27, challenge_len);
	}

	inline static void BuildResponseHeader(u_char * dst, const u_char * src, bool swap_dst_src)
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

	inline void AuthWithNamePwd_new(const u_char * data)
	{
		int pkt_len = 43 + mNameLen;
		u_char * pkt = (u_char*)malloc(pkt_len);
		BuildResponseHeader(pkt, data, true);
		ChapOrigin(identifier, challenge_len, challenge_buf, mAuthPwd, mPwdLen, pkt + 27);
		NewChapSecondMd5(pkt + 27);
		memcpy(pkt + 43, mAuthName, mNameLen);
		UpdateLengthAndSend(pkt, mNameLen);
		//_PrintHex(pkt + 27, 16);
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

	time_t mTimeout = -1;
	int mNameLen = 0;
	int mPwdLen = 0;
	char mAuthName[255];
	char mAuthPwd[255];
};

#endif
