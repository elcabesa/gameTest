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

class Rect {
public:
    Rect(float top = 0, float left = 0, float height = 0, float width = 0);
    Rect(const sf::Vector2<float>& pos, const sf::Vector2<float>& size);
    float& top();
    float top() const;
    float& left();
    float left() const;
    float bottom() const;
    float right() const;
    float& width();
    float width() const;
    float& height();
    float height() const;

    bool contains(const Rect& other) const;
    bool intersects(const Rect& other) const;

private:
    float _top;
    float _left;
    float _height;
    float _width;
};

struct Node
{
    Rect rect;
    entt::entity id;
};

class quadTreeNode
{
public:
    
    void add(std::size_t depth, const Node& value);
    void findAllIntersections(std::vector<std::pair<entt::entity, entt::entity>>& intersections) const;
    void getRects(std::vector<Rect>& boxes) const;
    quadTreeNode(const Rect& rect, quadTreeNode* parent);
private:
        std::array<std::unique_ptr<quadTreeNode>, 4> _children;
        std::vector<Node> _values;
        Rect _rect;
        quadTreeNode* const _parent;

        bool _isLeaf() const;
        void _split();

        Rect _computeRect(int i);
        int _getQuadrant(const Rect& valueRect);
        void _findIntersectionsInDescendants(const Node& value, std::vector<std::pair<entt::entity, entt::entity>>& intersections) const;

};

class quadTree {
public:
    quadTree(const Rect& rect);
    void add(const Node& value);
    std::vector<std::pair<entt::entity, entt::entity>> findAllIntersections() const;
    std::vector<Rect> getRects() const;
private:
    std::unique_ptr<quadTreeNode> _root;
};

#endif
