#include "sfClient.hpp"
#include <iostream>

sf::TcpClient::TcpClient()
{
    isConnected = false;
    this->loopThread = NULL;
    closing = false;
}

sf::TcpClient::~TcpClient(){
    if(!this->closing)
        this->Disconnect();
}

bool sf::TcpClient::Connect(const IpAddress& remoteAddress, unsigned short remotePort, sf::Time timeout)
{
    sf::Socket::Status status = socket.connect(remoteAddress, remotePort, timeout);
    this->isConnected = status == sf::Socket::Status::Done;

    if(this->isConnected)
    {
        this->OnConnection();

        this->loopThread = new sf::Thread(&sf::TcpClient::loop, &(*this));
        this->loopThread->launch();

        return true;
    }

    return false;
}

void sf::TcpClient::Disconnect()
{
    {
        sf::Lock lock(closingMutex);
        this->isConnected = false;
        this->closing = true;
    }
    {
        sf::Lock lock(socketMutex);
        socket.disconnect();
    }

    if(this->loopThread != NULL)
    {
        this->loopThread->wait();
        delete this->loopThread;
        this->loopThread = NULL;
    }
}

bool sf::TcpClient::SendData(sf::Packet data)
{
    sf::Socket::Status status = socket.send(data);
    return status == sf::Socket::Status::Done;
}

void sf::TcpClient::loop()
{
    sf::SocketSelector selector;
    {
        sf::Lock lock(this->socketMutex);
        selector.add(this->socket);
    }

    while(true)
    {
        {
            sf::Lock lock(this->closingMutex);
            if(this->closing)
                break;
        }

        if(selector.wait(sf::seconds(5)))
        {
            {
                sf::Lock lock(socketMutex);
                if(selector.isReady(this->socket))
                {
                    sf::Packet data;
                    sf::Socket::Status status = this->socket.receive(data);
                    if(status == sf::Socket::Done)
                    {
                        this->OnDataReceive(data);
                    }else if(status == sf::Socket::Disconnected)
                    {
                        std::cout << "Connection lost" << std::endl;
                        break;
                    }
                }
            }
        }

    }
}
