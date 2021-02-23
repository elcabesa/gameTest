#ifndef RESOURCE_IDENTIFIERS_H
#define RESOURCE_IDENTIFIERS_H

// Forward declaration and a few type definitions
namespace sf
{
	class Texture;
}

template <typename Resource, typename Identifier>
class ResourceManager;


namespace Textures
{
	enum ID
	{
		man
    };
}

typedef ResourceManager<sf::Texture, Textures::ID> TextureManager;

#endif