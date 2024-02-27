#pragma once

#include "event_source.hpp"
#include "texture_atlas.hpp"

class SingletonInstanciator
{
private:

public:
    SingletonInstanciator()
    {
        EventSource::instanciate();
        TextureAtlas::instanciate();
    }

    ~SingletonInstanciator()
    {
        TextureAtlas::release();
        EventSource::release();
    }
};