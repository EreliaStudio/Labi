#include "miscellaneous/singleton_instanciator.hpp"

#include "miscellaneous/event_source.hpp"
#include "miscellaneous/texture_atlas.hpp"
#include "miscellaneous/context.hpp"

SingletonInstanciator::SingletonInstanciator()
{
	EventSource::instanciate();
	TextureAtlas::instanciate();
	Context::instanciate();

	TextureAtlas::instance()->add<spk::SpriteSheet>("ChunkSpriteSheet", "resources/texture/chunk_sprite_sheet.png", spk::Vector2Int(32, 66));
	TextureAtlas::instance()->add<spk::SpriteSheet>("PlayerSprite", "resources/texture/playerSpriteSheet.png", spk::Vector2Int(4, 4));

	Context::instance()->playerObject = nullptr;
	Context::instance()->tilemap = nullptr;
}

SingletonInstanciator::~SingletonInstanciator()
{
	Context::release();
	TextureAtlas::release();
	EventSource::release();
}