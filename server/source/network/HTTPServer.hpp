#ifndef NETWORK_HTTP_SERVER
#define NETWORK_HTTP_SERVER
  
#define _WIN32_WINNT 0x501
#include <WinSock2.h>
#include <WS2TCPip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <string>
#include <thread>

#include "Address.hpp"
#include "HTTP.hpp"


namespace Network
{
    HTTPResponse default_server_request_handler(HTTPRequest request);

    class HTTPServer
    {
    public:
        HTTPServer();
        ~HTTPServer();

        int init(int port, bool localhost = false);
        void setRequestHandler(HTTPResponse (*new_request_handler)(HTTPRequest));

        Address getSelfAddress();

        int listen_s();

    protected:
        int listen_socket;
        struct addrinfo* addr;
        Address self_address;
        HTTPResponse (*request_handler)(HTTPRequest) = default_server_request_handler;

        void initSelfAddress(int port);
    };
}

#endif //NETWORK_HTTP_SERVER