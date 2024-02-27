#pragma once

#include "sparkle.hpp"

class TextureAtlas : public spk::Singleton<TextureAtlas>
{
    friend class spk::Singleton<TextureAtlas>;

private:
    std::map<std::string, std::unique_ptr<spk::Texture>> _textures;

    TextureAtlas();

public:
    template<typename TTextureType, typename ... Args>
    TTextureType* add(const std::string& p_name, Args&& ... p_constructorArguments)
    {
        static_assert(std::is_base_of<spk::Texture, TTextureType>::value, "TTextureType must be a derivative of spk::Texture");

        if (_textures.contains(p_name))
        {
            _textures[p_name] = std::make_unique<TTextureType>(std::forward<Args>(p_constructorArguments)...);
        }
        else
        {
            _textures.insert(std::make_pair(p_name, std::make_unique<TTextureType>(std::forward<Args>(p_constructorArguments)...)));
        }

        return (dynamic_cast<TTextureType*>(_textures.at(p_name).get()));
    }
    
    template<typename TTextureType>
    const TTextureType* as(const std::string& p_name)
    {
        static_assert(std::is_base_of<spk::Texture, TTextureType>::value, "TTextureType must be a derivative of spk::Texture");

        return (dynamic_cast<const TTextureType*>(texture(p_name)));
    }

    const spk::Texture* texture(const std::string& p_name) const;
};