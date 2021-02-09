#ifndef QUADTREE_H
#define QUADTREE_H

#include <array>
#include <memory>

#include <SFML/Graphics.hpp>
#include "entt/src/entt/fwd.hpp"
// TODO translate children number in an enum?
// TODO move it inside a class??
static constexpr auto Threshold = std::size_t(16);
static constexpr auto MaxDepth = std::size_t(8);

struct Node
{
    sf::Rect<float> rect;
    entt::entity id;
};

class quadTreeNode
{
public:
    
    void add(std::size_t depth, const Node& value);
    void findAllIntersections(std::vector<std::pair<entt::entity, entt::entity>>& intersections) const;
    void getRects(std::vector<sf::Rect<float>>& boxes) const;
    quadTreeNode(const sf::Rect<float>& rect);
private:
        std::array<std::unique_ptr<quadTreeNode>, 4> _children;
        std::vector<Node> _values;
        sf::Rect<float> _rect;

        bool _isLeaf() const;
        void _split();

        sf::Rect<float> _computeRect(int i);
        int _getQuadrant(const sf::Rect<float>& valueRect);
        void _findIntersectionsInDescendants(const Node& value, std::vector<std::pair<entt::entity, entt::entity>>& intersections) const;

};

class quadTree {
public:
    quadTree(const sf::Rect<float>& rect);
    void add(const Node& value);
    std::vector<std::pair<entt::entity, entt::entity>> findAllIntersections() const;
    std::vector<sf::Rect<float>> getRects() const;
private:
    std::unique_ptr<quadTreeNode> _root;
};

#endif
