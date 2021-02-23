#include <iostream>

#include "parameters.h"
#include "world.h"

#include "components/illness.h"
#include "components/position.h"
#include "components/velocity.h"

#include "systems/collisionDetector.h"
#include "systems/health.h"
#include "systems/movement.h"
#include "systems/renderer.h"


World::World(sf::RenderTarget& outputTarget)
: _target{outputTarget}
, _worldView(sf::FloatRect(0, 0, worldX, worldY))
, _gui(_target)
, _elapsed{sf::Time::Zero}
, _zoomLevel{0}
, _textureManager{}
{
    _initPopulation();
    _target.setView(_worldView);

    _textureManager.load(Textures::man,	"data/man.png");
    auto& man = _textureManager.get(Textures::man);
    man.setSmooth(true);
    _sprite.setTexture(man);
    _sprite.setScale(sf::Vector2f(0.002f, 0.002f));
}

void World::_initPopulation() {
    for(auto i = 0u; i < population; ++i) {
        const auto entity = _registry.create();
        _registry.emplace<position>(entity, float(std::rand() % (worldX-2 * objectSize) + objectSize), float(std::rand() % (worldY-2 * objectSize) + objectSize));
        _registry.emplace<velocity>(entity, float((std::rand() % 50)/100.0 - 0.245), float((std::rand() % 50)/100.0 - 0.245));
        if (std::rand() % 1000 < illInitialPermill) {
            _registry.emplace<ill>(entity);
        }
        else {
            _registry.emplace<healthy>(entity);
        }
    }
    CD_init(_registry);
    _updateHealthyInfo();
}

void World::_updateHealthyInfo() {
    _gui.setHealthInfo(
        _registry.view<healthy>().size(),
        _registry.view<ill>().size(),
        _registry.size() - _registry.view<healthy>().size() - _registry.view<ill>().size() - _registry.view<recovered>().size(),
        _registry.view<recovered>().size()
    );
}

bool World::processInput(sf::Event ev) {
    //TODO decouple input & world using command list
    if(_gui.handleEvent(ev)) {
        return true;
    }

    if (ev.type == sf::Event::Resized)
    {
        //TODO WRITE CODE FOR resize
        // update the view to the new size of the window
        /*sf::FloatRect visibleArea(0.f, 0.f, ev.size.width, ev.size.height);
        _worldView.reset(visibleArea);
        _target.setView(_worldView);*/
        return true;
    }
    if (ev.type == sf::Event::KeyPressed)
    {
        if (ev.key.code == sf::Keyboard::Add)
        {
            if (_zoomLevel<10) {
                ++_zoomLevel;
                _worldView.zoom(0.5);
                _target.setView(_worldView);
                _ensureViewInsideLimits();
            }

            return true;
        }
        if (ev.key.code == sf::Keyboard::Subtract)
        {
            if (_zoomLevel>0) {
                --_zoomLevel;
                _worldView.zoom(2.0);
                _target.setView(_worldView);
                _ensureViewInsideLimits();
            }
            return true;
        }
        if (ev.key.code == sf::Keyboard::Left)
        {
            _worldView.move(-1, 0);
            _target.setView(_worldView);
            _ensureViewInsideLimits();
            return true;
        }
        if (ev.key.code == sf::Keyboard::Right)
        {
            _worldView.move(1, 0);
            _target.setView(_worldView);
            _ensureViewInsideLimits();
            return true;
        }
        if (ev.key.code == sf::Keyboard::Up)
        {
            _worldView.move(0, -1);
            _target.setView(_worldView);
            _ensureViewInsideLimits();
            return true;
        }
        if (ev.key.code == sf::Keyboard::Down)
        {
            _worldView.move(0, 1);
            _target.setView(_worldView);
            _ensureViewInsideLimits();
            return true;
        }
    }
    return false;

}

void World::_ensureViewInsideLimits() {
    auto b = _getViewBorders();
    if (b.left < 0) {
        _worldView.setCenter(_worldView.getSize().x / 2.f, _worldView.getCenter().y);
    }
    b = _getViewBorders();
    if (b.top < 0) {
        _worldView.setCenter(_worldView.getCenter().x, _worldView.getSize().y / 2.f);
    }
    b = _getViewBorders();
    if (b.left + b.width >= worldX) {
        _worldView.setCenter(worldX - _worldView.getSize().x / 2.f, _worldView.getCenter().y);
    }
    b = _getViewBorders();
    if (b.top + b.height >= worldY) {
        _worldView.setCenter(_worldView.getCenter().x , worldY - _worldView.getSize().y / 2.f);
    }
    _target.setView(_worldView);
}

void World::update(sf::Time dt) {
    updatePosition(_registry);
    worldBorderCollision(_registry);
    calcCollision(_registry);
    updateHealth(_registry, dt);

    _elapsed += dt;
    if (_elapsed.asSeconds() >= updateHealthTime) {
        // TODO insert a method in the world
        // TODO use an oberver and decouple?
        _updateHealthyInfo();
        _elapsed -= sf::seconds(updateHealthTime);
    }
}

void World::render() {
    // manage gui animation
    //TODO use elapsed time to have a smoth transition
    _gui.manageTransitions();

    draw(_target, _registry, _sprite, _zoomLevel);
    //drawQuadTreeDebugInfo(_target, getDebugRects());
    _gui.draw();

}

sf::FloatRect World::_getViewBorders() const {
    auto& center = _worldView.getCenter();
    auto& size = _worldView.getSize();
    return sf::FloatRect(center - size / 2.0f, size);
}
