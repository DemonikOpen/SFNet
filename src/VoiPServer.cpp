#include "VoiPServer.hpp"
#include <iostream>

voip::Server::Server() : sf::TcpServer()
{

}

void voip::Server::OnDataReceive(const sf::TcpSocket& client, sf::Packet data)
{
    sf::Packet pack;
    pack << data.getData();

    sf::Uint8 id;
    pack >> id;

    if(id == audioData)
    {
        for (std::vector<sf::TcpSocket*>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
        {
            sf::TcpSocket& receiver = **it;
            if(&receiver != &client)
            {
                receiver.send(data);
            }
        }
    }else if(id == endOfStream)
    {
        this->Disconnect((sf::TcpSocket*)&client);
    }
}

void voip::Server::Disconnect(sf::TcpSocket* client)
{
    std::cout << "Client Disconnection" << std::endl;
    sf::TcpServer::Disconnect(client);
}

bool voip::Server::OnConnectionRequest(const sf::TcpSocket& client)
{
    std::cout << "Incomming connection" << std::endl;
    return true;
}
