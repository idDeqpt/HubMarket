#include <iostream>

#include "Network/URL.hpp"
#include "Network/HTTPServer.hpp"
#include "Dictionary.hpp"


int main()
{
    Network::HTTPServer server;
    server.init(80);

    Network::URL url("http", Network::Address(Network::IP(172, 26, 144, 1), 80), "api/test");
    //Dictionary<std::string, std::string>* params_ptr = url.getParamsPtr();
    url.getParamsPtr()["param1"] = "value1";
    url.getParamsPtr()["param123"] = "value123";

    std::cout << url.toString() << std::endl;
    std::string str = "123.43.23.77:8000";
    Network::Address addr = Network::Address(str);
    std::cout << addr.toString() << std::endl;

    std::cout << server.getSelfAddress().toString() << std::endl;

    while (true)
        server.listen_s();

    system("pause");
    return 0;
}