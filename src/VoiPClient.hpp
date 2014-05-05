#ifndef VOIPCLIENT_HPP_INCLUDED
#define VOIPCLIENT_HPP_INCLUDED

#include "sfClient.hpp"
#include "sfAudioStream.hpp"
#include "sfAudioRecorder.hpp"

namespace voip{
    class Client : public sf::TcpClient{
    private:
        AudioStream* audioStream;
        AudioRecorder* audioRecorder;
    protected:
        void OnDataReceive(sf::Packet data) override;
    public:
        Client();
        ~Client();
    };
}

#endif // VOIPCLIENT_HPP_INCLUDED
