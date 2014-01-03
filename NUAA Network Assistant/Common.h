#pragma once

#include <winsock2.h>
#include <iphlpapi.h>
#include "Util.h"
#include "NetDetector.h"
#include "ArpHacker.h"
#include "RouteHacker.h"

#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib")

extern NetDetector _NetDetector;
extern NetInfo _NetInfo;
extern ArpHacker _ArpHacker;
extern RouteHacker _RouteHacker;
