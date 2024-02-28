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

        TextureAtlas::instance()->add<spk::SpriteSheet>("worldBackgound", "worldBackground.png", spk::Vector2Int(16, 24));
        TextureAtlas::instance()->add<spk::SpriteSheet>("playerSpriteSheet", "playerSpriteSheet.png", spk::Vector2Int(4, 4));
    }

    ~SingletonInstanciator()
    {
        TextureAtlas::release();
        EventSource::release();
    }
};