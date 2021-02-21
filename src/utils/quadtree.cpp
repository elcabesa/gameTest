#include <iostream>
#include "quadtree.h"

quadTreeNode::quadTreeNode(const sf::FloatRect& rect, unsigned int depth, quadTreeNode* parent):_rect{rect}, _parent{parent}, _depth{depth} {}

bool quadTreeNode::_isLeaf() const {
    return !static_cast<bool>(_children[0]);
}

sf::FloatRect quadTreeNode::_computeRect(int i) {
    sf::Vector2<float> origin(_rect.left, _rect.top);
    sf::Vector2<float> childSize(_rect.width / 2.0f, _rect.height / 2.0f);

    switch (i) {
        // North West
        case 0:
            return sf::FloatRect(origin, childSize);
        // Norst East
        case 1:
            return sf::FloatRect(sf::Vector2<float>(origin.x + childSize.x, origin.y), childSize);
        // South West
        case 2:
            return sf::FloatRect(sf::Vector2<float>(origin.x, origin.y + childSize.y), childSize);
        // South East
        case 3:
            return sf::FloatRect(origin + childSize, childSize);
        default:
            assert(false && "Invalid child index");
            return sf::FloatRect();
    }
}

int quadTreeNode::_getQuadrant(const sf::FloatRect& valueRect) {
    sf::Vector2<float> center(_rect.left + _rect.width / 2.0f, _rect.top + _rect.height / 2.0f);
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

void quadTreeNode::add(const Node& value, std::unordered_map<entt::entity, quadTreeNode*>& cache) {
    assert(_rect.contains(sf::Vector2f(value.rect.left, value.rect.top)));
    assert(_rect.contains(sf::Vector2f(value.rect.left + value.rect.width, value.rect.top + value.rect.height)));
    if (_isLeaf()) {
        // Insert the value in this node if possible
        if (_depth >= MaxDepth || _values.size() < Threshold) {
            _values.push_back(value);
            cache[value.id] = this;
        } else {
            // Otherwise, we split and we try again
            _split(cache);
            add(value, cache);
        }
    } else {
        auto i = _getQuadrant(value.rect);
        // Add the value in a child if the value is entirely contained in it
        if (i != -1) {
            _children[i]->add(value, cache);
        } else {
            // Otherwise, we add the value in the current node
            _values.push_back(value);
            cache[value.id] = this;
        }
    }
}

void quadTreeNode::_split(std::unordered_map<entt::entity, quadTreeNode*>& cache) {
    assert(_isLeaf() && "Only leaves can be split");
    
    // Create childrens
    unsigned i = 0;
    for (auto& child : _children) {
        child = std::make_unique<quadTreeNode>(_computeRect(i), _depth + 1, this);
        ++i;
    }


    // Assign values to childrens
    auto newValues = std::vector<Node>(); // New values for this node

    for (const auto& value : _values)
    {
        auto i = _getQuadrant(value.rect);
        if (i != -1) {
            _children[i]->_values.push_back(value);
            cache[value.id] = _children[i].get();
        }
        else {
            newValues.push_back(value);
            cache[value.id] = this;
        }
    }
    _values = std::move(newValues);
}

void quadTreeNode::findAllIntersections(std::vector<std::pair<entt::entity, entt::entity>>& intersections) const
{
    //nodes += _values.size();
    // Find intersections between values stored in this node
    // Make sure to not report the same intersection twice
    for (auto i = std::size_t(1); i < _values.size(); ++i) {
        for (auto j = std::size_t(0); j < i; ++j) {
            //++tests;
            if (_values[i].rect.intersects(_values[j].rect)) {
                intersections.emplace_back(_values[i].id, _values[j].id);
            }
        }
    }

    if (!_isLeaf()) {
        // Values in this node can intersect values in descendants
        for (const auto& child : _children) {
            for (const auto& value : _values) {
                //++tests;
                if (value.rect.intersects(child->_rect)) {
                    child->_findIntersectionsInDescendants(value, intersections);
                }
            }
        }
        // Find intersections in children
        for (const auto& child : _children) {
            child->findAllIntersections(intersections);
        }
    }
}

void quadTreeNode::_findIntersectionsInDescendants(const Node& value, std::vector<std::pair<entt::entity, entt::entity>>& intersections) const
{
    // Test against the values stored in this node
    for (const auto& other : _values) {
        //++tests;
        if (value.rect.intersects(other.rect)) {
            intersections.emplace_back(value.id, other.id);
        }
    }
    // Test against values stored into descendants of this node
    if (!_isLeaf()) {
        for (const auto& child : _children) {
            //++tests;
            if (value.rect.intersects(child->_rect)) {
                child->_findIntersectionsInDescendants(value, intersections);
            }
        }
    }
}

void quadTreeNode::getRects(std::vector<sf::FloatRect>& boxes) const
{
    if (_isLeaf()) {
        boxes.emplace_back(_rect);
    } else {
        for(const auto& child: _children) {
            child->getRects(boxes); 
        }
    }
}

void quadTreeNode::updatePosition(entt::entity en, const sf::FloatRect& dest, std::unordered_map<entt::entity, quadTreeNode*>& cache) {
    // check that the entity is contained here
    auto it = _findValue(en);
    assert(it != _values.end());

    // update the rect
    it->rect = dest;
    // check if the rect is still inside the rect
    if (!(_rect.contains(sf::Vector2f(dest.left, dest.top)) && _rect.contains(sf::Vector2f(dest.left + dest.width, dest.top + dest.width)))) {
        //save the node;
        auto node = *it;
        //locally remove the node
        _removeFromVector(it);

        assert(_parent != nullptr);
        quadTreeNode* parent = _parent;
        _parent->_tryMerge(cache);

        //PAY ATTENCTION, FROM HERE 'this' could be nullptr!! don't do any action on the node itself
        //find the first parent quadTreeNode containing the rect
        while (!(parent->_rect.contains(sf::Vector2f(dest.left, dest.top)) && parent->_rect.contains(sf::Vector2f(dest.left + dest.width, dest.top + dest.width))) && parent != nullptr) {
            parent = parent->_parent;
        }
        assert(parent->_rect.contains(sf::Vector2f(dest.left, dest.top)));
        assert(parent->_rect.contains(sf::Vector2f(dest.left + dest.width, dest.top + dest.width)));
        assert(parent != nullptr);
        //add to it & return
        parent->add(node, cache);

    } else {
        if(!_isLeaf()) {
            //check if we can move it in a children
            auto i = _getQuadrant(dest);
            // movethe value in a child if the value is entirely contained in it
            if (i != -1) {
                
                auto node = *it;
                //locally remove the node
                *it = std::move(_values.back());
                _values.pop_back();

                //add in the children
                _children[i]->add(node, cache);
            }
        }
    }
}

std::vector<Node>::iterator quadTreeNode::_findValue(entt::entity en) {
    return std::find_if(_values.begin(), _values.end(), [&en](const Node& x) {return en == x.id;});
}

void quadTreeNode::_removeFromVector(std::vector<Node>::iterator it) {
    *it = std::move(_values.back());
    _values.pop_back();
}

void quadTreeNode::_tryMerge(std::unordered_map<entt::entity, quadTreeNode*>& cache)
{
    assert(!_isLeaf());
    auto nbValues = _values.size();
    for (const auto& child : _children) {
        //can merge only leaf nodes
        if (!child->_isLeaf()) {
            return;
        }
        nbValues += child->_values.size();
    }

    if (nbValues <= Threshold) {
        _values.reserve(nbValues);
        // Merge the values of all the children
        for (const auto& child : _children) {
            for (const auto& value : child->_values) {
                _values.push_back(value);
                cache[value.id] = this;
            }
        }
        // Remove the children
        for (auto& child : _children) {
            child.reset();
        }
    }
}

void quadTreeNode::remove(entt::entity en, std::unordered_map<entt::entity, quadTreeNode*>& cache) {
    // Remove the value from node
    _removeValue(en);
    if (_isLeaf()) {
        if (_parent != nullptr) {
            _parent->_tryMerge(cache);
        }
    }
}

void quadTreeNode::_removeValue(entt::entity en) {
    // Find the value in node->values
    auto it = _findValue(en);
    assert(it != _values.end());
    // Swap with the last element and pop back
    _removeFromVector(it);
}


quadTree::quadTree(const sf::FloatRect& rect):
    _root{std::make_unique<quadTreeNode>(rect)}
{}

void quadTree::add(const Node& n) {
    _root->add(n, _cache);
}

void quadTree::updatePosition(entt::entity en, const sf::FloatRect& dest) {
    _cache.at(en)->updatePosition(en, dest, _cache);
}

std::vector<std::pair<entt::entity, entt::entity>> quadTree::findAllIntersections() const {
    auto intersections = std::vector<std::pair<entt::entity, entt::entity>>();
    _root->findAllIntersections(intersections);
    return intersections;
}

std::vector<sf::FloatRect> quadTree::getRects() const {
    std::vector<sf::FloatRect> boxes;
    _root->getRects(boxes);
    return boxes;
}

void quadTree::remove(entt::entity en) {
    _cache.at(en)->remove(en, _cache);
}