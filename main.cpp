#include "src/VoiPServer.hpp"
#include "src/VoiPClient.hpp"
#include <iostream>

int main(int argc,char *argv[])
{
    if(atoi(argv[1]) == 1)
    {
        voip::Server* server = new voip::Server();

        server->Listen(15364);
        std::cout << "Press enter to stop server";
        std::cin.ignore(10000, '\n');

        delete server;
    }

    if(atoi(argv[1]) == 2)
    {
        voip::Client* client = new voip::Client();

        std::cout << "Connection to localhost" << std::endl;
        if(!client->Connect("127.0.0.1", 15364))
        {
            std::cout << "Error establishing connection" << std::endl;
            return -1;
        }

        std::cout << "Press enter to stop client";
        std::cin.ignore(10000, '\n');

        delete client;
    }

    std::cout << "Exit with success" << std::endl;

    return 0;
}
