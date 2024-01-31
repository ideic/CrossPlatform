#include "UDPServer.h"
#include "NetworkHeader.h"
#include <limits>
using namespace Xaba::Network;
using namespace std::string_literals;
struct UDPServer::SocketInfo {
#ifdef WIN32
    SOCKET socketId{ MY_INVALID_SOCKET };
#else
    int socketId{ MY_INVALID_SOCKET };
#endif
};
constexpr int MAX_MSG_SIZE = std::numeric_limits<uint16_t>::max();
UDPServer::UDPServer(std::string host, std::uint16_t port, std::shared_ptr<ILogger> logger) : host_(std::move(host)), port_(std::move(port)), logger_(std::move(logger))
{
    socketInfo_ = std::shared_ptr<SocketInfo>(new SocketInfo, [](UDPServer::SocketInfo* socketInfo) {
        if (socketInfo->socketId != MY_INVALID_SOCKET)
            CLOSESOCKET(socketInfo->socketId);
        });
}

void UDPServer::Init()
{
    if (socketInfo_->socketId = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); socketInfo_->socketId == static_cast<decltype(socketInfo_->socketId)>(MY_SOCKET_ERROR)) {
        auto error = MY_GET_LAST_ERROR;
        throw std::runtime_error("UDPServer Socket Init failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));

    }
    struct sockaddr_in servaddr {};

    // Filling server information 
    servaddr.sin_family = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //inet_pton(servaddr.sin_family, _host.c_str(), &servaddr.sin_addr.s_addr);
    servaddr.sin_port = htons(port_);

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    if (auto res = setsockopt(socketInfo_->socketId, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&tv), sizeof(tv)); res == SOCKET_ERROR) {
        auto error = MY_GET_LAST_ERROR;
        throw std::runtime_error("UDPServer setsockopt SO_RCVTIMEO failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));

    }
    char opt = 1;
    if (auto res = setsockopt(socketInfo_->socketId, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, &opt, sizeof(opt)); res == SOCKET_ERROR)
    {
		auto error = MY_GET_LAST_ERROR;
		throw std::runtime_error("UDPServer setsockopt SO_EXCLUSIVEADDRUSE failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
	}

    if (auto res = setsockopt(socketInfo_->socketId, SOL_SOCKET, SO_RCVBUF, (char*)&MAX_MSG_SIZE, sizeof(MAX_MSG_SIZE)); res == SOCKET_ERROR)
    {
        auto error = MY_GET_LAST_ERROR;
        throw std::runtime_error("UDPServer setsockopt SO_RCVBUF failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
    }

    // Bind the socket with the server address 
    if (auto res = bind(socketInfo_->socketId, (const struct sockaddr*)&servaddr, sizeof(servaddr)); res == MY_SOCKET_ERROR)
    {
        auto error = MY_GET_LAST_ERROR;
        throw std::runtime_error("UDPServer bind failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
    }
}

bool UDPServer::KeepRunning()
{
    struct sockaddr_in clientAddr {};
    socklen_t clientAddrSize = sizeof(clientAddr);
    char buffer[MAX_MSG_SIZE];
    auto result = recvfrom(socketInfo_->socketId, (char*)buffer, MAX_MSG_SIZE, 0, (struct sockaddr*)&clientAddr,&clientAddrSize);

    if (result == MY_SOCKET_ERROR) {
		auto error = MY_GET_LAST_ERROR;
        if (error == WSAETIMEDOUT) {
			return true;
		}
		logger_->Error("UDP Receive failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
	}
    queue.push(std::vector<uint8_t>(buffer, buffer + result));
    return true;
}
