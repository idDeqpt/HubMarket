#include "TCPClient.hpp"

#include <string>
#include <iostream>

#define _WIN32_WINNT 0x501
#include <WinSock2.h>
#include <WS2TCPip.h>
#pragma comment(lib, "Ws2_32.lib")


Network::TCPClient::TCPClient()
{
	server_socket = NULL;
}

Network::TCPClient::~TCPClient()
{
	this->close();
	WSACleanup();
}


int Network::TCPClient::connect(std::string host, int port)
{
	WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cerr << "WSAStartup failed: " << result << "\n";
        return result;
    }

	struct hostent *host_info = gethostbyname(host.c_str());
	if (host_info == nullptr)
		return 1;

	in_addr addr = *(in_addr*)host_info->h_addr_list[0];
	return this->connect(Address(IP(addr), port));
}

int Network::TCPClient::connect(Address address)
{
	if (this->server_socket != NULL)
		this->close();
	std::cout << "connect to " << address.toString() << std::endl;

	WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cerr << "WSAStartup failed: " << result << "\n";
        return result;
    }

    this->server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->server_socket == INVALID_SOCKET) {
        std::cerr << "Error at socket: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address.ip.toString().c_str());
    server_addr.sin_port = htons(address.port);

    if (::connect(this->server_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
    {
        std::cerr << "Error at connect: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    return 0;
}

bool Network::TCPClient::close()
{
	if (this->server_socket == NULL)
		return false;

	closesocket(this->server_socket);
    this->server_socket == NULL;

    return true;
}


void Network::TCPClient::send(std::string message)
{
	if (this->server_socket != NULL)
		::send(this->server_socket, message.c_str(), message.length(), NULL);
}

std::string Network::TCPClient::recv()
{
	if (this->server_socket == NULL)
		return "";

	/*
	const int max_buffer_size = 1024;
	char buf[max_buffer_size];
	int bytes = ::recv(this->server_socket, buf, max_buffer_size, NULL);
	buf[bytes] = '\0';
	std::cout << buf << std::endl;
	*/

	std::string return_data;
	const int max_buffer_size = 1024;
	char buf[max_buffer_size];
	while (::recv(this->server_socket, buf, max_buffer_size, NULL) > 0)
	{
		//std::cout << "BUF: " << buf << std::endl;
        for (int i = 0; buf[i] >= 32 || buf[i] == '\n' || buf[i] == '\r'; i++)
            return_data += buf[i];
	}

	return return_data;
}