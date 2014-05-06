#include <iostream>
#include "sfServer.hpp"

sf::TcpServer::TcpServer()
{
    closing = false;
    port = 0;
    this->loopThread = NULL;
}

sf::TcpServer::~TcpServer()
{
    if(!this->closing)
        this->Close();
}

void sf::TcpServer::Close()
{
    {
        sf::Lock lock(closingMutex);
        this->closing = true;
    }

    if(this->loopThread != NULL)
    {
        this->loopThread->wait();
        delete this->loopThread;
        this->loopThread = NULL;
    }
}

void sf::TcpServer::Disconnect(sf::TcpSocket* client)
{
    client->disconnect();
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

    {
        sf::Lock lock(selectorMutex);
        selector.add(listener);
    }


    while(true)
    {
        {
            sf::Lock lock(this->closingMutex);
            if(this->closing)
                break;
        }

        {
            sf::Lock lock(selectorMutex);
            if(selector.wait(sf::seconds(5)))
            {
                if(selector.isReady(listener))
                {
                    sf::TcpSocket* client = new sf::TcpSocket();
                    if(listener.accept(*client) == sf::Socket::Done)
                    {
                        if(this->OnConnectionRequest(*client))
                        {
                            sf::Lock lock(this->clientsMutex);
                            this->clients.push_back(client);
                            selector.add(*client);
                        }else{
                            client->disconnect();
                            delete client;
                        }
                    }else{
                        delete client;
                    }
                }else
                {
                    sf::Lock lock(clientsMutex);
                    // The listener socket is not ready, test all other sockets (the clients)
                    for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
                    {
                        sf::TcpSocket& sender = **it;
                        if (selector.isReady(sender))
                        {
                            // The client has sent some data, we can receive it
                            sf::Packet packet;
                            sf::Socket::Status status = sender.receive(packet);
                            if (status == sf::Socket::Done)
                            {
                                OnDataReceive(sender, packet);
                            }else if(status == sf::Socket::Disconnected)
                            {
                                selector.remove(sender);
                                Disconnect(&sender);
                                delete &sender;
                                clients.erase(it);
                                --it;
                            }
                        }
                    }

                }
            }
        }
    }

    std::cout << "Server closed" << std::endl;
}
