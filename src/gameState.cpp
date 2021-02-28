#include "gameState.h"

GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, _world(*context.target, *context.textures) //TODO use the textureManger from the context
, _player{_world.getDispatcher()} {
}

void GameState::render()
{
    _world.render();
}

State::PROPAGATE GameState::update(sf::Time dt)
{
    _world.update(dt);
    _player.handleRealTimeEvent();
    return State::PROPAGATE::CONTINUE;
}

State::PROPAGATE GameState::handleEvent(const sf::Event& event)
{
    // Game input handling
    if (!_world.processInput(event)) {
        // TODO manage resize
        _player.handleEvent(event);
    }

    // Escape pressed, trigger the pause screen
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        requestStackPush(States::Pause);
    }

    return State::PROPAGATE::CONTINUE;
}