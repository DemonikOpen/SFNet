#include "sfClient.hpp"

bool sf::TcpClient::Connect(const IpAddress& remoteAddress, unsigned short remotePort, sf::Time timeout = sf::Time::Zero)
{
    sf::Socket::Status status = socket.connect(remoteAddress, remotePort, timeout);
    return status == sf::Socket::Status::Done;
}
