#include <iostream>

#include "Network/URL.hpp"
#include "Network/HTTP.hpp"
#include "Network/HTTPServer.hpp"
#include "Dictionary.hpp"




int main()
{
    Network::HTTPServer server;
    server.init(80, true);

    Network::HTTPRequest req("GET / HTTP/1.1\r\nHost: 172.26.144.1\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Safari/537.36 OPR/114.0.0.0 (Edition Yx GX 03)\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7");
    std::string str = "GET / HTTP/1.1\r\n\
Host: localhost\r\n\
Connection: keep-alive\r\n\
Cache-Control: max-age=0\r\n\
sec-ch-ua: \"Chromium\";v=\"128\", \"Not;A=Brand\";v=\"24\", \"Opera GX\";v=\"114\"\r\n\
sec-ch-ua-mobile: ?0\r\n\
sec-ch-ua-platform: \"Windows\"\r\n\
Upgrade-Insecure-Requests: 1\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Safari/537.36 OPR/114.0.0.0 (Edition Yx GX 03)\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n\
Sec-Fetch-Site: none\r\n\
Sec-Fetch-Mode: navigate\r\n\
Sec-Fetch-User: ?1\r\n\
Sec-Fetch-Dest: document\r\n\
Accept-Encoding: gzip, deflate, br, zstd\r\n\
Accept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7\r\n\
\r\n\r\n\
body dATA lalala!";
    Network::HTTPRequest req(str);

    std::cout << "method: " << req.start_line["method"] << "@" << std::endl;
    std::cout << "uri: " << req.start_line.getItemPtr(1).key << "@" << std::endl;
    std::cout << "version: " << req.start_line["http-version"] << "@" << std::endl;
    std::cout << "headers count: " << req.headers.getSize() << std::endl;
    for (unsigned int i = 0; i < req.headers.getSize(); i++)
        std::cout << "@" << req.headers.getItemPtr(i).key << "@: @" << req.headers.getItemPtr(i).value << "@\n";
    std::cout << "body content: @" << req.body << "@" << std::endl;
    //std::cout << req.toString() << std::endl;

    std::cout << server.getSelfAddress().toString() << std::endl;

    while (true)
        server.listen_s();

    system("pause");
    return 0;
}