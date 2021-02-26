#ifndef PLAYER_H
#define PLAYER_H

#include "entt/src/entt/fwd.hpp"
#include <SFML/Window/Event.hpp>

#include <map>

class Player {
public:
    enum Action
    {
        PanLeft,
        PanRight,
        PanUp,
        PanDown,
        ZoomIn,
        ZoomOut,
        ActionCount
    };

    Player(entt::dispatcher& d);
    void assignKey(Action action, sf::Keyboard::Key key);
    sf::Keyboard::Key getAssignedKey(Action action) const;
    void handleEvent(const sf::Event& event);
    void handleRealTimeEvent();
private:
    void _initKeyBindings();
    bool _isRealtimeAction(const Action a) const;
    void _enqueueAction(const Action a);

    std::map<sf::Keyboard::Key, Action> _keyBinding;
	entt::dispatcher& _dispatcher;
};

#endif