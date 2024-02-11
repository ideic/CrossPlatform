#include "UDPServer.h"
#include "NetworkHeader.h"
#include <limits>
#include <cstdint>
#include <array>
#include <string>
#include <memory>
#include <Common/Logger/Logger.h>
#include <utility>
#include <stdexcept>
#include <vector>

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

struct UDPServer::SocketInfo {
#ifdef WIN32
    SOCKET socketId{ MY_INVALID_SOCKET };
#else
    int socketId{ MY_INVALID_SOCKET };
#endif
};
constexpr int MAX_MSG_SIZE = std::numeric_limits<uint16_t>::max();
UDPServer::UDPServer(std::string host, std::uint16_t port, std::shared_ptr<ILogger> logger) : host_(std::move(host)), 
    port_(port), 
    socketInfo_(std::shared_ptr<SocketInfo>(new SocketInfo, [](UDPServer::SocketInfo* socketInfo) {
      if (socketInfo->socketId != MY_INVALID_SOCKET) { CLOSESOCKET(socketInfo->socketId); }
    })),
    logger_(std::move(logger))
{
}

void UDPServer::Init()
{
    if (socketInfo_->socketId = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); socketInfo_->socketId == static_cast<decltype(socketInfo_->socketId)>(MY_SOCKET_ERROR)) {
        auto error = MY_GET_LAST_ERROR;
        // NOLINTNEXTLINE(misc-include-cleaner)
        throw std::runtime_error("UDPServer Socket Init failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));

    }
    struct sockaddr_in servaddr {};

    // Filling server information 
    servaddr.sin_family = AF_INET; // IPv4 
    //servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_pton(servaddr.sin_family, host_.c_str(), &servaddr.sin_addr.s_addr);
    servaddr.sin_port = htons(port_);

    const int FiveSeconds = 5;
    struct timeval timeout{FiveSeconds,0};

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    if (auto res = setsockopt(socketInfo_->socketId, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeout), sizeof(timeout)); res == MY_SOCKET_ERROR) {
        auto error = MY_GET_LAST_ERROR;
        // NOLINTNEXTLINE(misc-include-cleaner)
        throw std::runtime_error("UDPServer setsockopt SO_RCVTIMEO failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));

    }
#ifdef WIN32
	char opt = 0;
#else
    int opt = 0;
#endif // WIN32

    if (auto res = setsockopt(socketInfo_->socketId, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); res == MY_SOCKET_ERROR)
    {
        auto error = MY_GET_LAST_ERROR;
        // NOLINTNEXTLINE(misc-include-cleaner)
        throw std::runtime_error("UDPServer setsockopt SO_REUSEADDR failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
    }
#ifdef WIN32
    opt = 1;
    if (auto res = setsockopt(socketInfo_->socketId, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, &opt, sizeof(opt)); res == MY_SOCKET_ERROR)
    {
        auto error = MY_GET_LAST_ERROR;
        // NOLINTNEXTLINE(misc-include-cleaner)
        throw std::runtime_error("UDPServer setsockopt SO_EXCLUSIVEADDRUSE failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
    }
#endif // WIN32
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    if (auto res = setsockopt(socketInfo_->socketId, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char*>(&MAX_MSG_SIZE), sizeof(MAX_MSG_SIZE)); res == MY_SOCKET_ERROR)
    {
        auto error = MY_GET_LAST_ERROR;
        // NOLINTNEXTLINE(misc-include-cleaner)
        throw std::runtime_error("UDPServer setsockopt SO_RCVBUF failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
    }

    // Bind the socket with the server address 
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    if (auto res = bind(socketInfo_->socketId, reinterpret_cast<const struct sockaddr*>(&servaddr), sizeof(servaddr)); res == MY_SOCKET_ERROR)
    {
        auto error = MY_GET_LAST_ERROR;
        // NOLINTNEXTLINE(misc-include-cleaner)
        throw std::runtime_error("UDPServer bind failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
    }
}

bool UDPServer::KeepRunning()
{
    struct sockaddr_in clientAddr {};
    socklen_t clientAddrSize = sizeof(clientAddr);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init,hicpp-member-init)
    std::array<char, MAX_MSG_SIZE> buffer;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    auto result = recvfrom(socketInfo_->socketId, reinterpret_cast<char*>(buffer.data()), buffer.size(), 0, reinterpret_cast<struct sockaddr *>(&clientAddr), &clientAddrSize);

    if (result == MY_SOCKET_ERROR) {
		auto error = MY_GET_LAST_ERROR;
        if (error == MY_ETIMEDOUT) {
			return true;
		}
        // NOLINTNEXTLINE(misc-include-cleaner)
		logger_->Error("UDP Receive failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
        return true;
	}
    queue.push(std::vector<uint8_t>(begin(buffer), begin(buffer) + result));
    return true;
}
