#include "VoiPClient.hpp"

voip::Client::Client()
{
    audioStream = new voip::AudioStream();
    audioRecorder = new voip::AudioRecorder(*this);
}

voip::Client::~Client()
{
    if(audioStream != NULL)
    {
        delete audioStream;
        audioStream = NULL;
    }
    if(audioRecorder != NULL)
    {
        delete audioRecorder;
        audioRecorder = NULL;
    }
}

void voip::Client::OnConnection()
{
    audioStream->play();
    audioRecorder->start(44100);
}

void voip::Client::Disconnect()
{
    audioStream->pause();
    audioRecorder->stop();
    sf::TcpClient::Disconnect();
}

void voip::Client::OnDataReceive(sf::Packet packet)
{
    sf::Uint8 id;
    packet >> id;

    if (id == audioData)
    {
        std::cout << "Receiving audio..." << std::endl;
        const sf::Int16* samples = reinterpret_cast<const sf::Int16*>(static_cast<const char*>(packet.getData()) + 1);
        std::size_t sampleCount = (packet.getDataSize() - 1) / sizeof(sf::Int16);

        audioStream->SetSamples(samples, sampleCount);
    }
    else if (id == endOfStream)
    {
        std::cout << "Audio data has been 100% received!" << std::endl;
    }
    else
    {
        std::cout << "Invalid packet received..." << std::endl;
    }
}
