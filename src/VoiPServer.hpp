#ifndef VOIPSERVER_HPP_INCLUDED
#define VOIPSERVER_HPP_INCLUDED

#include "sfServer.hpp"
#include "sfAudioStream.hpp"

namespace voip{
    class Server : public sf::TcpServer{
    private:

    protected:
        bool OnConnectionRequest(const sf::TcpSocket& client) override;
        void OnDataReceive(const sf::TcpSocket& client, const sf::Packet data) override;
    public:
        Server();
        ~Server(){};
    };
}

#endif // VOIPSERVER_HPP_INCLUDED

/*



*/
