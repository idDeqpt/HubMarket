#include <iostream>

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

    Network::TCPServer server;

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
                /*
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
                */
                
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