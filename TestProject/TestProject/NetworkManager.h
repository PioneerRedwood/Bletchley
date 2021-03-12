#ifndef _NETWORK_MANAGER_
#define _NETWORK_MANAGER_

#include "predef.h"

#include "NetworkManager.h"
#include "NetworkClient.h"
#include "NetworkServer.h"

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	void OnServer(bool isServer);
	void OnClient(bool isClient);

	void Update();
	void Read();
	void Write();
	void ParsePacket(char* buffer);

private:
	bool enable	  = false;
	bool isServer = false;
	bool isClient = false;
	NetworkServer* server;
	NetworkClient* client;
	
};

#endif // !_NETWORK_MANAGER_
