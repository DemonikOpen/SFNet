#include "VoiPServer.hpp"
#include <iostream>

voip::Server::Server() : sf::TcpServer()
{

}

void voip::Server::OnDataReceive(const sf::TcpSocket& client, sf::Packet data)
{

    for (std::list<sf::TcpSocket*>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
    {
        sf::TcpSocket& receiver = **it;
        if(&receiver != &client)
        {
            std::cout << "Sending data to.." << std::endl;
            receiver.send(data);
        }
    }
}

bool voip::Server::OnConnectionRequest(const sf::TcpSocket& client)
{
    return true;
}
