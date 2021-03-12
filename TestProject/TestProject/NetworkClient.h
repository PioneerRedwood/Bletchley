#ifndef _NETWORK_CLIENT_
#define _NETWORK_CLIENT_

#include "predef.h"

struct SocketBuffer
{
	int totalSize;
	int currentSize;

	char buffer[BUFFER_SIZE] = { 0, };

	SocketBuffer()
	{
		totalSize = -1;
		currentSize = 0;

		ZeroMemory(buffer, BUFFER_SIZE);
	}
};

class NetworkClient
{
public:
	NetworkClient() {};
	~NetworkClient() {};

	std::deque<SocketBuffer> bufferDeque;

	bool Init(unsigned sec, int port);
	bool Connect();
	bool Loop();

	bool Shutdown();

	bool Send(const char* contents, unsigned size);
	bool Recv();

private:
	int clientId = -1;

	WSAData data;
	fd_set	masterfds;

	struct timeval timeout;

	SOCKET	clientSocket;
	SOCKADDR_IN address;

	char buffer[BUFFER_SIZE];

	bool InitSocket(unsigned sec, int port);
};

#endif // !_NETWORKCLIENT_
