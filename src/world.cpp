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
, _worldView(_target.getDefaultView())
, _gui(_target)
, _elapsed{sf::Time::Zero}
{
    _initPopulation();
}

void World::_initPopulation() {
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
    _updateHealtyInfo();
}

void World::_updateHealtyInfo() {
    _gui.setHealthInfo(
        _registry.view<healty>().size(),
        _registry.view<ill>().size(),
        _registry.size() - _registry.view<healty>().size() - _registry.view<ill>().size() - _registry.view<recovered>().size(),
        _registry.view<recovered>().size()
    );
}

bool World::processInput(sf::Event ev) {
    return _gui.handleEvent(ev);
}

void World::update(sf::Time dt) {
    updatePosition(_registry);
    worldBorderCollision(_registry);
    calcCollision(_registry);
    updateHealth(_registry, dt);

    _elapsed += dt;
    if (_elapsed.asSeconds() >= updateHealthTime) {
        //TODO insert a method in the world
        // TODO use an oberver and decouple?
        _updateHealtyInfo();
        _elapsed -= sf::seconds(updateHealthTime);
    }
}

void World::render() {
    // manage gui animation
    //TODO use elapsed time to have a smoth transition
    _gui.manageTransitions();

    draw(_target, _registry);
    //drawQuadTreeDebugInfo(_target, getDebugRects());
    _gui.draw();

}

