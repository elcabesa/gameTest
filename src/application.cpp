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
: _window(sf::VideoMode(dimX, dimY), "Simulator", sf::Style::Resize | sf::Style::Close) 
, _isFullScreen{false}
, _gui(_window)
, _buttonPosition{0}
, _animateButton{false}
, _hideButton{false} {
    // create the window
    //_window.setVerticalSyncEnabled(true);
    _window.setFramerateLimit(fps);

    _gui.setAbsoluteViewport({0, 0, dimX, dimY});
    tgui::Button::Ptr container = tgui::Button::create();
    container->setPosition(_buttonPosition, 0);
    container->setSize(300, 100);
    container->setText("ciao");
    _gui.add(container, "button");
    container->onPress([&]{ _hideButton = !_hideButton; _animateButton = true; });
}




void Application::run() {
    const sf::Time TimePerFrame = sf::seconds(1.f/simSpeed);

    //todo move inside world/game
    _initPopulation();

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
                std::cout<<"warning"<<std::endl;
            }
		}
        _render();
        
        _statistics.print();
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
    CD_init(_registry);
}

void Application::_processInput() {
    // handle events
    sf::Event event;
    while (_window.pollEvent(event)) {
        // program events
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::F11) {
                _isFullScreen = !_isFullScreen;
                _window.create( _isFullScreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode(dimX, dimY), "Simulator", _isFullScreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close);
            }
        }

        if (!_gui.handleEvent(event)) {
            //std::cout<<"event"<<std::endl;
            // TODO handle events for the game
        }
        if (event.type == sf::Event::Closed) {
            _window.close();
        }
    }
    _statistics.addEvtTime();
}

void Application::_update(sf::Time dt) {
    static sf::Time elapsed = sf::Time::Zero;
    updatePosition(_registry);
    worldBorderCollision(_registry);
    calcCollision(_registry);
    updateHealth(_registry, dt);

    elapsed += dt;
    if (elapsed.asSeconds() >= 1.f) {
        //TODO insert a method in the world
        std::cout<<"sani:"<<_registry.view<healty>().size()<< "\tmalati:"<<_registry.view<ill>().size()<<"\tmorti:"<<(_registry.size() - _registry.view<healty>().size() - _registry.view<ill>().size() - _registry.view<recovered>().size())<<"\tguariti:"<<_registry.view<recovered>().size()<<std::endl;
        elapsed -= sf::seconds(1.f);
    }
    _statistics.addSimTime();
}

//TODO add lagtime
void Application::_render() {
    // manage gui animation
    if (_animateButton) {
        auto widget = _gui.get("button");
        if (_hideButton) {
            _buttonPosition -= 10; 
            if (_buttonPosition <= -250) {
                _buttonPosition = -250;
               _animateButton = false; 
            }
        } else {
            _buttonPosition += 10;
            if (_buttonPosition >= 0) {
                _buttonPosition = 0;
               _animateButton = false; 
            }
        }
        widget->setPosition(_buttonPosition, 0);
    }

    _window.clear();
    draw(_window, _registry);
    drawQuadTreeDebugInfo(_window, getDebugRects());
    _gui.draw();
    _statistics.addDrwTime();
    _window.display();
}