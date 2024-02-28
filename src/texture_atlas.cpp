#include "texture_atlas.hpp"

TextureAtlas::TextureAtlas()
{
	add<spk::SpriteSheet>("PlayerSprite", "playerSpriteSheet.png", spk::Vector2Int(4, 4));
	add<spk::SpriteSheet>("WorldTileset", "worldBackground.png", spk::Vector2Int(16, 24));
}

const spk::Texture* TextureAtlas::texture(const std::string& p_name) const
{
	if (_textures.contains(p_name) == false)
		spk::throwException("No texture [" + p_name + "] loaded");
	return (_textures.at(p_name).get());
}