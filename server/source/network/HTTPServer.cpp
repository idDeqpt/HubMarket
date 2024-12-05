#include "HTTPServer.hpp"

#include <iostream>
#include <sstream>
#include <string>

#define _WIN32_WINNT 0x501
#include <WinSock2.h>
#include <WS2TCPip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "Address.hpp"


std::string Network::default_server_request_handler(std::string request)
{
    //return query;

    std::stringstream response; // сюда будет записываться ответ клиенту
    std::stringstream response_body; // тело ответа

    // Данные успешно получены
    // формируем тело ответа (HTML)
    response_body << "<title>Test C++ HTTP Server</title>\n"
        << "<h1>Test page</h1>\n"
        << "<p>This is body of the test page...</p>\n"
        << "<h2>Request headers</h2>\n"
        << "<pre>" << request << "</pre>\n"
        << "<em><small>Test C++ Http Server</small></em>\n";

    // Формируем весь ответ вместе с заголовками
    response << "HTTP/1.1 200 OK\r\n"
        << "Version: HTTP/1.1\r\n"
        << "Content-Type: text/html; charset=utf-8\r\n"
        << "Content-Length: " << response_body.str().length()
        << "\r\n\r\n"
        << response_body.str();
    return response.str();
}


Network::HTTPServer::HTTPServer()
{

}

Network::HTTPServer::~HTTPServer()
{
    closesocket(this->listen_socket);
    freeaddrinfo(this->addr);
    WSACleanup();
}


Network::Address Network::HTTPServer::getSelfAddress()
{
    return self_address;
}


int Network::HTTPServer::init(int port, bool localhost)
{
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
        result = getaddrinfo("127.0.0.1", std::to_string(port).c_str(), &hints, &this->addr);
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
}

void Network::HTTPServer::setRequestHandler(std::string (*new_request_handler)(std::string))
{
    request_handler = new_request_handler;
}


int Network::HTTPServer::listen_s()
{
    // Принимаем входящие соединения
    int client_socket = accept(this->listen_socket, NULL, NULL);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "accept failed: " << WSAGetLastError() << "\n";
        closesocket(this->listen_socket);
        WSACleanup();
        return 1;
    }

    const int max_client_buffer_size = 1024;
    char buf[max_client_buffer_size];

    int result = recv(client_socket, buf, max_client_buffer_size, 0);
    buf[result] = '\0';
    std::cout << buf << std::endl;

    if (result == SOCKET_ERROR) {
        // ошибка получения данных
        std::cerr << "recv failed: " << result << "\n";
        closesocket(client_socket);
    } else if (result == 0) {
        // соединение закрыто клиентом
        std::cerr << "connection closed...\n";
    } else if (result > 0) {
        
        std::string response = request_handler(std::string(buf));
        // Отправляем ответ клиенту с помощью функции send
        result = send(client_socket, response.c_str(), response.length(), 0);

        if (result == SOCKET_ERROR) {
            // произошла ошибка при отправле данных
            std::cerr << "send failed: " << WSAGetLastError() << "\n";
        }
        // Закрываем соединение к клиентом
        closesocket(client_socket);
    }
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