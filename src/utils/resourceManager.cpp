#include <cassert>

#include <SFML/Graphics/Texture.hpp>
#include "resourceManager.h"
#include "resourceIdentifiers.h"

template <typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
	// Create and load resource
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filename))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	// If loading successful, insert resource to map
	_insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceManager<Resource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondParam)
{
	// Create and load resource
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filename, secondParam))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

	// If loading successful, insert resource to map
	_insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceManager<Resource, Identifier>::get(Identifier id)
{
	auto found = _resourceMap.find(id);
	assert(found != _resourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceManager<Resource, Identifier>::get(Identifier id) const
{
	auto found = _resourceMap.find(id);
	assert(found != _resourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::_insertResource(Identifier id, std::unique_ptr<Resource> resource) 
{
	// Insert and check success
	auto inserted = _resourceMap.insert(std::make_pair(id, std::move(resource)));
	//TODO remove warning
	assert(inserted.second);
}

template class ResourceManager<sf::Texture, Textures::ID>;