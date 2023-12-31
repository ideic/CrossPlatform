#include "TCPClient.h"
#include <functional>
#include <vector>

#include "NetworkHeader.h"
#include <iostream>

using namespace std::string_literals;
using namespace Xaba::Network;


struct TCPClient::SocketInfo {
#ifdef WIN32
	SOCKET socketId{ MY_INVALID_SOCKET };
#else
    int socketId{ MY_INVALID_SOCKET };
#endif
};

TCPClient::TCPClient(std::string host, uint16_t port): _host(std::move(host)), _port(std::move(port)){
    _socketInfo = std::shared_ptr<SocketInfo>(new SocketInfo, [](TCPClient::SocketInfo* socketInfo) {
        CLOSESOCKET(socketInfo->socketId);
    });
}


std::vector<uint8_t> TCPClient::ReceiveData()
{
    std::vector<uint8_t> buff(4096);
    int readByte = recv(_socketInfo->socketId, (char*)buff.data(), static_cast<int>(buff.size()), 0);
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
    inet_pton(servaddr.sin_family, _host.c_str(), &servaddr.sin_addr.s_addr);
    // servaddr.sin_addr.s_addr = ( inet_addr(_host.c_str());
    servaddr.sin_port = htons(_port);

    // connect the client socket to server socket
    if (connect(_socketInfo->socketId, (sockaddr*)&servaddr, sizeof(servaddr)) == MY_SOCKET_ERROR) {
        auto error = MY_GET_LAST_ERROR;
        throw std::runtime_error("TCP Connect failed:"s + std::to_string(error) + " Reason: "s + MY_GET_ERROR_MESSAGE(error));
    }
}

