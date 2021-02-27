#ifndef QUADTREE_NODE_H
#define QUADTREE_NODE_H

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include "entt/src/entt/fwd.hpp"

//TODO create a function to test is a sf::rect contains sf::rect
struct Node
{
    sf::FloatRect rect;
    entt::entity id;
};


class quadTreeNode
{
public:
    quadTreeNode(const sf::FloatRect& rect, std::unordered_map<entt::entity, quadTreeNode*>& cache, unsigned int depth = 0, quadTreeNode* parent = nullptr);
    
    void add(entt::entity en, const sf::FloatRect& dest);
    void updatePosition(entt::entity en, const sf::FloatRect& dest);
    void remove(entt::entity en);

    void findAllIntersections(std::vector<std::pair<entt::entity, entt::entity>>& intersections) const;
    void getRects(std::vector<sf::FloatRect>& boxes) const;
private:
        static constexpr auto _threshold = std::size_t(16);
        static constexpr auto _maxDepth = std::size_t(8);

        std::array<std::unique_ptr<quadTreeNode>, 4> _children;
        std::vector<Node> _values;
        sf::FloatRect _rect;
        quadTreeNode* const _parent;
        std::unordered_map<entt::entity, quadTreeNode*>& _cache;
        unsigned int _depth;

        void _add(const Node& value);
        bool _isLeaf() const;
        void _split();
        sf::FloatRect _computeSubRect(int i);
        int _getQuadrant(const sf::FloatRect& valueRect);
        std::vector<Node>::iterator _findValue(entt::entity en);
        void _findIntersectionsInDescendants(const Node& value, std::vector<std::pair<entt::entity, entt::entity>>& intersections) const;
        void _tryMerge();
        void _removeValue(entt::entity en);
        void _removeFromVector(std::vector<Node>::iterator it);
};

#endif