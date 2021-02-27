#include <cassert>

#include <SFML/Graphics/Texture.hpp>
#include "resourceManager.h"
#include "resourceIdentifiers.h"
#include "spdlog/sinks/stdout_color_sinks.h"

template <typename Resource, typename Identifier>
ResourceManager<Resource, Identifier>::ResourceManager(){
	_logger = spdlog::stdout_color_mt("ResourceManager");
}

template <typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::load(Identifier id, const std::string& filename)
{
	_logger->info("loading resource {}", filename);
	// Create and load resource
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filename)) {
		_logger->debug("failed loading resource {}", filename);
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
	}

	// If loading successful, insert resource to map
	_insertResource(id, std::move(resource));
	_logger->debug("... done");

}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceManager<Resource, Identifier>::load(Identifier id, const std::string& filename, const Parameter& secondParam)
{
	_logger->info("loading resource {}", filename);
	// Create and load resource
	std::unique_ptr<Resource> resource(new Resource());
	if (!resource->loadFromFile(filename, secondParam)) {
		_logger->debug("failed loading resource {}", filename);
		throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
	}

	// If loading successful, insert resource to map
	_insertResource(id, std::move(resource));
	_logger->debug("... done");
}

template <typename Resource, typename Identifier>
Resource& ResourceManager<Resource, Identifier>::get(Identifier id)
{
	_logger->trace("get resource {}", id);
	auto found = _resourceMap.find(id);
	assert(found != _resourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceManager<Resource, Identifier>::get(Identifier id) const
{
	_logger->trace("get resource {}", id);
	auto found = _resourceMap.find(id);
	assert(found != _resourceMap.end());

	return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceManager<Resource, Identifier>::_insertResource(Identifier id, std::unique_ptr<Resource> resource) 
{
	_logger->trace("insert resource {}", id);
	// Insert and check success
	auto inserted = _resourceMap.insert(std::make_pair(id, std::move(resource)));
	//TODO remove warning
	assert(inserted.second);
}

template class ResourceManager<sf::Texture, Textures::ID>;