#pragma once
#include <string>
#include <vector>
#include <memory>
namespace Xaba::Network {
	class TCPClient
	{
		struct SocketInfo;
		std::string mHost;
		uint16_t mPort;

		std::shared_ptr<SocketInfo> _socketInfo;

	public:
		TCPClient(std::string host, uint16_t port);
		void Connect();

		std::vector<uint8_t> ReceiveData();
		void SendData(std::string_view message);
	};
}
