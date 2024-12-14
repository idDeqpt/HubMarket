#include "HTTPServer.hpp"

#include <iostream>

#include "TCPServer.hpp"
#include "HTTP.hpp"
#include "URL.hpp"
#include "Dictionary.hpp"


Network::HTTPResponse Network::default_404_handler()
{
    HTTPResponse response;

    response.body = "<p>404</p>";

    response.start_line["http-version"] = "HTTP/1.1";
    response.start_line["status-code"] = "404";
    response.start_line["status-comment"] = "NOT FOUND";

    response.headers["Version"] = "HTTP/1.1";
    response.headers["Content-Type"] = "text/html; charset=utf-8";
    response.headers["Version"] = "HTTP/1.1";
    response.headers["Content-Length"] = std::to_string(response.body.length());

    return response;
}



Network::HTTPServer::HTTPServer() : TCPServer()
{

}

Network::HTTPServer::~HTTPServer()
{

}


void Network::HTTPServer::set404Handler(HTTPResponse (*new_404_handler)(void))
{
    code_404_handler = new_404_handler;
}


bool Network::HTTPServer::addHandler(std::string path, std::function<HTTPResponse(HTTPRequest)> handler)
{
    return paths_handlers.add(path, handler);
}

bool Network::HTTPServer::removeHandler(std::string path)
{
    return paths_handlers.remove(path);
}


std::string Network::HTTPServer::http_handler(std::string request)
{
    HTTPRequest req(request);
    URI uri(req.start_line["uri"]);
    std::string path = uri.toString(false);

    std::cout << "============uri: " << path << std::endl;

    if (!paths_handlers.has(path))
        return code_404_handler().toString();

    return paths_handlers[path](req).toString();
}

void Network::HTTPServer::client_handler(int client_socket)
{
    //std::cout << "Start " << client_socket << std::endl;
    fd_set read_s;
    timeval time_out;
    time_out.tv_sec = 5;
    FD_ZERO(&read_s);
    FD_SET(client_socket, &read_s);
    if (select(0, &read_s, NULL, NULL, &time_out) > 0)
    {
        const int max_client_buffer_size = 1024;
        char buf[max_client_buffer_size];

        int result = recv(client_socket, buf, max_client_buffer_size, 0);
        buf[result] = '\0';
        std::string request = buf;
        //std::cout << buf << std::endl;

        if (result == SOCKET_ERROR)
            std::cerr << "recv failed: " << result << "\n";
        else if (result == 0)
            std::cerr << "connection closed...\n";
        else if (result > 0)
        {
            std::string response = http_handler(request);
            result = send(client_socket, response.c_str(), response.length(), 0);
            sessions_data.push_back(ServerSessionData(sessions_data.size(), request, response));
            //std::cout << sessions_data.size() << "\n";

            if (result == SOCKET_ERROR)
                std::cerr << "send failed: " << WSAGetLastError() << "\n";
        }
    }

    closesocket(client_socket);
    //std::cout << "Finish " << client_socket << std::endl;
}