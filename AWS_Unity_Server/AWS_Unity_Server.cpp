// 2021.03.17
// Windows에서만 동작

#include "predef.h"
#include "NetworkServer.h"

#include <chrono>
#include <thread>

int main()
{
	DataStorage::GetInstance()->SetValue("filelog", true);
	DataStorage::GetInstance()->SetValue("consolelog", true);

	// server 
	double period = 1;

	NetworkServer server;

	std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();

	if (server.Init(period))
	{
		while (true)
		{
			std::chrono::duration<double> now = std::chrono::system_clock::now() - startTime;
			if (now.count() >= period)
			{
				startTime = std::chrono::system_clock::now();
				if (!server.Loop())
				{
					continue;
				}
			}
		}

		server.Shutdown();
		return 0;
	}
}