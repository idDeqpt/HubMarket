#include <iostream>

#include "Network/URL.hpp"
#include "Network/HTTP.hpp"
#include "Network/HTTPServer.hpp"
#include "Dictionary.hpp"


int main()
{
    Network::HTTPServer server;
    server.init(80);

    Network::HTTPRequest req("GET / HTTP/1.1\r\nHost: 172.26.144.1\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Safari/537.36 OPR/114.0.0.0 (Edition Yx GX 03)\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7");

    std::cout << req.headers.getItemPtr(0).key << "@" << req.headers.getItemPtr(0).value << std::endl;
    std::cout << req.toString() << std::endl;

    std::cout << server.getSelfAddress().toString() << std::endl;

    while (true)
        server.listen_s();

    system("pause");
    return 0;
}