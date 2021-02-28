#include <iostream>

#include <SFML/Graphics/RenderTarget.hpp>

#include "parameters.h"
#include "world.h"

#include "components/illness.h"
#include "components/position.h"
#include "components/velocity.h"

#include "systems/collisionDetector.h"
#include "systems/health.h"
#include "systems/movement.h"
#include "systems/renderer.h"

World::World(sf::RenderTarget& outputTarget, const TextureManager& textureManager)
: _target{outputTarget}
, _worldView(sf::FloatRect(0, 0, worldX, worldY))
, _gui(_target)
, _dispatcher{}
, _elapsed{sf::Time::Zero}
, _zoomLevel{0}
, _textureManager(textureManager)
{
    _initPopulation();
    _target.setView(_worldView);

    auto& man = _textureManager.get(Textures::man);
    _sprite.setTexture(man);
    _sprite.setScale(sf::Vector2f(0.002f, 0.002f));

    _dispatcher.sink<ZoomEvent>().connect<&World::zoom>(*this);
    _dispatcher.sink<PanEvent>().connect<&World::pan>(*this);
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

bool World::processInput(const sf::Event& ev) {
    return _gui.handleEvent(ev);
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
    _dispatcher.update<ZoomEvent>();
    _dispatcher.update<PanEvent>();
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

void World::_zoomIn() {
    if (_zoomLevel<10) {
        ++_zoomLevel;
        _worldView.zoom(0.5);
        _target.setView(_worldView);
        _ensureViewInsideLimits();
    }
}

void World::_zoomOut() {
    if (_zoomLevel>0) {
        --_zoomLevel;
        _worldView.zoom(2.0);
        _target.setView(_worldView);
        _ensureViewInsideLimits();
    }
}

void World::_panLeft() {
    _worldView.move(-1, 0);
    _target.setView(_worldView);
    _ensureViewInsideLimits();
}

void World::_panRight() {
    _worldView.move(1, 0);
    _target.setView(_worldView);
    _ensureViewInsideLimits();
}

void World::_panUp() {
    _worldView.move(0, -1);
    _target.setView(_worldView);
    _ensureViewInsideLimits();
}

void World::_panDown() {
    _worldView.move(0, 1);
    _target.setView(_worldView);
    _ensureViewInsideLimits();

}

void World::zoom(const ZoomEvent& ev) {
    if (ev.type == ZoomEvent::IN) {
        _zoomIn();
    } else {
        _zoomOut();
    }
}

void World::pan(const PanEvent& ev) {
    switch (ev.direction) {
    case PanEvent::UP :
        _panUp();
        break;
    case PanEvent::DOWN :
        _panDown();
        break;
    case PanEvent::LEFT :
        _panLeft();
        break;
    case PanEvent::RIGHT :
        _panRight();
        break;
    default:
        break;
    }
}

entt::dispatcher& World::getDispatcher() {
    return _dispatcher;
}