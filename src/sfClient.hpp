#ifndef SFCLIENT_HPP_INCLUDED
#define SFCLIENT_HPP_INCLUDED

#include <SFML/Network.hpp>

namespace sf{
    class TcpClient
    {
    private:

    protected:
        sf::TcpSocket socket;
        virtual void OnDataReceive(const sf::Packet data) = 0;
    public:
        bool Connect(const IpAddress& remoteAddress, unsigned short remotePort, sf::Time timeout = sf::Time::Zero);
    };
}

#endif // SFCLIENT_HPP_INCLUDED
