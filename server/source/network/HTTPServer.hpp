#ifndef NETWORK_HTTP_SERVER
#define NETWORK_HTTP_SERVER
  
#define _WIN32_WINNT 0x501
#include <WinSock2.h>
#include <WS2TCPip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <string>
#include <thread>

#include "Address.hpp"


namespace Network
{
    std::string default_server_request_handler(std::string query);

    class HTTPServer
    {
    public:
        HTTPServer();
        ~HTTPServer();

        int init(int port, bool localhost = false);
        void setRequestHandler(std::string (*new_request_handler)(std::string));

        Address getSelfAddress();

        int listen_s();

    protected:
        int listen_socket;
        struct addrinfo* addr;
        Address self_address;
        std::string (*request_handler)(std::string) = default_server_request_handler;

        void initSelfAddress(int port);
    };
}

#endif //NETWORK_HTTP_SERVER