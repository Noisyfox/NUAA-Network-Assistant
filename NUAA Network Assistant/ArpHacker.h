#pragma once

#include <stdlib.h>

class ArpHacker
{
private:
	NetDetector netDet;
	NetInfo netInfo;
	BOOL isArpHacked;
public:
	ArpHacker();
	~ArpHacker();
	BOOL Init(NetInfo & adapter);
	BOOL FlushArp();
	BOOL HackArp();
	BOOL IsArpHacked();
};
