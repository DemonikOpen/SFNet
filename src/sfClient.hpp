#ifndef SFCLIENT_HPP_INCLUDED
#define SFCLIENT_HPP_INCLUDED

#include <SFML/Network.hpp>

namespace sf{
    class TcpClient
    {
    private:
        bool closing;
        sf::Thread* loopThread;
        sf::Mutex socketMutex, closingMutex;
        void loop();
    protected:
        sf::TcpSocket socket;
        virtual void OnConnection() = 0;
        virtual void OnDataReceive(sf::Packet data) = 0;
        bool isConnected;
    public:
        TcpClient();
        virtual ~TcpClient();
        bool Connect(const IpAddress& remoteAddress, unsigned short remotePort, sf::Time timeout = sf::Time::Zero);
        virtual void Disconnect();
        bool SendData(sf::Packet data);
    };
}

#endif // SFCLIENT_HPP_INCLUDED
