#include "UDPClient.h"
#include "NetworkHeader.h"
#include <limits>
using namespace Xaba::Network;
using namespace std::string_literals;

struct UDPClient::SocketInfo {
#ifdef WIN32
    SOCKET socketId{ MY_INVALID_SOCKET };
#else
    int socketId{ MY_INVALID_SOCKET };
#endif
};

constexpr int MAX_MSG_SIZE = std::numeric_limits<uint16_t>::max();
UDPClient::UDPClient(std::string host, uint16_t port, std::shared_ptr<ILogger> logger) : host_(std::move(host)), port_(port), logger_(std::move(logger))
{
    socketInfo_ = std::shared_ptr<SocketInfo>(new SocketInfo, [](UDPClient::SocketInfo* socketInfo) {
		if (socketInfo->socketId != MY_INVALID_SOCKET)
			CLOSESOCKET(socketInfo->socketId);
		});
}

void UDPClient::Init()
{
    if (socketInfo_->socketId = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); socketInfo_->socketId == static_cast<decltype(socketInfo_->socketId)>(MY_SOCKET_ERROR)) {
        auto error = MY_GET_LAST_ERROR;
        throw std::runtime_error("UDPClient Socket Init failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));

    }


    if (auto res = setsockopt(socketInfo_->socketId, SOL_SOCKET, SO_SNDBUF, (char*)&MAX_MSG_SIZE, sizeof(MAX_MSG_SIZE)); res == MY_SOCKET_ERROR)
    {
        auto error = MY_GET_LAST_ERROR;
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
    int res = recvfrom(socketInfo_->socketId, reinterpret_cast<char*>(buffer.data()), static_cast<int>(buffer.size()), 0, reinterpret_cast<sockaddr*>(&from), &len);
    if (res == MY_SOCKET_ERROR) {
		auto error = MY_GET_LAST_ERROR;
		logger_->Error("UDPClient recvfrom failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
		return {};
	}
    fromPort = ntohs(from.sin_port);
#ifdef WIN32
    auto ip = ntohl(from.sin_addr.S_un.S_addr);
#else
    auto ip = ntohl(from.sin_addr.s_addr); //.S_un.S_addr);

#endif // WIN32

    fromHost = std::to_string((ip >> 24) & 0xFF) + "."s + std::to_string((ip >> 16) & 0xFF) + "."s + std::to_string((ip >> 8) & 0xFF) + "."s + std::to_string(ip & 0xFF);
	buffer.resize(res);
    return buffer;
}

bool UDPClient::SendData(std::string_view message)
{
	if (message.empty())
		return true;

    if (message.size() > MAX_MSG_SIZE) {
        logger_->Error("UDPClient::SendData message size is too big");
        return false;
    }
    struct sockaddr_in servaddr {};

    // Filling server information 
    servaddr.sin_family = AF_INET; // IPv4 
    //servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_pton(servaddr.sin_family, host_.c_str(), &servaddr.sin_addr.s_addr);
    servaddr.sin_port = htons(port_);

    if (auto res = sendto(socketInfo_->socketId, message.data(), static_cast<int>(message.size()), 0, reinterpret_cast<sockaddr*>(&servaddr), sizeof(servaddr)); res == MY_SOCKET_ERROR) {
		auto error = MY_GET_LAST_ERROR;
		logger_->Error("UDPClient sendto failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
        return false;
    }
    return true;
}
