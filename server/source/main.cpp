#include <iostream>
#include <string>
#include <functional>

#include "Network/URL.hpp"
#include "Network/HTTP.hpp"
#include "Network/HTTPServer.hpp"
#include "Network/TCPServer.hpp"
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
    Dictionary<ServerStates, std::string> menu_selection_variants = {
        DictItem<ServerStates, std::string>(ServerStates::INIT, "�����"),
        DictItem<ServerStates, std::string>(ServerStates::EXIT, "��室")
    };

    Network::HTTPServer server;
    server.addHandler("/", [](Network::HTTPRequest request) -> Network::HTTPResponse
    {
        Network::HTTPResponse response;
        Network::URI uri(request.start_line["uri"]);
        std::cout << request.toString() << std::endl;

        for (unsigned int i = 0; i < uri.getParamsPtr().getSize(); i++)
            response.body += "<p>" + uri.getParamsPtr().getItemPtr(i).key + ": " + uri.getParamsPtr().getItemPtr(i).value + "</p>";
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
                std::cout << "�롥�� ����⢨�:\n";
                for (unsigned int i = 0; i < menu_selection_variants.getSize(); i++)
                    std::cout << "\t" << i + 1 << ". " + menu_selection_variants.getItemPtr(i).value + ";\n";
                std::cout << "����: ";

                std::string action_str;
                std::cin >> action_str;
                std::cout << std::endl;

                try
                {
                    int action = stoi(action_str) - 1;
                    if ((action >= 0) && (action < menu_selection_variants.getSize()))
                        state = menu_selection_variants.getItemPtr(action).key;
                    else
                        std::cout << "������� �����४⭮� ���祭��!\n";
                }
                catch(...)
                {
                    std::cout << "������� �����४⭮� ���祭��!\n";
                }
            } break;

            case ServerStates::INIT:
            {
                std::string localhost;
                std::cout << "�������� �ࢥ� �� localhost? (\"��\" ��� ���⢥ত����): ";
                std::cin >> localhost;

                server.init(80, localhost == "��");
                server.start(1);

                std::cout << "���� �ࢥ�: " << server.getSelfAddress().toString() << std::endl;

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
                    std::cout << "����� �����: " << session.getId() << "\n"
                    << "�����:\n"
                    << session.getRequest() << "\n"
                    << "�⢥�:\n"
                    << session.getResponse() << "\n"
                    << "============================\n"
                    << "��� ��⠭���� ࠡ��� ������ Space\n\n";
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
                std::cout << "��� �த������� ࠡ��� ������ \"����\", ��� ��室� �� ��㣮� ���祭��: ";
                std::cin >> command;

                if (command == "����")
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