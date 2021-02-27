#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include "spdlog/spdlog.h"

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

    std::shared_ptr<spdlog::logger> _logger;

    void _processInput();
    void _update(sf::Time dt);
    void _render();
    void _configureWindow();
    void _toggleLoggerLevel(spdlog::level::level_enum l);
    
};

#endif