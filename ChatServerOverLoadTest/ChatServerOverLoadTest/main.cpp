#include <asio.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <mutex>

struct ConnectionResult
{
	bool success;
	double duration;
};

ConnectionResult SimulateClient(const std::string& server_ip, const int server_port)
{
	asio::io_context context;
	asio::ip::tcp::socket socket(context);

	ConnectionResult result;

	auto start = std::chrono::high_resolution_clock::now();

	try
	{
		socket.connect(asio::ip::tcp::endpoint(asio::ip::make_address(server_ip), server_port));

		result.success = true;
	}

	catch (const std::exception&)
	{
		result.success = false;
	}

	auto end = std::chrono::high_resolution_clock::now();
	result.duration = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

	return result;
}

void ClientThread(const std::string& server_ip, int server_port, std::vector<ConnectionResult>& connectionresult, std::mutex& mutex)
{
	auto result = SimulateClient(server_ip, server_port);

	std::lock_guard<std::mutex> lock(mutex);

	connectionresult.push_back(result);
}

int main()
{
	std::string SERVER_IP = "127.0.0.1";
	int SERVER_PORT = 9000;

	std::vector<ConnectionResult> result;
	std::vector<std::thread> client;
	std::mutex mutex;

	for (int i = 0; i < 10000; i++)
	{
		client.emplace_back(ClientThread, SERVER_IP, SERVER_PORT, std::ref(result), std::ref(mutex));
	}

	for (int i = 0; i < client.size(); i++)
	{
		client[i].join();
	}
	
	int count = 0;
	double totalduration = 0;

	for (int i = 0; i < result.size(); i++)
	{
		if (result[i].success)
		{
			count++;

			totalduration += result[i].duration;
		}
	}

	std::cout << "Successful connections: " << count << "/10000" << std::endl;

	if (count > 0)
	{
		std::cout << "Average connection time: " << totalduration / count << " ms" << std::endl;
	}

	std::cin.get();

	return 0;
}