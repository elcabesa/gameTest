#include <iostream>
#include "quadtree.h"

quadTree::quadTree(const sf::FloatRect& rect):
    _root{std::make_unique<quadTreeNode>(rect, _cache)}
{}

void quadTree::add(entt::entity en, const sf::FloatRect& dest) {
    _root->add(en, dest);
}

void quadTree::updatePosition(entt::entity en, const sf::FloatRect& dest) {
    _cache.at(en)->updatePosition(en, dest);
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
    _cache.at(en)->remove(en);
}