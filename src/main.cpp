#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "components/collisionBox.h"
#include "components/illness.h"
#include "components/position.h"
#include "components/velocity.h"

#include "systems/collisionDetector.h"
#include "systems/health.h"
#include "systems/movement.h"
#include "systems/renderer.h"

#include "utils/occupancy.h"

#include "entt/src/entt/entt.hpp"

#include "parameters.h"

void initPopulation(entt::registry& reg) {
    for(auto i = 0u; i < population; ++i) {
        const auto entity = reg.create();
        float x = float(std::rand() % dimX);
        float y = float(std::rand() % dimY);
        reg.emplace<position>(entity, x, y);
        reg.emplace<velocity>(entity, float((std::rand() % 50)/100.0 - 0.245), float((std::rand() % 50)/100.0 - 0.245));
        reg.emplace<collisionBox>(entity, collisionBox(x, y, 2, 2));
        if (std::rand() % 1000 < illInitialPermill) {
            reg.emplace<ill>(entity);
        }
        else {
            reg.emplace<healty>(entity);
        }
    }
}

void update(entt::registry& reg, sf::Time elapsed, CollisionDetector& cd) {
    static unsigned int i = 0;
    updatePosition(reg);
    worldBorderCollision(reg);
    updateCollisionBoxes(reg);

    cd.calcCollision();
    updateHealth(reg, elapsed);
    if(++i >50) {
        std::cout<<"sani:"<<reg.view<healty>().size()<< "\tmalati:"<<reg.view<ill>().size()<<"\tmorti:"<<(reg.size() - reg.view<healty>().size() - reg.view<ill>().size() - reg.view<recovered>().size())<<"\tguariti:"<<reg.view<recovered>().size()<<std::endl;
        i = 0;
    }
}

int main()
{
    cpuOccupancy occupancy;
    entt::registry registry;
    CollisionDetector collDet(registry);

    initPopulation(registry);

    // create the window
    sf::RenderWindow window(sf::VideoMode(dimX, dimY), "Particles");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(50);

    // create a clock to track the elapsed time
    sf::Clock clock;
    sf::Clock occupancyClock;
    // run the main loop
    while (window.isOpen())
    {
        sf::Time displayTime = occupancyClock.restart();
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        sf::Time evtTime = occupancyClock.restart();

        // update system
        update(registry, clock.restart(), collDet);
        
        sf::Time simTime = occupancyClock.restart();

        // draw it
        window.clear();
        draw(window, registry);
        //drawQuadTreeDebugInfo(window, getDebugRects());
        sf::Time drawTime = occupancyClock.restart();
        window.display();
        
        
        occupancy.add(evtTime.asMicroseconds(), simTime.asMicroseconds(), drawTime.asMicroseconds(), displayTime.asMicroseconds());
    }

    return 0;
}
 
