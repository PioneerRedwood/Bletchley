#ifndef _PREDEF_
#define _PREDEF_

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <errno.h>

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <memory>
#include <map>

#include "Utils.cpp"
#include "Logger.h"
#include "DataStorage.h"

#pragma comment(lib, "ws2_32.lib")

//////////////////////////////////////////////////////////

#define		LOG_ERROR			-1
#define		LOG_INFO			1

//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////

#define		SERVERADDRESS		"127.0.0.1"
#define		BUFFER_SIZE			2048				// 2KB
#define		PORT				9000
#define		BACKLOG				5

//////////////////////////////////////////////////////////

#define		MSG_CONNECTION_REQUEST			0x10
#define		MSG_CONNECTION_RESPONSE			0x11
#define		MSG_CONNECTION_DENY				0x12
#define		MSG_KEEPALVIE					0x13

//////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////

#endif // !_PREDEF_
