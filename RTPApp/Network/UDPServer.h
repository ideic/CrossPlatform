#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Common/BlockingQueue.h"
namespace Xaba::Network {
	class UDPServer
	{
		struct SocketInfo;
		std::string _host;
		std::uint16_t _port;

		std::shared_ptr<SocketInfo> _socketInfo;

		public:
			UDPServer(std::string _host, std::uint16_t _port);
			void Init();
			bool Fetch();
			BlockingQueue<std::vector<uint8_t>> queue{};
	};
}
