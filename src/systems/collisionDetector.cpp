#include "collisionDetector.h"
#include "parameters.h"
#include "components/position.h"
#include "components/velocity.h"
#include "components/illness.h"
#include "entt/src/entt/entity/registry.hpp"
#include "quadtree/include/Quadtree.h"

std::vector<sf::RectangleShape> quadTreeRects;

const std::vector<sf::RectangleShape>& getDebugRects() {
    return quadTreeRects;
}

struct Node
{
    quadtree::Box<float> box;
    entt::entity id;
};

void calcCollision(entt::registry& reg) {

    auto box = quadtree::Box(0.0f, 0.0f, float(dimX+2), float(dimY+2));
    auto getBox = [](Node* node)
    {
        return node->box;
    };
    auto quadtree = quadtree::Quadtree<Node*, decltype(getBox)>(box, getBox);

    auto view = reg.view<position, velocity>();
    std::vector<Node> nodes;

    for (auto it = view.begin(); it!= view.end(); ++it) {
        auto& en1 = *it;
        auto & p1 =view.get<position>(en1); 
        Node node;
        node.id = en1;
        node.box.top = p1.y;
        node.box.left = p1.x;
        node.box.height = 2;
        node.box.width = 2;
        nodes.emplace_back(node);
    }
    for(auto& node:nodes) {
        quadtree.add(&node);
    }

    quadTreeRects.clear();
    for(auto&b : quadtree.getboxes()){
        sf::RectangleShape r(sf::Vector2(b.width,b.height));
        r.setPosition(b.left, b.top);
        r.setFillColor(sf::Color::Transparent);
        r.setOutlineColor(sf::Color::Yellow);
        r.setOutlineThickness(1);
        quadTreeRects.push_back(r);
    }
    


    auto intersections = quadtree.findAllIntersections();
    //std::cout<<"intersection size:"<<intersections.size()<<std::endl;
    for(auto& inter: intersections) {
        auto & v1 = view.get<velocity>(inter.first->id); 
        auto & v2 = view.get<velocity>(inter.second->id); 
        //std::cout<<"COLLISION "<< (int)inter.first->id<<" "<< (int)inter.second->id<<std::endl;
        

        v1.dx = (std::rand() % 50)/100.0 - 0.245;
        v1.dy = (std::rand() % 50)/100.0 - 0.245;
        v2.dx = (std::rand() % 50)/100.0 - 0.245;
        v2.dy = (std::rand() % 50)/100.0 - 0.245;

        if (reg.has<ill>(inter.first->id) && reg.has<healty>(inter.second->id)) {
            if((std::rand() % 100) < illProbability) {
                reg.remove<healty>(inter.second->id);
                reg.emplace<ill>(inter.second->id);
            }
        }
        if (reg.has<ill>(inter.second->id) && reg.has<healty>(inter.first->id)) {
            if((std::rand() % 100) < illProbability) {
                reg.remove<healty>(inter.first->id);
                reg.emplace<ill>(inter.first->id);
            }
        }
    }
}