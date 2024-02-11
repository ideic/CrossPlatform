#include "TCPClient.h"
#include <vector>
#include <stdexcept>
#include "NetworkHeader.h"
#include <string>
#include <cinttypes>
#include <memory>
#include <utility>
#include <Common/Logger/Logger.h>
#if __has_include(<WinSock2.h>)
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <WinSock2.h>
#endif
#if __has_include(<ws2tcpip.h>)
    #include <ws2tcpip.h>
#endif

using namespace std::string_literals;
using namespace Xaba::Network;


struct TCPClient::SocketInfo {
#ifdef WIN32
	SOCKET socketId{ MY_INVALID_SOCKET };
#else
    int socketId{ MY_INVALID_SOCKET };
#endif
};

TCPClient::TCPClient(std::string host, uint16_t port): mHost(std::move(host)), mPort(port),
    _socketInfo (std::shared_ptr<SocketInfo>(new SocketInfo, [](TCPClient::SocketInfo* socketInfo) {
        CLOSESOCKET(socketInfo->socketId);
    })){
}


std::vector<uint8_t> TCPClient::ReceiveData()
{
    constexpr int MAX_MSG_SIZE = 4096;
    std::vector<uint8_t> buff(MAX_MSG_SIZE);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    const int readByte = recv(_socketInfo->socketId, reinterpret_cast<char*>(buff.data()), static_cast<int>(buff.size()), 0);
    buff.resize(static_cast<size_t>(readByte));
    return buff;
}

void TCPClient::SendData(std::string_view message){
    auto res = send(_socketInfo->socketId, message.data(), static_cast<int>(message.length()), 0);
    if (static_cast<size_t>(res) != message.length()) {
		auto error = MY_GET_LAST_ERROR;
		throw std::runtime_error("TCP Send failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
	}
}

void TCPClient::Connect() {
    INIT_SOCKET;
    struct sockaddr_in servaddr {};

    // socket create and varification
    _socketInfo->socketId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socketInfo->socketId == MY_INVALID_SOCKET) {
        auto error = MY_GET_LAST_ERROR;
        throw std::runtime_error("TCP Socket Init failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
    }

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    inet_pton(servaddr.sin_family, mHost.c_str(), &servaddr.sin_addr.s_addr);
    // servaddr.sin_addr.s_addr = ( inet_addr(_host.c_str());
    servaddr.sin_port = htons(mPort);

    // connect the client socket to server socket
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    if (connect(_socketInfo->socketId, reinterpret_cast<sockaddr*>(&servaddr), sizeof(servaddr)) == MY_SOCKET_ERROR) {
        auto error = MY_GET_LAST_ERROR;
        throw std::runtime_error("TCP Connect failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
    }
}

