#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>

#include "entt/src/entt/entt.hpp"

#include "utils/resourceManager.h"

#include "gui.h"
#include "resourceIdentifiers.h"

struct ZoomEvent { 
    enum Type{
        IN,
        OUT
    };
    Type type; };
    
struct PanEvent { 
    enum Direction{
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    Direction direction;
};

class World {
public:
    World(sf::RenderTarget& outputTarget);
    bool processInput(sf::Event ev);
    void update(sf::Time dt);
    void render();
    void zoom(const ZoomEvent&);
    void pan(const PanEvent&);
    entt::dispatcher& getDispatcher();

private:
    sf::RenderTarget& _target;
	sf::View _worldView;

    Gui _gui; // TODO maybe move to gamestate??

    entt::registry _registry;
    entt::dispatcher _dispatcher;

    sf::Time _elapsed;

    int _zoomLevel;
    TextureManager _textureManager;
    sf::Sprite _sprite;

    void _initPopulation();
    void _updateHealthyInfo();
    sf::FloatRect _getViewBorders() const;
    void _ensureViewInsideLimits();

    static constexpr float updateHealthTime = 0.1f;

    void _zoomIn();
    void _zoomOut();
    void _panLeft();
    void _panRight();
    void _panUp();
    void _panDown();
};

#endif