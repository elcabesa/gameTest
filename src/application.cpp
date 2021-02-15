#include "application.h"
#include "parameters.h"

#include <iostream>

//TODO move inside game/world
#include "components/illness.h"
#include "components/position.h"
#include "components/velocity.h"

#include "systems/collisionDetector.h"
#include "systems/health.h"
#include "systems/movement.h"
#include "systems/renderer.h"

//TODO find a good name for application
Application::Application()
: _window(sf::VideoMode(dimX, dimY), "Simulator", sf::Style::Resize | sf::Style::Close) {
    // create the window
    //window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(50);
}




void Application::run() {
    //todo move inside workd/game
    _initPopulation();

    // create a clock to track the elapsed time
    _updateDt.restart();
    // run the main loop
    while (_window.isOpen())
    {
        _occupancy.addDisTime();

        _processInput();
        _update(_updateDt.restart());
        _render();
        
        _occupancy.print();
    }
}

void Application::_initPopulation() {
    for(auto i = 0u; i < population; ++i) {
        const auto entity = _registry.create();
        _registry.emplace<position>(entity, float(std::rand() % dimX), float(std::rand() % dimY));
        _registry.emplace<velocity>(entity, float((std::rand() % 50)/100.0 - 0.245), float((std::rand() % 50)/100.0 - 0.245));
        if (std::rand() % 1000 < illInitialPermill) {
            _registry.emplace<ill>(entity);
        }
        else {
            _registry.emplace<healty>(entity);
        }
    }
}

void Application::_processInput() {
    // handle events
    sf::Event event;
    while (_window.pollEvent(event)) {
        if(event.type == sf::Event::Closed) {
            _window.close();
        }
    }
    _occupancy.addEvtTime();
}

void Application::_update(sf::Time dt) {
    static unsigned int i = 0;
    updatePosition(_registry);
    worldBorderCollision(_registry);
    calcCollision(_registry);
    updateHealth(_registry, dt);
    //TODO prit it every 1s indipendenlty from fps
    if (++i > 50) {
        std::cout<<"sani:"<<_registry.view<healty>().size()<< "\tmalati:"<<_registry.view<ill>().size()<<"\tmorti:"<<(_registry.size() - _registry.view<healty>().size() - _registry.view<ill>().size() - _registry.view<recovered>().size())<<"\tguariti:"<<_registry.view<recovered>().size()<<std::endl;
        i = 0;
    }
    _occupancy.addSimTime();
}

//TODO add interpolation and decoupling to renderer
void Application::_render() {
    _window.clear();
    draw(_window, _registry);
    //drawQuadTreeDebugInfo(_window, getDebugRects());
    _occupancy.addDrwTime();
    _window.display();
}