#include "collisionDetector.h"
#include "parameters.h"
#include "components/collisionBox.h"
#include "components/velocity.h"
#include "components/illness.h"
#include "entt/src/entt/entity/registry.hpp"
#include "quadtree/include/Quadtree.h"

std::vector<sf::RectangleShape> quadTreeRects;

const std::vector<sf::RectangleShape>& getDebugRects() {
    return quadTreeRects;
}

void calcCollision(entt::registry& reg) {

    auto box = quadtree::Box(0.0f, 0.0f, float(dimX + 2), float(dimY + 2));
    
    auto view = reg.view<collisionBox, velocity>();
    
    auto getBox = [&](entt::entity id)
    {
        return view.get<collisionBox>(id);
    };

    auto quadtree = quadtree::Quadtree<entt::entity, decltype(getBox)>(box, getBox);

    for(auto& en: view) {
        quadtree.add(en);
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
        auto & v1 = view.get<velocity>(inter.first); 
        auto & v2 = view.get<velocity>(inter.second); 
        //std::cout<<"COLLISION "<< (int)inter.first<<" "<< (int)inter.second<<std::endl;
        

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