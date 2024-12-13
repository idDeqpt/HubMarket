#include "HTTPServer.hpp"

#include <iostream>
#include <sstream>
#include <string>

#define _WIN32_WINNT 0x501
#include <WinSock2.h>
#include <WS2TCPip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "Address.hpp"
#include "ServerSessionData.hpp"


Network::HTTPResponse Network::default_server_request_handler(HTTPRequest request)
{
    HTTPResponse response;
    
    response.body = "<title>Test C++ HTTP Server</title>\n\
                     <h1>Test page</h1>\n\
                     <a>URI: " + request.start_line["uri"] + "<a>\
                     <p>This is body of the test page...</p>\n\
                     <h2>Request headers</h2>\n\
                     <pre>" + request.toString() + "</pre>\n\
                     <em><small>Test C++ Http Server</small></em>\n";

    response.start_line["http-version"] = "HTTP/1.1";
    response.start_line["status-code"] = "200";
    response.start_line["status-comment"] = "OK";

    response.headers["Version"] = "HTTP/1.1";
    response.headers["Content-Type"] = "text/html; charset=utf-8";
    response.headers["Version"] = "HTTP/1.1";
    response.headers["Content-Length"] = std::to_string(response.body.length());

    return response;
}


Network::HTTPServer::HTTPServer()
{
    last_requested_session_data = 0;
    //available_threads_count = std::thread::hardware_concurrency();
    inited = false;
    started = false;
}

Network::HTTPServer::~HTTPServer()
{
    stop();
    closesocket(this->listen_socket);
    freeaddrinfo(this->addr);
    WSACleanup();
}


int Network::HTTPServer::init(int port, bool localhost)
{
    if (inited)
        return 0;

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << "\n";
        return result;
    }

    this->addr = NULL;
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if (localhost)
    {
        self_address.port = port;
        result = getaddrinfo("127.0.0.1", std::to_string(port).c_str(), &hints, &this->addr);
    }
    else
    {
        initSelfAddress(port);
        result = getaddrinfo(self_address.ip.toString().c_str(), std::to_string(port).c_str(), &hints, &this->addr);
    }

    if (result != 0) {
        std::cerr << "getaddrinfo failed: " << result << "\n";
        WSACleanup(); // выгрузка библиотеки Ws2_32.dll
        return 1;
    }

    this->listen_socket = socket(this->addr->ai_family, this->addr->ai_socktype, this->addr->ai_protocol);
    if (this->listen_socket == INVALID_SOCKET) {
        std::cerr << "Error at socket: " << WSAGetLastError() << "\n";
        freeaddrinfo(this->addr);
        WSACleanup();
        return 1;
    }

    result = bind(this->listen_socket, addr->ai_addr, (int)addr->ai_addrlen);

    if (result == SOCKET_ERROR) {
        std::cerr << "bind failed with error: " << WSAGetLastError() << "\n";
        freeaddrinfo(this->addr);
        closesocket(this->listen_socket);
        WSACleanup();
        return 1;
    }

    if (listen(this->listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen failed with error: " << WSAGetLastError() << "\n";
        closesocket(this->listen_socket);
        WSACleanup();
        return 1;
    }

    inited = true;
}

bool Network::HTTPServer::start(int threads_count)
{
    if (!inited || started)
        return false;
    started = true;

    //if ((threads_count > available_threads_count) || (threads_count == -1))
    //    threads_count = available_threads_count;
    //else if (threads_count < 1)
    //    threads_count = 1;

    //listen_threads.resize(threads_count);
    //for (unsigned int i = 0; i < listen_threads.size(); i++)
    //    listen_threads[i] = std::thread{&HTTPServer::thread_method, this};
    listen_thread = std::thread{&HTTPServer::thread_method, this};

    return true;
}

bool Network::HTTPServer::stop()
{
    if (!inited || !started)
        return false;
    started = false;

    //for (unsigned int i = 0; i < listen_threads.size(); i++)
    //    listen_threads[i].join();
    listen_thread.join();

    return true;
}


void Network::HTTPServer::setRequestHandler(HTTPResponse (*new_request_handler)(HTTPRequest request))
{
    request_handler = new_request_handler;
}


Network::Address Network::HTTPServer::getSelfAddress()
{
    return self_address;
}


bool Network::HTTPServer::hasNewSessionData()
{
   return last_requested_session_data < sessions_data.size();
}

Network::ServerSessionData Network::HTTPServer::getNextSessionData()
{
    return (hasNewSessionData()) ? sessions_data[last_requested_session_data++] : ServerSessionData();
}



void Network::HTTPServer::initSelfAddress(int port)
{
    char hostname[128];  
    hostent * host_info;
    in_addr self_addr;

    gethostname(hostname, 128);
    host_info = gethostbyname(hostname);

    if ((host_info == NULL) || (host_info->h_addr_list[0] == 0))
        self_address = Address();

    self_addr.s_addr = *(u_long*)host_info->h_addr_list[0];
    self_address = Address(IP(self_addr), port);
}

int Network::HTTPServer::listen_method()
{
    if (!started)
        return 0;

    // Принимаем входящие соединения
    int client_socket = accept(this->listen_socket, NULL, NULL);
    if (client_socket == INVALID_SOCKET)
    {
        std::cerr << "accept failed: " << WSAGetLastError() << "\n";
        closesocket(this->listen_socket);
        WSACleanup();
        return 1;
    }

    const int max_client_buffer_size = 1024;
    char buf[max_client_buffer_size];

    int result = recv(client_socket, buf, max_client_buffer_size, 0);
    buf[result] = '\0';
    //std::cout << buf << std::endl;

    if (result == SOCKET_ERROR) {
        // ошибка получения данных
        std::cerr << "recv failed: " << result << "\n";
        closesocket(client_socket);
    } else if (result == 0) {
        // соединение закрыто клиентом
        std::cerr << "connection closed...\n";
    } else if (result > 0) {
        HTTPRequest request = HTTPRequest(std::string(buf));
        HTTPResponse response = request_handler(request);
        std::string response_str = response.toString();

        result = send(client_socket, response_str.c_str(), response_str.length(), 0);
        sessions_data.push_back(ServerSessionData(sessions_data.size(), request.toString(), response_str));

        if (result == SOCKET_ERROR) {
            // произошла ошибка при отправле данных
            std::cerr << "send failed: " << WSAGetLastError() << "\n";
        }
        // Закрываем соединение к клиентом
        closesocket(client_socket);
    }
}

void Network::HTTPServer::thread_method()
{
    while (started)
        listen_method();
}