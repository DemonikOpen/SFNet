#ifndef SFAUDIORECORDER_HPP_INCLUDED
#define SFAUDIORECORDER_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>


const sf::Uint8 audioData = 1;
const sf::Uint8 endOfStream = 2;

namespace sf{
    ////////////////////////////////////////////////////////////
    /// Specialization of audio recorder for sending recorded audio
    /// data through the network
    ////////////////////////////////////////////////////////////
    class NetworkRecorder : public sf::SoundRecorder
    {
    public :

        ////////////////////////////////////////////////////////////
        /// Constructor
        ///
        /// \param host Remote host to which send the recording data
        /// \param port Port of the remote host
        ///
        ////////////////////////////////////////////////////////////
        NetworkRecorder(const sf::TcpSocket& socket) :
        m_socket(socket)
        {
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

            // Send the audio packet to the server
            return m_socket.send(packet) == sf::Socket::Done;
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
            m_socket.send(packet);
        }

        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        sf::TcpSocket m_socket; ///< Socket used to communicate with the server
    };
}
#endif // SFAUDIORECORDER_HPP_INCLUDED
