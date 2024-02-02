#pragma once
#include <string>
#include <stdexcept>

#if __has_include(<WinSock2.h>)
#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <WinSock2.h>
#endif
#if __has_include(<ws2tcpip.h>)
#include <ws2tcpip.h>
#endif

#ifdef WIN32
#define MY_INVALID_SOCKET INVALID_SOCKET
#define MY_SOCKET_ERROR SOCKET_ERROR
#define MY_GET_LAST_ERROR WSAGetLastError()
#define MY_GET_ERROR_MESSAGE GetErrorMessage
#define INIT_SOCKET InitSocketCommunicaiton()
#define CLOSESOCKET closesocket
#define MY_ETIMEDOUT WSAETIMEDOUT
#endif
#ifndef WIN32
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

#define MY_INVALID_SOCKET -1
#define MY_SOCKET_ERROR -1
#define MY_GET_LAST_ERROR errno
#define MY_GET_ERROR_MESSAGE std::strerror
#define INIT_SOCKET ;
#define CLOSESOCKET close
#define MY_ETIMEDOUT ETIMEDOUT

#endif 
#ifdef WIN32
namespace Xaba::Network
{
    inline std::string GetErrorMessage(int systemtErrorCode)
    {
        char* message;
        auto msgLength = FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_FROM_HMODULE,
            NULL,
            systemtErrorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&message,
            0,
            NULL
        );

        return std::string(message, msgLength);
    }

    inline void InitSocketCommunicaiton() {
        WSADATA wsaData = {  };
        auto iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            throw std::runtime_error("Error at WSA Init" + std::to_string(iResult));
        }
    }

    inline void CloseSocketCommunicaiton() {
        WSACleanup();
    }
}
#endif