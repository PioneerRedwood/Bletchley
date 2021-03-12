#include "predef.h"
#include "NetworkServer.h"

#include <chrono>
#include <thread>
#include "NetworkClient.h"


// server main
int main()
{
	std::cout << std::this_thread::get_id() << " worked..\n";
	double period = 0.25;

	NetworkServer server;
	DataStorage::GetInstance()->SetValue("filelog", true);
	DataStorage::GetInstance()->SetValue("consolelog", true);

	std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();

	if (server.Init(period))
	{
		while (true)
		{
			if (server.Connect())
			{
				while (true)
				{
					std::chrono::duration<double> now = std::chrono::system_clock::now() - startTime;
					if (now.count() >= period)
					{
						startTime = std::chrono::system_clock::now();
						if (!server.Loop())
						{
							break;
						}
					}
				}
			}
			else
			{
				std::chrono::duration<double> now = std::chrono::system_clock::now() - startTime;
				if (now.count() >= period)
				{
					std::cout << "lisening.. \n";
					startTime = std::chrono::system_clock::now();
					continue;
				}
			}
		}
	}

	server.Shutdown();
	std::cout << std::this_thread::get_id() << " exit..\n";

	return 0;
}

// client main
int clientMain()
{
	/*
		auto tempClient = [](double period)
		{
			DataStorage::GetInstance()->SetValue("filelog", true);

			std::cout << std::this_thread::get_id() << " worked..\n";
			NetworkClient client;
			std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();

			if (client.Init(1, PORT))
			{
				if (client.Connect())
				{
					while (true)
					{
						std::chrono::duration<double> now = std::chrono::system_clock::now() - startTime;
						if (now.count() >= period)
						{
							startTime = std::chrono::system_clock::now();
							if (!client.Loop())
							{
								break;
							}
						}
					}
				}
			}
			client.Shutdown();
			std::cout << std::this_thread::get_id() << " exit..\n";
		};

		//std::thread clientThread = std::thread(tempClient, 3.0);
		//serverThread.join();
	*/

	std::cout << std::this_thread::get_id() << " worked..\n";
	double period = 0.25;

	NetworkClient client;
	DataStorage::GetInstance()->SetValue("filelog", true);
	DataStorage::GetInstance()->SetValue("consolelog", true);

	std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();

	if (client.Init(period, PORT))
	{
		while (true)
		{
			if (client.Connect())
			{
				while (true)
				{
					std::chrono::duration<double> now = std::chrono::system_clock::now() - startTime;
					if (now.count() >= period)
					{
						startTime = std::chrono::system_clock::now();
						if (!client.Loop())
						{
							break;
						}
					}
				}
			}
			else
			{
				std::chrono::duration<double> now = std::chrono::system_clock::now() - startTime;
				if (now.count() >= period)
				{
					startTime = std::chrono::system_clock::now();
					continue;
				}
			}
		}
	}

	client.Shutdown();
	std::cout << std::this_thread::get_id() << " exit..\n";

	return 0;
}