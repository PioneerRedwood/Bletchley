#ifndef _NETWORK_SERVER_
#define _NETWORK_SERVER_

#include "predef.h"

class NetworkServer
{
public:
	NetworkServer();
	~NetworkServer();

	bool Init(unsigned sec);
	void Shutdown();
	bool Loop();
	bool Connect();

	// callbacks?
	uint16_t SendMessage(uint16_t conn, const char* messageBuffer);

private:
	WSAData data;
	fd_set masterfds;

	struct timeval timeout;

	uint16_t maxfd;

	SOCKET masterSocket;
	SOCKET tempSocket;

	sockaddr_in	server_addr;

	char buffer[BUFFER_SIZE];

	void Setup(int port, int interval);
	bool InitSocket(unsigned sec);
	bool Bind();
	bool Listen();
	bool Receive(SOCKET fd);

	int clientId = 0;
	std::deque<SOCKET> clientDeque;
	std::map<SOCKET, int> clientMap;
};

#endif
