#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

#include "utils/statistics.h"

#include "player.h"
#include "world.h"


class Application {
public:
    Application();
    void run();
private:
    sf::RenderWindow _window;
    bool _isFullScreen;

    World _world;
    Player _player;

    sf::Clock _updateDt;
    EngineStatistics _statistics;

    void _processInput();
    void _update(sf::Time dt);
    void _render();
    
};

#endif