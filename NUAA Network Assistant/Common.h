#pragma once

#include <winsock2.h>
#include <ws2def.h>
#include <ws2ipdef.h>
#include <iphlpapi.h>
#include <pcap.h>

#include "VISTA.h"

#include "Util.h"
#include "NetDetector.h"
#include "ArpHacker.h"
#include "RouteHacker.h"
#include "NAT.h"

#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wpcap.lib")

extern NetDetector _NetDetector;
extern NetInfo _NetInfo;
extern ArpHacker _ArpHacker;
extern RouteHacker _RouteHacker;
extern NAT _NAT;
