#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "components/illness.h"
#include "components/position.h"
#include "components/velocity.h"

#include "systems/collisionDetector.h"
#include "systems/health.h"
#include "systems/movement.h"
#include "systems/renderer.h"

#include "entt/src/entt/entt.hpp"

#include "parameters.h"

void initPopulation(entt::registry& reg) {
    for(auto i = 0u; i < population; ++i) {
        const auto entity = reg.create();
        if( i == 0) {
            reg.emplace<follow>(entity);
        }
        reg.emplace<position>(entity, float(std::rand() % dimX), float(std::rand() % dimY));
        reg.emplace<velocity>(entity, float((std::rand() % 50)/100.0 - 0.245), float((std::rand() % 50)/100.0 - 0.245));
        /*if (std::rand() % 1000 < illInitialPermill) {
            reg.emplace<ill>(entity);
        }
        else */{
            reg.emplace<healty>(entity);
        }
    }
}

void update(entt::registry& reg, sf::Time elapsed) {
    updatePosition(reg);
    calcCollision(reg);
    updateHealth(reg, elapsed);
    std::cout<<"sani:"<<reg.view<healty>().size()<< "\tmalati:"<<reg.view<ill>().size()<<"\tmorti:"<<(reg.size() - reg.view<healty>().size() - reg.view<ill>().size() - reg.view<recovered>().size())<<"\tguariti:"<<reg.view<recovered>().size()<<std::endl;
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
        //drawQuadTreeDebugInfo(window, getDebugRects());
        draw(window, registry);
        
        sf::Time drawTime = occupancyClock.restart();
        window.display();
        sf::Time displayTime = occupancyClock.restart();
        
        std::cout<<"evt:"<<evtTime.asMicroseconds()<<" sim:"<<simTime.asMicroseconds()<<" draw:"<<drawTime.asMicroseconds()<<" display:"<<displayTime.asMicroseconds()<<std::endl;
    }

    return 0;
}
 
