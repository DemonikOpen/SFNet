#ifndef SFAUDIORECORDER_HPP_INCLUDED
#define SFAUDIORECORDER_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>

#ifndef PACKET_INDEXES
#define PACKET_INDEXES
const sf::Uint8 audioData = 1;
const sf::Uint8 endOfStream = 2;
#endif

namespace voip{
    ////////////////////////////////////////////////////////////
    /// Specialization of audio recorder for sending recorded audio
    /// data through the network
    ////////////////////////////////////////////////////////////
    class AudioRecorder : public sf::SoundRecorder
    {
    public :

        ////////////////////////////////////////////////////////////
        /// Constructor
        ///
        /// \param host Remote host to which send the recording data
        /// \param port Port of the remote host
        ///
        ////////////////////////////////////////////////////////////
        AudioRecorder(sf::TcpClient& client)
        {
            m_client = &client;
        }

    private :

        ////////////////////////////////////////////////////////////
        /// /see SoundRecorder::OnStart
        ///
        ////////////////////////////////////////////////////////////
        virtual bool onStart()
        {
            return true;
        }

        ////////////////////////////////////////////////////////////
        /// /see SoundRecorder::ProcessSamples
        ///
        ////////////////////////////////////////////////////////////
        virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
        {
            // Pack the audio samples into a network packet
            sf::Packet packet;
            packet << audioData;
            packet.append(samples, sampleCount * sizeof(sf::Int16));

            std::cout << "Sending audio..." << std::endl;

            // Send the audio packet to the server
            return m_client->SendData(packet);
        }

        ////////////////////////////////////////////////////////////
        /// /see SoundRecorder::OnStop
        ///
        ////////////////////////////////////////////////////////////
        virtual void onStop()
        {
            // Send a "end-of-stream" packet
            sf::Packet packet;
            packet << endOfStream;
            m_client->SendData(packet);
        }

        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        sf::TcpClient* m_client; ///< Socket used to communicate with the server
    };
}
#endif // SFAUDIORECORDER_HPP_INCLUDED
