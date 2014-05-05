#ifndef SFAUDIOSTREAM_HPP_INCLUDED
#define SFAUDIOSTREAM_HPP_INCLUDED

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iomanip>
#include <iostream>
#include <iterator>


#ifndef PACKET_INDEXES
#define PACKET_INDEXES
const sf::Uint8 audioData = 1;
const sf::Uint8 endOfStream = 2;
#endif

namespace voip{

    ////////////////////////////////////////////////////////////
    /// Customized sound stream for acquiring audio data
    /// from the network
    ////////////////////////////////////////////////////////////
    class AudioStream : public sf::SoundStream
    {
    public :

        ////////////////////////////////////////////////////////////
        /// Default constructor
        ///
        ////////////////////////////////////////////////////////////
        AudioStream() :
        m_offset (0)
        {
            // Set the sound parameters
            initialize(1, 44100);
        }

        void SetSamples(const sf::Int16* samples, std::size_t sampleCount)
        {
            sf::Lock lock(m_mutex);
            std::copy(samples, samples + sampleCount, std::back_inserter(m_samples));
        }

    private :

        ////////////////////////////////////////////////////////////
        /// /see SoundStream::OnGetData
        ///
        ////////////////////////////////////////////////////////////
        virtual bool onGetData(sf::SoundStream::Chunk& data)
        {
            // We have reached the end of the buffer and all audio data have been played : we can stop playback
            if ((m_offset >= m_samples.size()))
                return false;

            // No new data has arrived since last update : wait until we get some
            while ((m_offset >= m_samples.size()))
                sf::sleep(sf::milliseconds(10));

            // Copy samples into a local buffer to avoid synchronization problems
            // (don't forget that we run in two separate threads)
            {
                sf::Lock lock(m_mutex);
                m_tempBuffer.assign(m_samples.begin() + m_offset, m_samples.end());
            }

            std::cout << "Parsing audio..." << std::endl;

            // Fill audio data to pass to the stream
            data.samples = &m_tempBuffer[0];
            data.sampleCount = m_tempBuffer.size();

            // Update the playing offset
            m_offset += m_tempBuffer.size();

            return true;
        }

        ////////////////////////////////////////////////////////////
        /// /see SoundStream::OnSeek
        ///
        ////////////////////////////////////////////////////////////
        virtual void onSeek(sf::Time timeOffset)
        {
            m_offset = timeOffset.asMilliseconds() * getSampleRate() * getChannelCount() / 1000;
        }

        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        sf::Mutex m_mutex;
        std::vector<sf::Int16> m_samples;
        std::vector<sf::Int16> m_tempBuffer;
        std::size_t m_offset;
    };
}
#endif // SFAUDIOSTREAM_HPP_INCLUDED
