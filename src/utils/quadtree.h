#ifndef QUADTREE_H
#define QUADTREE_H

#include <array>
#include <memory>
#include <vector>
#include <unordered_map>

#include <SFML/Graphics/Rect.hpp>
#include "entt/src/entt/fwd.hpp"

#include "quadtreeNode.h"

class quadTree {
public:
    // todo make a decent interface, use node or en+rect consistently
    quadTree(const sf::FloatRect& rect);
    void add(entt::entity en, const sf::FloatRect& dest);
    void updatePosition(entt::entity en, const sf::FloatRect& dest);
    void remove(entt::entity en);
    std::vector<std::pair<entt::entity, entt::entity>> findAllIntersections() const;
    std::vector<sf::FloatRect> getRects() const;
private:
    std::unordered_map<entt::entity, quadTreeNode*> _cache; // TODO find a better name
    std::unique_ptr<quadTreeNode> _root;
};

#endif
