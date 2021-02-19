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
, _world{_window}
{
    // create the window
    //_window.setVerticalSyncEnabled(true);
    _window.setFramerateLimit(fps);
}




void Application::run() {
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
                std::cout<<"warning"<<std::endl;
            }
		}
        _render();
        
        _statistics.print();
    }
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
                // TODO create a function to setup widow all the time there is a change
                //_window.setVerticalSyncEnabled(true);
                _window.setFramerateLimit(fps);
            }
        }

        if (!_world.processInput(event)) {
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
    _world.update(dt);
    _statistics.addSimTime();
}

//TODO add lagtime
void Application::_render() {

    
    _window.clear();
    _world.render();
    _statistics.addDrwTime();
    _window.display();
}