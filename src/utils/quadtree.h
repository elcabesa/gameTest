#ifndef QUADTREE_H
#define QUADTREE_H

#include <array>
#include <memory>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include "entt/src/entt/fwd.hpp"
// TODO translate children number in an enum?
// TODO move constants inside a class??
static constexpr auto Threshold = std::size_t(16);
static constexpr auto MaxDepth = std::size_t(8);

// TODO remove rect and use sf::rect?s
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
    
    void add(const Node& value, std::unordered_map<entt::entity, quadTreeNode*>& cache);
    void findAllIntersections(std::vector<std::pair<entt::entity, entt::entity>>& intersections) const;
    void getRects(std::vector<Rect>& boxes) const;
    quadTreeNode(const Rect& rect, unsigned int depth = 0, quadTreeNode* parent = nullptr);
    void updatePosition(entt::entity en, const Rect& dest, std::unordered_map<entt::entity, quadTreeNode*>& cache);
    void remove(entt::entity en, std::unordered_map<entt::entity, quadTreeNode*>& cache);
private:
        std::array<std::unique_ptr<quadTreeNode>, 4> _children;
        std::vector<Node> _values;
        Rect _rect;
        quadTreeNode* const _parent;
        unsigned int _depth;

        bool _isLeaf() const;
        void _split(std::unordered_map<entt::entity, quadTreeNode*>& cache);

        Rect _computeRect(int i);
        int _getQuadrant(const Rect& valueRect);
        void _findIntersectionsInDescendants(const Node& value, std::vector<std::pair<entt::entity, entt::entity>>& intersections) const;
        void _tryMerge(std::unordered_map<entt::entity, quadTreeNode*>& cache);
        
        void _removeValue(entt::entity en);
        std::vector<Node>::iterator _findValue(entt::entity en);
        void _removeFromVector(std::vector<Node>::iterator it);
};

class quadTree {
public:
    // todo make a decent interface, use node or en+rect consistently
    quadTree(const Rect& rect);
    void add(const Node& value);
    void updatePosition(entt::entity en, const Rect& dest);
    void remove(entt::entity en);
    std::vector<std::pair<entt::entity, entt::entity>> findAllIntersections() const;
    std::vector<Rect> getRects() const;
private:
    std::unordered_map<entt::entity, quadTreeNode*> _cache; // TODO find a better name
    std::unique_ptr<quadTreeNode> _root;
};

#endif
