#include "pauseState.h"

PauseState::PauseState(StateStack& stack, Context context)
: State(stack, context) {}

PauseState::~PauseState() {
}

void PauseState::render() {
}

State::PROPAGATE PauseState::update(sf::Time) {
    return State::PROPAGATE::STOP;
}

State::PROPAGATE PauseState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        requestStackPop();
    }
	return State::PROPAGATE::STOP;
}
