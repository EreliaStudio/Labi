#pragma once

#include "miscellaneous/event_source.hpp"
#include "miscellaneous/texture_atlas.hpp"

class SingletonInstanciator
{
private:

public:
    SingletonInstanciator()
    {
        EventSource::instanciate();
        TextureAtlas::instanciate();

        TextureAtlas::instance()->add<spk::SpriteSheet>("ChunkSpriteSheet", "resources/texture/chunk_sprite_sheet.png", spk::Vector2Int(32, 66));
        TextureAtlas::instance()->add<spk::SpriteSheet>("PlayerSprite", "resources/texture/playerSpriteSheet.png", spk::Vector2Int(4, 4));
    }

    ~SingletonInstanciator()
    {
        TextureAtlas::release();
        EventSource::release();
    }
};