#pragma once
#include <string>
#include <vector>
#include <memory>
namespace Xaba::Network {
	class UDPClient
	{
		struct SocketInfo;
		std::string _host;
		std::uint16_t _port;

		std::shared_ptr<SocketInfo> _socketInfo;

	public:
		UDPClient(std::string host, uint16_t port);
		void Connect();

		std::vector<uint8_t> ReceiveData();
		void SendData(std::string_view message);
	};
}
