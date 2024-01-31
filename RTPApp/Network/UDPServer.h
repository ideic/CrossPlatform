#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Common/BlockingQueue.h"
#include <Common/Logger/Logger.h>
namespace Xaba::Network {
	class UDPServer
	{
		struct SocketInfo;
		std::string host_;
		std::uint16_t port_;

		std::shared_ptr<SocketInfo> socketInfo_;
		std::shared_ptr<ILogger> logger_;
		public:
			UDPServer(std::string _host, std::uint16_t _port, std::shared_ptr<ILogger> logger);
			void Init();
			bool KeepRunning();
			BlockingQueue<std::vector<uint8_t>> queue{};
	};
}
