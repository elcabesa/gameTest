#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include "spdlog/spdlog.h"

#include "utils/statistics.h"

#include "player.h"
#include "stateStack.h"
#include "world.h"


class Application {
public:
    Application();
    void run();
private:
    sf::RenderWindow _window;
    bool _isFullScreen;

    TextureManager _textureManager;
    State::Context _context;
    StateStack _stateStack;

    sf::Clock _updateDt;
    EngineStatistics _statistics;

    std::shared_ptr<spdlog::logger> _logger;

    void _processInput();
    void _update(sf::Time dt);
    void _render();
    void _configureWindow();
    void _toggleLoggerLevel(spdlog::level::level_enum l);
    void _registerStates();
    
};

#endif