#ifndef _NETWORK_SERVER_
#define _NETWORK_SERVER_

#include "predef.h"
#include <stack>

class NetworkServer
{
public:

	NetworkServer()
	{
		Setup();
	}
	~NetworkServer()
	{
		closesocket(masterSocket);
	}

	bool Init(long sec);
	void Shutdown();
	bool Loop();
	bool Connect();

private:
	WSAData data;
	fd_set masterfds;
	fd_set readfds;

	struct timeval timeout;

	uint16_t maxfd;

	SOCKET masterSocket;
	SOCKET tempSocket;

	sockaddr_in	server_addr;

	char buffer[BUFFER_SIZE];

	void Setup();
	bool InitSocket(long sec);
	bool Receive(SOCKET fd);

	std::deque<SOCKET> clientDeque;
};

#endif
