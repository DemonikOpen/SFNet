#include "src/VoiPServer.hpp"
#include <iostream>

int main()
{
    voip::Server* server = new voip::Server();

    server->Listen(15364);
    std::cout << "Press enter to stop server";
    std::cin.ignore(10000, '\n');

    delete server;

    std::cout << "Exit with success" << std::endl;

    return 0;
}
