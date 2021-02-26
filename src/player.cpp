#include "player.h"
#include "world.h"

Player::Player(entt::dispatcher& d)
: _dispatcher{d}
{
    _initKeyBindings();
}

void Player::_initKeyBindings() {
    assignKey(PanLeft,  sf::Keyboard::Left);
	assignKey(PanRight, sf::Keyboard::Right);
	assignKey(PanUp,    sf::Keyboard::Up);
	assignKey(PanDown,  sf::Keyboard::Down);
    assignKey(ZoomIn,   sf::Keyboard::Add);
    assignKey(ZoomOut,  sf::Keyboard::Subtract);
}

void Player::assignKey(Action action, sf::Keyboard::Key key) {
    for (auto it = _keyBinding.begin(); it != _keyBinding.end(); ) {
        if (it->second == action) {
            it = _keyBinding.erase(it);
        } else {
            ++it;
        }
    }
    _keyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const {
    for(const auto& pair: _keyBinding) {
		if (pair.second == action) {
			return pair.first;
        }
	}

	return sf::Keyboard::Unknown;
}

bool Player::_isRealtimeAction(const Action a) const {
    switch (a) {
        case PanLeft:
        case PanRight:
        case PanUp:
        case PanDown:
            return true;
        default:
            return false;
    }
}

void Player::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        // Check if pressed key appears in key binding, trigger command if so
        auto found = _keyBinding.find(event.key.code);
        if (found != _keyBinding.end() && !_isRealtimeAction(found->second)) {
            _enqueueAction(found->second);
        }   
    }
}

void Player::handleRealTimeEvent() {
    // Traverse all assigned keys and check if they are pressed
	for(const auto& pair: _keyBinding) {
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && _isRealtimeAction(pair.second)) {
			_enqueueAction(pair.second);
        }
	}
}

void Player::_enqueueAction(const Action a) {
    //TODO can be done with a map??
    switch (a) {
        case PanLeft:
            _dispatcher.enqueue<PanEvent>(PanEvent::LEFT);  
            break;
        case PanRight:
            _dispatcher.enqueue<PanEvent>(PanEvent::RIGHT);  
            break;
        case PanUp:
            _dispatcher.enqueue<PanEvent>(PanEvent::UP);  
            break;
        case PanDown:
            _dispatcher.enqueue<PanEvent>(PanEvent::DOWN);  
            break;
        case ZoomOut:
            _dispatcher.enqueue<ZoomEvent>(ZoomEvent::OUT);  
            break;
        case ZoomIn:
            _dispatcher.enqueue<ZoomEvent>(ZoomEvent::IN);  
            break;
        default:
            //TODO log unexpected action
            return;
    }
}

