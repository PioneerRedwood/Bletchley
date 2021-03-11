#include <iostream>
#include <thread>

int main()
{
	auto testThread = [](int i) 
	{
		std::cout << std::this_thread::get_id() << std::endl;

		for (int j = 0; j < i; ++j)
		{
			std::cout << j << std::endl;
 		}

		std::cout << "thread is over\n";
	};

	std::thread test = std::thread(testThread, 5);

	test.join();

	return 0;
}