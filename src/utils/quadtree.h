#ifndef QUADTREE_H
#define QUADTREE_H

#include <array>
#include <memory>
#include <vector>
#include <unordered_map>

#include <SFML/Graphics/Rect.hpp>
#include "entt/src/entt/fwd.hpp"

// TODO translate children number in an enum?
// TODO move constants inside a class??
static constexpr auto Threshold = std::size_t(16);
static constexpr auto MaxDepth = std::size_t(8);

struct Node
{
    sf::FloatRect rect;
    entt::entity id;
};

class quadTreeNode
{
public:
    
    void add(const Node& value, std::unordered_map<entt::entity, quadTreeNode*>& cache);
    void findAllIntersections(std::vector<std::pair<entt::entity, entt::entity>>& intersections) const;
    void getRects(std::vector<sf::FloatRect>& boxes) const;
    quadTreeNode(const sf::FloatRect& rect, unsigned int depth = 0, quadTreeNode* parent = nullptr);
    void updatePosition(entt::entity en, const sf::FloatRect& dest, std::unordered_map<entt::entity, quadTreeNode*>& cache);
    void remove(entt::entity en, std::unordered_map<entt::entity, quadTreeNode*>& cache);
private:
        std::array<std::unique_ptr<quadTreeNode>, 4> _children;
        std::vector<Node> _values;
        sf::FloatRect _rect;
        quadTreeNode* const _parent;
        unsigned int _depth;

        bool _isLeaf() const;
        void _split(std::unordered_map<entt::entity, quadTreeNode*>& cache);

        sf::FloatRect _computeRect(int i);
        int _getQuadrant(const sf::FloatRect& valueRect);
        void _findIntersectionsInDescendants(const Node& value, std::vector<std::pair<entt::entity, entt::entity>>& intersections) const;
        void _tryMerge(std::unordered_map<entt::entity, quadTreeNode*>& cache);
        
        void _removeValue(entt::entity en);
        std::vector<Node>::iterator _findValue(entt::entity en);
        void _removeFromVector(std::vector<Node>::iterator it);
};

class quadTree {
public:
    // todo make a decent interface, use node or en+rect consistently
    quadTree(const sf::FloatRect& rect);
    void add(const Node& value);
    void updatePosition(entt::entity en, const sf::FloatRect& dest);
    void remove(entt::entity en);
    std::vector<std::pair<entt::entity, entt::entity>> findAllIntersections() const;
    std::vector<sf::FloatRect> getRects() const;
private:
    std::unordered_map<entt::entity, quadTreeNode*> _cache; // TODO find a better name
    std::unique_ptr<quadTreeNode> _root;
};

#endif
