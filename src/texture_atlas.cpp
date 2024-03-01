#include "texture_atlas.hpp"

TextureAtlas::TextureAtlas()
{

}

const spk::Texture* TextureAtlas::texture(const std::string& p_name) const
{
	if (_textures.contains(p_name) == false)
		spk::throwException("No texture [" + p_name + "] loaded");
	return (_textures.at(p_name).get());
}