#include "VoiPServer.hpp"

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
            receiver.send(data);
        }
    }
}

bool voip::Server::OnConnectionRequest(const sf::TcpSocket& client)
{
    return true;
}
