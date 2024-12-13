#ifndef NETWORK_HTTP_SERVER
#define NETWORK_HTTP_SERVER
  
#define _WIN32_WINNT 0x501
#include <WinSock2.h>
#include <WS2TCPip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <string>
#include <thread>
#include <vector>

#include "Address.hpp"
#include "HTTP.hpp"
#include "ServerSessionData.hpp";


namespace Network
{
    HTTPResponse default_server_request_handler(HTTPRequest request);

    class HTTPServer
    {
    public:
        HTTPServer();
        ~HTTPServer();

        int init(int port, bool localhost = false);
        bool start(int threads_count = -1);
        bool stop();

        void setRequestHandler(HTTPResponse (*new_request_handler)(HTTPRequest));

        Address getSelfAddress();

        bool hasNewSessionData();
        ServerSessionData getNextSessionData();

    protected:
        bool inited, started;
        int listen_socket;
        struct addrinfo* addr;
        Address self_address;
        int last_requested_session_data;
        std::vector<ServerSessionData> sessions_data;
        //unsigned int available_threads_count;
        //std::vector<std::thread> listen_threads
        std::thread listen_thread;
        HTTPResponse (*request_handler)(HTTPRequest) = default_server_request_handler;

        void initSelfAddress(int port);
        int listen_method();
        void thread_method();
    };
}

#endif //NETWORK_HTTP_SERVER