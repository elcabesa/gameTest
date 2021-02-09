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
    bool isLeaf() const;
    void add(std::size_t depth, const sf::Rect<float>& rect, const Node* value);
    void findAllIntersections(const sf::Rect<float>& rect, std::vector<std::pair<entt::entity, entt::entity>>& intersections) const;
    void getRects(const sf::Rect<float>& rect, std::vector<sf::Rect<float>>& boxes) const;
private:
        std::array<std::unique_ptr<quadTreeNode>, 4> _children;
        std::vector<const Node*> _values;

        void _split(const sf::Rect<float>& rect);

        static sf::Rect<float> _computeRect(const sf::Rect<float>& rect, int i);
        static int _getQuadrant(const sf::Rect<float>& nodeRect, const sf::Rect<float>& valueRect);
        void _findIntersectionsInDescendants(const Node* value, const sf::Rect<float>& rect, std::vector<std::pair<entt::entity, entt::entity>>& intersections) const;

};

class quadTree {
public:
    quadTree(const sf::Rect<float>& rect);
    void add(const Node* value);
    std::vector<std::pair<entt::entity, entt::entity>> findAllIntersections() const;
    std::vector<sf::Rect<float>> getRects() const;
private:
    const sf::Rect<float> _rect;
    std::unique_ptr<quadTreeNode> _root;
};

#endif
