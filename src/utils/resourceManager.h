#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <memory>
#include <map>
#include <string>

template <typename Resource, typename Identifier>
class ResourceManager
{
public:
	void load(Identifier id, const std::string& filename);

	template <typename Parameter>
	void load(Identifier id, const std::string& filename, const Parameter& secondParam);

	Resource& get(Identifier id);
	const Resource& get(Identifier id) const;

private:
	void _insertResource(Identifier id, std::unique_ptr<Resource> resource);
	std::map<Identifier, std::unique_ptr<Resource>>	_resourceMap;
};

#endif