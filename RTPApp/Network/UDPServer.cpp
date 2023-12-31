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

UDPServer::UDPServer(std::string _host, std::uint16_t _port) : _host(std::move(_host)), _port(std::move(_port))
{
    _socketInfo = std::shared_ptr<SocketInfo>(new SocketInfo, [](UDPServer::SocketInfo* socketInfo) {
        if (socketInfo->socketId != MY_INVALID_SOCKET)
            CLOSESOCKET(socketInfo->socketId);
        });
}

void UDPServer::Init()
{
    if (_socketInfo->socketId = socket(AF_INET, SOCK_DGRAM, 0); _socketInfo->socketId == static_cast<decltype(_socketInfo->socketId)>(MY_SOCKET_ERROR)) {
        auto error = MY_GET_LAST_ERROR;
        throw std::runtime_error("UDPServer Socket Init failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));

    }
    struct sockaddr_in servaddr {};

    // Filling server information 
    servaddr.sin_family = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //inet_pton(servaddr.sin_family, _host.c_str(), &servaddr.sin_addr.s_addr);
    servaddr.sin_port = htons(_port);

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(_socketInfo->socketId, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&tv), sizeof(tv)) < 0) {
        auto error = MY_GET_LAST_ERROR;
        throw std::runtime_error("UDPServer setsockopt failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));

    }

    // Bind the socket with the server address 
    if (auto res = bind(_socketInfo->socketId, (const struct sockaddr*)&servaddr, sizeof(servaddr)); res == MY_SOCKET_ERROR)
    {
        auto error = MY_GET_LAST_ERROR;
        throw std::runtime_error("UDPServer bind failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
    }
}
constexpr int MAX_MSG_SIZE = std::numeric_limits<uint16_t>::max();
bool UDPServer::Fetch()
{
    struct sockaddr_in clientAddr {};
    socklen_t clientAddrSize = sizeof(clientAddr);
    char buffer[MAX_MSG_SIZE];
    auto result = recvfrom(_socketInfo->socketId, (char*)buffer, MAX_MSG_SIZE, MSG_WAITALL, (struct sockaddr*)&clientAddr,&clientAddrSize);

    if (result == MY_SOCKET_ERROR) {
		auto error = MY_GET_LAST_ERROR;
		throw std::runtime_error("UDP Receive failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
	}
    queue.push(std::vector<uint8_t>(buffer, buffer + result));
    return true;
}
