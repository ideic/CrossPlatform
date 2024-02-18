#include "UDPClient.h"
#include "NetworkHeader.h"
#include <limits>
#include <array>
#include <string>
#include <memory>
#include <Common/Logger/Logger.h>
#include <utility>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <string_view>

using namespace Xaba::Network;
using namespace std::string_literals;

#if __has_include(<WinSock2.h>)
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <WinSock2.h>
#endif
#if __has_include(<ws2tcpip.h>)
    #include <ws2tcpip.h>
#endif

struct UDPClient::SocketInfo {
#ifdef WIN32
    SOCKET socketId{ MY_INVALID_SOCKET };
#else
    int socketId{ MY_INVALID_SOCKET };
#endif
};

constexpr int MAX_MSG_SIZE = std::numeric_limits<uint16_t>::max();
UDPClient::UDPClient(std::string host, uint16_t port, std::shared_ptr<ILogger> logger) : mHost(std::move(host)), 
    mPort(port), 
    socketInfo_ (std::shared_ptr<SocketInfo>(new SocketInfo, [](UDPClient::SocketInfo* socketInfo) {
          if (socketInfo->socketId != MY_INVALID_SOCKET) {
			CLOSESOCKET(socketInfo->socketId);
          }
    })),
    logger_(std::move(logger))
    {
}

void UDPClient::Init()
{
    if (socketInfo_->socketId = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); socketInfo_->socketId == static_cast<decltype(socketInfo_->socketId)>(MY_SOCKET_ERROR)) {
        auto error = MY_GET_LAST_ERROR;
      // NOLINTNEXTLINE(misc-include-cleaner, concurrency-mt-unsafe)
        throw std::runtime_error("UDPClient Socket Init failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));

    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    if (auto res = setsockopt(socketInfo_->socketId, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char*>(&MAX_MSG_SIZE), sizeof(MAX_MSG_SIZE)); res == MY_SOCKET_ERROR)
    {
        auto error = MY_GET_LAST_ERROR;
        // NOLINTNEXTLINE(misc-include-cleaner, concurrency-mt-unsafe)
        throw std::runtime_error("UDPServer setsockopt SO_RCVBUF failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
    }
}

std::vector<uint8_t> UDPClient::ReceiveData(std::string& fromHost, uint16_t& fromPort)
{
    sockaddr_in from{};
#ifdef WIN32
    int len = sizeof(from);
#else
    socklen_t len = sizeof(from);
#endif
    std::vector<uint8_t> buffer(MAX_MSG_SIZE);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    const auto res = recvfrom(socketInfo_->socketId, reinterpret_cast<char*>(buffer.data()), static_cast<int>(buffer.size()), 0, reinterpret_cast<sockaddr*>(&from), &len);
    if (res == MY_SOCKET_ERROR) {
		auto error = MY_GET_LAST_ERROR;
        // NOLINTNEXTLINE(misc-include-cleaner, concurrency-mt-unsafe)
		logger_->Error("UDPClient recvfrom failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
		return {};
	}
    fromPort = ntohs(from.sin_port);
    // NOLINTNEXTLINE(misc-include-cleaner)
    std::array<char, INET_ADDRSTRLEN> fromHostBuffer{};
#ifdef WIN32
    const auto *ipAddress = inet_ntop(AF_INET, &from.sin_addr, fromHostBuffer.data(), fromHostBuffer.size());
#else
    const auto *ipAddress = inet_ntop(AF_INET, &from.sin_addr, fromHostBuffer.data(), fromHostBuffer.size());
#endif // WIN32
    if (ipAddress == nullptr) {
		auto error = MY_GET_LAST_ERROR;
        // NOLINTNEXTLINE(misc-include-cleaner, concurrency-mt-unsafe)
		logger_->Error("UDPClient inet_ntop failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
		return {};
	}
    fromHost = std::string(begin(fromHostBuffer), end(fromHostBuffer));
	buffer.resize(res);
    return buffer;
}

bool UDPClient::SendData(std::string_view message)
{
    if (message.empty()) { return true; }

    if (message.size() > MAX_MSG_SIZE) {
        logger_->Error("UDPClient::SendData message size is too big");
        return false;
    }
    struct sockaddr_in servaddr {};

    // Filling server information 
    servaddr.sin_family = AF_INET; // IPv4 
    //servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // NOLINTNEXTLINE(misc-include-cleaner)
    inet_pton(servaddr.sin_family, mHost.c_str(), &servaddr.sin_addr.s_addr);
    servaddr.sin_port = htons(mPort);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    if (auto res = sendto(socketInfo_->socketId, message.data(), static_cast<int>(message.size()), 0, reinterpret_cast<sockaddr*>(&servaddr), sizeof(servaddr)); res == MY_SOCKET_ERROR) {
		auto error = MY_GET_LAST_ERROR;
        // NOLINTNEXTLINE(misc-include-cleaner,concurrency-mt-unsafe)
		logger_->Error("UDPClient sendto failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
        return false;
    }
    return true;
}
