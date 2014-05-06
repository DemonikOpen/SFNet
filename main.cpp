#include "src/VoiPServer.hpp"
#include "src/VoiPClient.hpp"
#include <iostream>

int main(int argc,char *argv[])
{

    if(atoi(argv[1]) == 1)
    {
        voip::Server* server = new voip::Server();

        if(!server->Listen(15364))
        {
            std::cout << "Error trying to listen on port 15364" << std::endl;
            return -1;
        }
        std::cout << "Listening on port 15364" << std::endl;
        std::cout << "Press enter to stop server" << std::endl;
        std::cin.ignore(10000, '\n');

        server->Close();

        delete server;
    }

    if(atoi(argv[1]) == 2)
    {
        voip::Client* client = new voip::Client();

        std::string host;
        std::cout << "Enter the host you want to connect to: ";
        std::cin >> host;
        std::cin.ignore(10000, '\n');

        std::cout << "Connecting to " << host << " on port 15634" << std::endl;
        if(!client->Connect(host, 15364))
        {
            std::cout << "Error establishing connection" << std::endl;
            return -1;
        }

        std::cout << "Connection established" << std::endl;
        std::cout << "Press enter to stop client" << std::endl;
        std::cin.ignore(10000, '\n');

        client->Disconnect();

        delete client;
    }

    std::cout << "Exit with success" << std::endl;

    return 0;
}
