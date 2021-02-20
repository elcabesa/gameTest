#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>

#include "entt/src/entt/entt.hpp"

#include "gui.h"

class World {
public:
    World(sf::RenderTarget& outputTarget);
    bool processInput(sf::Event ev);
    void update(sf::Time dt);
    void render();

private:
    sf::RenderTarget& _target;
	sf::View _worldView;

    Gui _gui; // TODO maybe move to gamestate??

    entt::registry _registry;

    sf::Time _elapsed;

    int _zoomLevel;
    sf::Texture _texture;
    sf::Sprite _sprite;

    void _initPopulation();
    void _updateHealthyInfo();
    sf::FloatRect _getViewBorders() const;
    void _ensureViewInsideLimits();

    static constexpr float updateHealthTime = 0.1f;
};

#endif