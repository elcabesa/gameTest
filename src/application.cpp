
#include "spdlog/sinks/stdout_color_sinks.h"
#include "application.h"
#include "gameState.h"
#include "parameters.h"
#include "pauseState.h"

//TODO find a good name for application
Application::Application()
: _window(sf::VideoMode(dimX, dimY), "Simulator", sf::Style::Resize | sf::Style::Close) 
, _isFullScreen{false}
, _textureManager{}
, _context{_window, _textureManager}
, _stateStack(_context)
{
    _logger = spdlog::stdout_color_mt("Application");
    _logger->set_level(spdlog::level::info);
    _logger->trace("Application ctor");

    _textureManager.load(Textures::man,	"data/man.png");
    auto& man = _textureManager.get(Textures::man);
    man.setSmooth(true);

    _registerStates();
    _stateStack.pushState(States::ID::Game);

    _configureWindow();
}

void Application::run() {
    _logger->info("Application start");
    const sf::Time TimePerFrame = sf::seconds(1.f/simSpeed);

    // create a clock to track the elapsed time
    _updateDt.restart();
    // run the main loop
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (_window.isOpen())
    {
        _statistics.addDisTime();

        timeSinceLastUpdate += _updateDt.restart();
        int i = 0;
        while (timeSinceLastUpdate > TimePerFrame && i < 10) {
            timeSinceLastUpdate -= TimePerFrame;
			_processInput();
			_update(TimePerFrame);
            ++i;
            if(i == 10) {
                _logger->warn("system is too slow");
            }
		}
        _render();
        
        _statistics.print();
    }
    _logger->trace("Exit Application");
}

void Application::_processInput() {
    _logger->trace("Process input");
    // handle events
    sf::Event event;
    while (_window.pollEvent(event)) {
        _logger->debug("New Event");
        // program events
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::F11) {
                _logger->debug("F11, fullscreen toggle");
                _isFullScreen = !_isFullScreen;
                _window.create( _isFullScreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode(dimX, dimY), "Simulator", _isFullScreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close);
                _configureWindow();
            }
            if (event.key.code == sf::Keyboard::F1) {
                _logger->debug("F1, setLog Level Info");
                _toggleLoggerLevel(spdlog::level::info);
            }
            if (event.key.code == sf::Keyboard::F2) {
                _logger->debug("F2, setLog Level Debug");
                _toggleLoggerLevel(spdlog::level::debug);
            }
            if (event.key.code == sf::Keyboard::F3) {
                _logger->debug("F3, setLog Level Trace");
                _toggleLoggerLevel(spdlog::level::trace);
            }

        }

        _stateStack.handleEvent(event);
        
        if (event.type == sf::Event::Closed) {
            _logger->debug("Close Event");
            _window.close();
        }
    }
    _statistics.addEvtTime();
}

void Application::_update(sf::Time dt) {
    _logger->trace("Update");
    _stateStack.update(dt);
    _statistics.addSimTime();
}

//TODO add lagtime
void Application::_render() {
    _logger->trace("Render");
    _window.clear();
    _stateStack.render();
    _statistics.addDrwTime();
    _window.display();
}

void Application::_configureWindow() {
    //_window.setVerticalSyncEnabled(true);
    _window.setFramerateLimit(fps);
    _window.setKeyRepeatEnabled(false);
}

void Application::_toggleLoggerLevel(spdlog::level::level_enum l) {
    _logger->set_level(l);
    spdlog::get("Statistics")->set_level(l);
}

void Application::_registerStates() {
    _stateStack.registerState<GameState>(States::ID::Game);
    _stateStack.registerState<PauseState>(States::ID::Pause);
}