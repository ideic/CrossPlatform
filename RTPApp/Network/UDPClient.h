#pragma once
#include <string>
#include <vector>
#include <memory>
#include <Common/Logger/Logger.h>
namespace Xaba::Network {
	class UDPClient
	{
		struct SocketInfo;
		std::string mHost;
		uint16_t mPort;

		std::shared_ptr<SocketInfo> socketInfo_;
		std::shared_ptr<ILogger> logger_;
	public:
		UDPClient(std::string host, uint16_t port, std::shared_ptr<ILogger> logger);
		void Init();
		std::vector<uint8_t> ReceiveData(std::string &fromHost, uint16_t &fromPort);
		bool SendData(std::string_view message);
	};
}
