#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

#include "entt/src/entt/entt.hpp"

#include "utils/statistics.h"

class Application {
public:
    Application();
    void run();
private:
    sf::RenderWindow _window;
    bool _isFullScreen;
    // TODO move inside game/world
    entt::registry _registry;

    sf::Clock _updateDt;
    EngineStatistics _statistics;

    // TODO move inside game/world
    void _initPopulation();

    void _processInput();
    void _update(sf::Time dt);
    void _render();
    
};

#endif