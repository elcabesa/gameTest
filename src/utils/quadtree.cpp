#include <iostream>
#include "quadtree.h"

//static unsigned long int nodes = 0;
//static unsigned long int tests = 0;

Rect::Rect(float top, float left, float height, float width): 
_top(top),
_left(left),
_height(height),
_width(width)
{}

Rect::Rect(const sf::Vector2<float>& pos, const sf::Vector2<float>& size):
_top(pos.y),
_left(pos.x),
_height(size.y),
_width(size.x)
{}

float& Rect::top() {return _top;}
float& Rect::left() {return _left;}
float Rect::bottom() const {return _top + _height;}
float Rect::right() const {return _left + _width;}
float& Rect::width() {return _width;}
float& Rect::height() {return _height;}

float Rect::top() const {return _top;}
float Rect::left() const {return _left;}
float Rect::width() const {return _width;}
float Rect::height() const {return _height;}


bool Rect::contains(const Rect& other) const {
    return _left <= other._left && other.right() < right() &&
            _top <= other._top && other.bottom() < bottom();
}

bool Rect::intersects(const Rect& other) const {
    float interLeft   = std::max(_left, other._left);
    float interTop    = std::max(_top, other._top);
    float interRight  = std::min(right(), other.right());
    float interBottom = std::min(bottom(), other.bottom());
    return (interLeft < interRight) && (interTop < interBottom);
}

quadTreeNode::quadTreeNode(const Rect& rect, quadTreeNode* parent):_rect{rect}, _parent{parent} {}

bool quadTreeNode::_isLeaf() const {
    return !static_cast<bool>(_children[0]);
}

Rect quadTreeNode::_computeRect(int i) {
    sf::Vector2<float> origin(_rect.left(), _rect.top());
    sf::Vector2<float> childSize(_rect.width() / 2.0f, _rect.height() / 2.0f);

    switch (i) {
        // North West
        case 0:
            return Rect(origin, childSize);
        // Norst East
        case 1:
            return Rect(sf::Vector2<float>(origin.x + childSize.x, origin.y), childSize);
        // South West
        case 2:
            return Rect(sf::Vector2<float>(origin.x, origin.y + childSize.y), childSize);
        // South East
        case 3:
            return Rect(origin + childSize, childSize);
        default:
            assert(false && "Invalid child index");
            return Rect();
    }
}

int quadTreeNode::_getQuadrant(const Rect& valueRect) {
    sf::Vector2<float> center(_rect.left() + _rect.width() / 2.0f, _rect.top() + _rect.height() / 2.0f);
    // West
    if (valueRect.right() < center.x)
    {
        // North West
        if (valueRect.bottom() < center.y)
            return 0;
        // South West
        else if (valueRect.top() >= center.y)
            return 2;
        // Not contained in any quadrant
        else
            return -1;
    }
    // East
    else if (valueRect.left() >= center.x)
    {
        // North East
        if (valueRect.bottom() < center.y)
            return 1;
        // South East
        else if (valueRect.top() >= center.y)
            return 3;
        // Not contained in any quadrant
        else
            return -1;
    }
    // Not contained in any quadrant
    else
        return -1;
}

void quadTreeNode::add(std::size_t depth, const Node& value) {
    assert(_rect.contains(value.rect));
    if (_isLeaf()) {
        // Insert the value in this node if possible
        if (depth >= MaxDepth || _values.size() < Threshold) {
            _values.push_back(value);
        } else {
            // Otherwise, we split and we try again
            _split();
            add(depth, value);
        }
    } else {
        auto i = _getQuadrant(value.rect);
        // Add the value in a child if the value is entirely contained in it
        if (i != -1) {
            _children[i]->add(depth + 1, value);
        } else {
            // Otherwise, we add the value in the current node
            _values.push_back(value);
        }
    }
}

void quadTreeNode::_split() {
    assert(_isLeaf() && "Only leaves can be split");
    
    // Create childrens
    unsigned i = 0;
    for (auto& child : _children) {
        child = std::make_unique<quadTreeNode>(_computeRect(i), this);
        ++i;
    }


    // Assign values to childrens
    auto newValues = std::vector<Node>(); // New values for this node

    for (const auto& value : _values)
    {
        auto i = _getQuadrant(value.rect);
        if (i != -1) {
            _children[i]->_values.push_back(value);
        }
        else {
            newValues.push_back(value);
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

void quadTreeNode::getRects(std::vector<Rect>& boxes) const
{
    if(_isLeaf()) {
        boxes.emplace_back(_rect);
    } else {
        for(const auto& child: _children) {
            child->getRects(boxes); 
        }
    }
}


quadTree::quadTree(const Rect& rect):
    _root{std::make_unique<quadTreeNode>(rect, nullptr)}
{}

void quadTree::add(const Node& n) {
    _root->add(0, n);
}

std::vector<std::pair<entt::entity, entt::entity>> quadTree::findAllIntersections() const {
    //tests = 0;
    //nodes = 0;
    auto intersections = std::vector<std::pair<entt::entity, entt::entity>>();
    _root->findAllIntersections(intersections);
    //std::cout<<"nodes "<<nodes<<" tests "<<tests<<std::endl;
    return intersections;
}

std::vector<Rect> quadTree::getRects() const {
    std::vector<Rect> boxes;
    _root->getRects(boxes);
    return boxes;
}