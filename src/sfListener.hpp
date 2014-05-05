#ifndef SFLISTENER_H_INCLUDED
#define SFLISTENER_H_INCLUDED

#include <SFML/Network.hpp>

namespace sf{
    template <typename ProtoListener>
    class Listener{
    protected:
        ProtoListener listener;
    public:

    };
}

#endif
