#ifndef SFSERVER_H_INCLUDED
#define SFSERVER_H_INCLUDED

#include <vector>
#include <SFML/Network.hpp>

namespace sf{

    class TcpServer{
    private:
        sf::SocketSelector selector;
        sf::Thread* loopThread;
        sf::Mutex closingMutex, clientsMutex, selectorMutex;
        bool closing;
        unsigned short port;
        void loop();
    protected:
        std::vector<sf::TcpSocket*> clients;

        virtual bool OnConnectionRequest(const sf::TcpSocket& client) = 0;
        virtual void OnDataReceive(const sf::TcpSocket& client, const sf::Packet data) = 0;
    public:
        TcpServer();
        virtual ~TcpServer();
        const bool Listen(unsigned short port);
        virtual void Disconnect(sf::TcpSocket* client);
        void Close();
    };
}

#endif // SFSERVER_H_INCLUDED
