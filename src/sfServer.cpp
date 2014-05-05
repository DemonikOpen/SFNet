#include <iostream>
#include "sfServer.hpp"

sf::TcpServer::TcpServer()
{
    closing = false;
    this->loopThread = NULL;
}

sf::TcpServer::~TcpServer()
{
    {
        sf::Lock lock(closingMutex);
        closing = true;
    }
    if(this->loopThread != NULL)
    {
        this->loopThread->wait();
        delete this->loopThread;
    }
}

const bool sf::TcpServer::Listen(unsigned short port)
{
    this->port = port;

    this->loopThread = new sf::Thread(&sf::TcpServer::loop, &(*this));
    this->loopThread->launch();

    return true;
}

void sf::TcpServer::loop()
{
    TcpListener listener;

    sf::Socket::Status status = listener.listen(this->port);
    if(status != sf::Socket::Status::Done){
        std::cerr << "Error while trying to listen on port " << this->port << std::endl << std::flush;
        return;
    }

    sf::SocketSelector selector;
    selector.add(listener);

    while(true)
    {
        {
            sf::Lock lock(this->closingMutex);
            if(this->closing)
                break;
        }

        if(selector.wait(sf::seconds(5)))
        {
            if(selector.isReady(listener))
            {
                sf::TcpSocket* client = new sf::TcpSocket();
                if(listener.accept(*client) == sf::Socket::Done)
                {
                    {
                        sf::Lock lock(this->clientsMutex);
                        this->clients.push_back(client);
                        selector.add(*client);
                    }
                }else{
                    delete client;
                }
            }else
            {
                sf::Lock lock(clientsMutex);
                 // The listener socket is not ready, test all other sockets (the clients)
                for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
                {
                    sf::TcpSocket& sender = **it;
                    if (selector.isReady(sender))
                    {
                        // The client has sent some data, we can receive it
                        sf::Packet packet;
                        if (sender.receive(packet) == sf::Socket::Done)
                        {
                            OnDataReceive(sender, packet);
                        }
                    }
                }
            }
        }
    }

    std::cout << "Server closed" << std::endl;
}
