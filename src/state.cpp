#include <SFML/Graphics/RenderTarget.hpp>

#include "state.h"
#include "stateStack.h"

State::Context::Context(sf::RenderTarget& target_, TextureManager& textures_)
: target(&target_)
, textures(&textures_)
{}

State::State(StateStack& stack, Context context)
: _stack(&stack)
, _context{context} {
}

State::~State() {
}

State::Context State::getContext() const {
	return _context;
}

void State::requestStackPush(States::ID stateID) {
	_stack->pushState(stateID);
}

void State::requestStackPop() {
	_stack->popState();
}

void State::requestStateClear() {
	_stack->clearStates();
}