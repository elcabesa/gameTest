#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "state.h"
#include "player.h"
#include "world.h"

class GameState : public State
{
public:
    GameState(StateStack& stack, Context context);

    virtual void render();
    virtual State::PROPAGATE update(sf::Time dt);
    virtual State::PROPAGATE handleEvent(const sf::Event& event);


private:
	World _world;
	Player _player;
};

#endif