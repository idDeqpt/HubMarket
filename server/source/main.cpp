#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>
#include <utility>

#include "Network/URL.hpp"
#include "Network/HTTP.hpp"
#include "Network/HTTPServer.hpp"
#include "Network/HTTPRequests.hpp"
#include "Network/TCPServer.hpp"
#include "Network/TCPClient.hpp"
#include "Network/ServerSessionData.hpp"
#include "Dictionary.hpp"
#include "Timer.hpp"
#include <windows.h>


int main()
{
    enum ServerStates
    {
        CHOOSE_STATE = 0,
        INIT,
        EXIT,
        PROCESS,
        PAUSE,
    } state = ServerStates::CHOOSE_STATE;

    std::vector<std::pair<ServerStates, std::string>> menu_selection_variants = {
        {ServerStates::INIT, "Запуск"},
        {ServerStates::EXIT, "Выход"}
    };


    std::cout << "|" << Network::Requests::get("http://localhost/?text=c%2B%2B+pair+какой+заголовок&lr=100932&clid=2437996") << "|" << std::endl;

    Network::HTTPServer server;
    server.addHandler("/", [](Network::HTTPRequest request) -> Network::HTTPResponse
    {
        Network::HTTPResponse response;
        Network::URI uri(request.start_line["uri"]);
        std::cout << request.toString() << std::endl;

        for (auto& [key, value] : uri.getParamsPtr())
            response.body += "<p>" + key + ": " + value + "</p>";
        response.body += "<p>success</p>";

        response.start_line["http-version"] = "HTTP/1.1";
        response.start_line["status-code"] = "200";
        response.start_line["status-comment"] = "OK";

        response.headers["Version"] = "HTTP/1.1";
        response.headers["Content-Type"] = "text/html; charset=utf-8";
        response.headers["Version"] = "HTTP/1.1";
        response.headers["Content-Length"] = std::to_string(response.body.length());

        return response;
    });

    Timer timer;
    while (state != ServerStates::EXIT)
    {
        switch(state)
        {
            case ServerStates::CHOOSE_STATE:
            {
                std::cout << "Выберите действие:\n";
                for (unsigned int i = 0; i < menu_selection_variants.size(); i++)
                    std::cout << "\t" << i + 1 << ". " + menu_selection_variants[i].second + ";\n";
                std::cout << "Ввод: ";

                std::string action_str;
                std::cin >> action_str;
                std::cout << std::endl;

                try
                {
                    int action = stoi(action_str) - 1;
                    if ((action >= 0) && (action < menu_selection_variants.size()))
                        state = menu_selection_variants[action].first;
                    else
                        std::cout << "Введено некорректное значение!\n";
                }
                catch(...)
                {
                    std::cout << "Введено некорректное значение!\n";
                }
            } break;

            case ServerStates::INIT:
            {
                std::string localhost;
                std::cout << "Запустить сервер на localhost? (\"да\" для подтверждения): ";
                std::cin >> localhost;

                server.init(80, localhost == "да");
                server.start(1);

                std::cout << "Адрес сервера: " << server.getSelfAddress().toString() << std::endl;

                state = ServerStates::PROCESS;
            } break;

            case ServerStates::EXIT:
            {
                std::cout << "exit\n";
            } break;

            case ServerStates::PROCESS:
            {
                if (server.hasNewSessionData())
                {
                    Network::ServerSessionData session = server.getNextSessionData();
                    std::cout << "Номер запроса: " << session.getId() << "\n"
                    << "Запрос:\n"
                    << session.getRequest() << "\n"
                    << "Ответ:\n"
                    << session.getResponse() << "\n"
                    << "============================\n"
                    << "Для остановки работы нажмите Space\n\n";
                }

                //std::cout << "a\n";
                if (GetAsyncKeyState(VK_SPACE) < 0)
                    state = ServerStates::PAUSE;
                timer.sleep(16);
            } break;

            case ServerStates::PAUSE:
            {
                server.stop();
                std::string command;
                std::cout << "Для продолжения работы введите \"старт\", для выхода любое другое значение: ";
                std::cin >> command;

                if (command == "старт")
                {
                    server.start(1);
                    state = ServerStates::PROCESS;
                }
                else
                    state = ServerStates::EXIT;
            } break;

            default:
            {
                state = ServerStates::CHOOSE_STATE;
            } break;
        }
    }

    system("pause");
    return 0;
}