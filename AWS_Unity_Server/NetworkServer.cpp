#include "NetworkServer.h"

#include "Logger.h"
#include "DataStorage.h"

bool		NetworkServer::Init(long sec)
{
	return InitSocket(sec);
}

void		NetworkServer::Shutdown()
{
	WSACleanup();
	int ret = closesocket(masterSocket);
	if (ret == SOCKET_ERROR)
	{
		Logger::Log(LOG_ERROR, "shutdown() failed");
	}
	else
	{
		std::cout << "closesocket() executed\n";
	}
}

bool		NetworkServer::Loop()
{
	bool isOK = false;

	if (tempSocket <= 0)
	{
		isOK = Connect();
	}

	readfds = masterfds;

	int ret = select(maxfd + 1, &readfds, nullptr, nullptr, &timeout);
	if (ret < 0)
	{
		Logger::Log(LOG_ERROR, "select() failed %d", WSAGetLastError());
	}
	else if (ret == 0)
	{
		// 클라이언트 연결 끊기면 다시 연결 시도하는 걸 넣어야 함
		Logger::Log(LOG_INFO, "select() worked .. but there is no change in fds");
		tempSocket = -1;
	}
	else
	{
		for (SOCKET socket : clientDeque)
		{
			if (FD_ISSET(socket, &readfds))
			{
				if (masterSocket == socket)
				{
					isOK = Connect();
				}
				else
				{
					isOK = Receive(socket);
				}
			}
		}
	}

	for (SOCKET socket : clientDeque)
	{
		// 접속 클라이언트 구분용
		char sockNo[4] = { 0, };

		// 체온 Temperature, 맥박, Pulse, 분당 호흡수 RR(Respiratory, Rate)
		char temp[4] = { 0, };
		char pulse[4] = { 0, };
		char breathRate[4] = { 0, };

		if (!(_itoa_s(socket, sockNo, 10)
				& _itoa_s(36, temp, 10)
				& _itoa_s(100, pulse, 10)
				& _itoa_s(70, breathRate, 10)))
		{
			// 보내는 소켓번호(4bytes) + 생체데이터(12bytes) + 현재 시각(?bytes)
			std::string str = Utils::GetCurrentDateTime() + '`' + sockNo + 
								'`' + temp + '`' + pulse + '`' + breathRate;
			const char* now = str.c_str();

			int ret = send(socket, now, str.size() + 1, 0);
	
			if (ret > 0)
			{
				std::cout << ret << " " << str.size() << std::endl;
			}
		}
	}

	return isOK;
}

void		NetworkServer::Setup()
{
	int ret = WSAStartup(MAKEWORD(2, 2), &data);
	if (ret != 0)
	{
		Logger::Log(LOG_ERROR, "WSAStartup failed %d", WSAGetLastError());
		Shutdown();
	}

	masterSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (masterSocket == SOCKET_ERROR)
	{
		Logger::Log(LOG_ERROR, "socket creation failed %d", WSAGetLastError());
	}

	FD_ZERO(&masterfds);
	FD_ZERO(&readfds);

	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.S_un.S_addr = htons(INADDR_ANY);

	ZeroMemory(buffer, BUFFER_SIZE);
}

bool		NetworkServer::InitSocket(long sec)
{
	bool result = false;

	timeout.tv_sec = sec;
	timeout.tv_usec = 0;

	int ret = bind(masterSocket, (struct sockaddr*)&server_addr, sizeof(server_addr));

	if (ret < 0)
	{
		Logger::Log(LOG_ERROR, "bind() failed %d", WSAGetLastError());
		result = false;
	}
	else
	{
		result = true;
	}


	FD_SET(masterSocket, &masterfds);
	maxfd = masterSocket;

	ret = listen(masterSocket, BACKLOG);

	if (ret < 0)
	{
		Logger::Log(LOG_ERROR, "listen() failed %d", WSAGetLastError());
		result = false;
	}
	else
	{
		result = true;
	}

	return result;
}

bool		NetworkServer::Connect()
{
	sockaddr_in client;
	int clientSize = sizeof(struct sockaddr_in);

	tempSocket = accept(masterSocket, (struct sockaddr*)&client, &clientSize);

	if (tempSocket < 0)
	{
		Logger::Log(LOG_ERROR, "accept() failed %d", WSAGetLastError());
	}
	else
	{
		unsigned long arg = 1;
		if (ioctlsocket(tempSocket, FIONBIO, &arg) != 0)
		{
			Logger::Log(LOG_ERROR, "set non-blocking failed %d", WSAGetLastError());
			return false;
		}

		FD_SET(tempSocket, &masterfds);

		wchar_t host[NI_MAXHOST] = { 0, };
		wchar_t service[NI_MAXSERV] = { 0, };

		if (GetNameInfoW((sockaddr*)&client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
		{
			std::wstring hs(host);
			std::wstring serv(service);
			Logger::Log(LOG_INFO, "New connection! %s:%s\n", std::string(hs.begin(), hs.end()).c_str(), std::string(serv.begin(), serv.end()).c_str());

			clientDeque.push_back(tempSocket);
		}
	}

	return false;
}

bool		NetworkServer::Receive(SOCKET client_fd)
{
	int ret = recv(client_fd, buffer, BUFFER_SIZE, 0);
	if (ret <= 0)
	{
		if (ret == SOCKET_ERROR)
		{

			closesocket(client_fd);
			FD_CLR(client_fd, &masterfds);
			return false;
		}
		else
		{
			return false;
		}
		Logger::Log(LOG_ERROR, "recv() failed");
		closesocket(client_fd);
		FD_CLR(client_fd, &masterfds);
		return false;
	}
	else
	{
		//Logger::Log(LOG_INFO, "%d : %s", client_fd, buffer);
		return true;
	}
}
