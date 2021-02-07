#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "components/illness.h"
#include "components/position.h"
#include "components/velocity.h"

#include "systems/health.h"
#include "systems/movement.h"

#include "entt/src/entt/entt.hpp"

#include "quadtree/include/Quadtree.h"

#include "parameters.h"

std::vector<sf::RectangleShape> quadTreeRects;

struct Node
{
    quadtree::Box<float> box;
    entt::entity id;
};

void initPopulation(entt::registry& reg) {
    for(auto i = 0u; i < population; ++i) {
        const auto entity = reg.create();
        reg.emplace<position>(entity, float(std::rand() % dimX), float(std::rand() % dimY));
        reg.emplace<velocity>(entity, float((std::rand() % 50)/100.0 - 0.245), float((std::rand() % 50)/100.0 - 0.245));
        if (std::rand() % 1000 < illInitialPermill) {
            reg.emplace<ill>(entity);
        }
        else {
            reg.emplace<healty>(entity);
        }
    }
}

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
        //std::cout<<(int)node.id<<" "<<node.box.top<<" "<<node.box.left<<std::endl;
    }

    /*quadTreeRects.clear();
    for(auto&b : quadtree.getboxes()){
        sf::RectangleShape r(sf::Vector2(b.width,b.height));
        r.setPosition(b.left, b.top);
        r.setFillColor(sf::Color::Transparent);
        r.setOutlineColor(sf::Color::Yellow);
        r.setOutlineThickness(1);
        quadTreeRects.push_back(r);
    }*/
    


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

void update(entt::registry& reg, sf::Time elapsed) {
    updatePosition(reg);
    calcCollision(reg);
    updateHealth(reg, elapsed);
    std::cout<<"sani:"<<reg.view<healty>().size()<< "\tmalati:"<<reg.view<ill>().size()<<"\tmorti:"<<(reg.size() - reg.view<healty>().size() - reg.view<ill>().size() - reg.view<recovered>().size())<<"\tguariti:"<<reg.view<recovered>().size()<<std::endl;
}

void draw(sf::RenderWindow & w, entt::registry& reg) {
    sf::VertexArray va;

    for (auto view = reg.view<position, healty>(); const entt::entity e : view) {
        auto & p =view.get<position>(e);
        va.append(sf::Vertex(sf::Vector2f(p.x, p.y), sf::Color(255u,255u,255u)));   
    }
    for (auto view = reg.view<position, ill>(); const entt::entity e : view) {
        auto & p =view.get<position>(e);
        va.append(sf::Vertex(sf::Vector2f(p.x, p.y), sf::Color(255u,0,0)));   
    }
    for (auto view = reg.view<position, recovered>(); const entt::entity e : view) {
        auto & p =view.get<position>(e);
        va.append(sf::Vertex(sf::Vector2f(p.x, p.y), sf::Color(0,0,255)));   
    }
    w.draw(va);

    /*for(auto &r:quadTreeRects) {
        w.draw(r);
    }*/
}

int main()
{

    entt::registry registry;

    initPopulation(registry);
    // create the window
    sf::RenderWindow window(sf::VideoMode(dimX, dimY), "Particles");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(50);

    // create the parti60000cle system
    //ParticleSystem particles(200);

    // create a clock to track the elapsed time
    sf::Clock clock;
    sf::Clock occupancyClock;

    // run the main loop
    while (window.isOpen())
    {
        occupancyClock.restart();
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        
        sf::Time evtTime = occupancyClock.restart();
        // update it
        sf::Time elapsed = clock.restart();
        update(registry, elapsed);
        
        sf::Time simTime = occupancyClock.restart();

        // draw it
        window.clear();
        draw(window, registry);
        sf::Time drawTime = occupancyClock.restart();
        window.display();
        sf::Time displayTime = occupancyClock.restart();
        
        std::cout<<"evt:"<<evtTime.asMicroseconds()<<" sim:"<<simTime.asMicroseconds()<<" draw:"<<drawTime.asMicroseconds()<<" display:"<<displayTime.asMicroseconds()<<std::endl;
    }

    return 0;
}
 
