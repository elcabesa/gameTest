#include "quadtree.h"

bool quadTreeNode::isLeaf() const {
    return !static_cast<bool>(_children[0]);
}

sf::Rect<float> quadTreeNode::_computeRect(const sf::Rect<float>& rect, int i) {
    sf::Vector2<float> origin(rect.left, rect.top);
    sf::Vector2<float> childSize(rect.width / 2.0f, rect.height / 2.0f);

    switch (i) {
        // North West
        case 0:
            return sf::Rect<float>(origin, childSize);
        // Norst East
        case 1:
            return sf::Rect<float>(sf::Vector2<float>(origin.x + childSize.x, origin.y), childSize);
        // South West
        case 2:
            return sf::Rect<float>(sf::Vector2<float>(origin.x, origin.y + childSize.y), childSize);
        // South East
        case 3:
            return sf::Rect<float>(origin + childSize, childSize);
        default:
            assert(false && "Invalid child index");
            return sf::Rect<float>();
    }
}

int quadTreeNode::_getQuadrant(const sf::Rect<float>& nodeRect, const sf::Rect<float>& valueRect) {
    sf::Vector2<float> center(nodeRect.left + nodeRect.width /2.0f, nodeRect.top + nodeRect.height /2.0f);
    // West
    if (valueRect.left + valueRect.width < center.x)
    {
        // North West
        if (valueRect.top + valueRect.height < center.y)
            return 0;
        // South West
        else if (valueRect.top >= center.y)
            return 2;
        // Not contained in any quadrant
        else
            return -1;
    }
    // East
    else if (valueRect.left >= center.x)
    {
        // North East
        if (valueRect.top + valueRect.height < center.y)
            return 1;
        // South East
        else if (valueRect.top >= center.y)
            return 3;
        // Not contained in any quadrant
        else
            return -1;
    }
    // Not contained in any quadrant
    else
        return -1;
}

void quadTreeNode::add(std::size_t depth, const sf::Rect<float>& rect, const Node* value) {

    assert(rect.contains(value->rect));
    if (isLeaf()) {
        // Insert the value in this node if possible
        if (depth >= MaxDepth || _values.size() < Threshold) {
            _values.push_back(value);
        } else {
            // Otherwise, we split and we try again
            _split(rect);
            add(depth, rect, value);
        }
    } else {
        auto i = _getQuadrant(rect, value->rect);
        // Add the value in a child if the value is entirely contained in it
        if (i != -1) {
            _children[static_cast<std::size_t>(i)].get()->add(depth + 1, _computeRect(rect, i), value);
        } else {
            // Otherwise, we add the value in the current node
            _values.push_back(value);
        }
    }
}

void quadTreeNode::_split(const sf::Rect<float>& rect) {
    assert(isLeaf() && "Only leaves can be split");
    
    // Create childrens
    for (auto& child : _children) {
        child = std::make_unique<quadTreeNode>();
    }

    // Assign values to childrens
    auto newValues = std::vector<const Node*>(); // New values for this node

    for (const auto& value : _values)
    {
        auto i = _getQuadrant(rect, value->rect);
        if (i != -1) {
            _children[static_cast<std::size_t>(i)]->_values.push_back(value);
        }
        else {
            newValues.push_back(value);
        }
    }
    _values = std::move(newValues);
}

void quadTreeNode::findAllIntersections(const sf::Rect<float>& rect, std::vector<std::pair<entt::entity, entt::entity>>& intersections) const
{
    // Find intersections between values stored in this node
    // Make sure to not report the same intersection twice
    for (auto i = std::size_t(0); i < _values.size(); ++i) {
        for (auto j = std::size_t(0); j < i; ++j) {
            if (_values[i]->rect.intersects(_values[j]->rect)) {
                intersections.emplace_back(_values[i]->id, _values[j]->id);
            }
        }
    }

    if (!isLeaf()) {
        // Values in this node can intersect values in descendants
        unsigned int i = 0;
        for (const auto& child : _children) {
            for (const auto& value : _values) {
                auto childRect = _computeRect(rect, i);
                if (value->rect.intersects(childRect)) {
                    child.get()->_findIntersectionsInDescendants(value, childRect, intersections);
                }
            }
            ++i;
        }
        // Find intersections in children
        i = 0;
        for (const auto& child : _children) {
            child.get()->findAllIntersections(_computeRect(rect, i), intersections);
            ++i;
        }
    }
}

void quadTreeNode::_findIntersectionsInDescendants(const Node* value, const sf::Rect<float>& rect, std::vector<std::pair<entt::entity, entt::entity>>& intersections) const
{
    // Test against the values stored in this node
    for (const auto& other : _values) {
        if (value->rect.intersects(other->rect)) {
            intersections.emplace_back(value->id, other->id);
        }
    }
    // Test against values stored into descendants of this node
    if (!isLeaf()) {
        unsigned int i = 0;
        for (const auto& child : _children) {
            auto childRect = _computeRect(rect, i);
            if (value->rect.intersects(childRect)) {
                child->_findIntersectionsInDescendants(value, childRect, intersections);
            }
            ++i;
        }
    }
}

void quadTreeNode::getRects(const sf::Rect<float>& rect, std::vector<sf::Rect<float>>& boxes) const
{
    if(isLeaf()) {
        boxes.emplace_back(rect);
    }
    else {
        for(unsigned int i = 0; i < 4; ++i) {
            _children[i].get()->getRects(_computeRect(rect, i), boxes); 
        }
    }
}


quadTree::quadTree(const sf::Rect<float>& rect):
    _rect{rect}, 
    _root{std::make_unique<quadTreeNode>()}
{}

void quadTree::add(const Node* const n) {
    _root->add(0, _rect, n);
}

std::vector<std::pair<entt::entity, entt::entity>> quadTree::findAllIntersections() const {
    auto intersections = std::vector<std::pair<entt::entity, entt::entity>>();
    _root.get()->findAllIntersections(_rect, intersections);
    return intersections;
}

std::vector<sf::Rect<float>> quadTree::getRects() const {
    std::vector<sf::Rect<float>> boxes;
    _root.get()->getRects(_rect, boxes);
    return boxes;
}