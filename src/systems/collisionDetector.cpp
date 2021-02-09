#include "collisionDetector.h"
#include "parameters.h"
#include "components/position.h"
#include "components/velocity.h"
#include "components/illness.h"
#include "entt/src/entt/entity/registry.hpp"
//#include "quadtree/include/Quadtree.h"
#include "quadtree.h"

std::vector<sf::RectangleShape> quadTreeRects;

const std::vector<sf::RectangleShape>& getDebugRects() {
    return quadTreeRects;
}

void calcCollision(entt::registry& reg) {
    auto quadtree = quadTree(sf::Rect<float>(0.0f, 0.0f, float(dimX+2), float(dimY+2)));


    Node node;
    node.rect.height = 2;
    node.rect.width = 2;

    auto view1 = reg.view<position>();
    for (auto& en: view1) {
        auto & p1 = view1.get<position>(en); 
        node.id = en;
        node.rect.top = p1.y;
        node.rect.left = p1.x;
        quadtree.add(node);   
    }

    quadTreeRects.clear();
    for(auto&b : quadtree.getRects()){
        sf::RectangleShape r(sf::Vector2(b.width,b.height));
        r.setPosition(b.left, b.top);
        r.setFillColor(sf::Color::Transparent);
        r.setOutlineColor(sf::Color::Yellow);
        r.setOutlineThickness(1);
        quadTreeRects.push_back(r);
    }
    

    auto view = reg.view<velocity>();
    

    auto intersections = quadtree.findAllIntersections();
    //std::cout<<"intersection size:"<<intersections.size()<<std::endl;
    for(auto& inter: intersections) {
        auto & v1 = view.get<velocity>(inter.first); 
        auto & v2 = view.get<velocity>(inter.second); 
        //std::cout<<"COLLISION "<< (int)inter.first->id<<" "<< (int)inter.second->id<<std::endl;
        

        v1.dx = (std::rand() % 50)/100.0 - 0.245;
        v1.dy = (std::rand() % 50)/100.0 - 0.245;
        v2.dx = (std::rand() % 50)/100.0 - 0.245;
        v2.dy = (std::rand() % 50)/100.0 - 0.245;

        if (reg.has<ill>(inter.first) && reg.has<healty>(inter.second)) {
            if((std::rand() % 100) < illProbability) {
                reg.remove<healty>(inter.second);
                reg.emplace<ill>(inter.second);
            }
        }
        if (reg.has<ill>(inter.second) && reg.has<healty>(inter.first)) {
            if((std::rand() % 100) < illProbability) {
                reg.remove<healty>(inter.first);
                reg.emplace<ill>(inter.first);
            }
        }
    }
}