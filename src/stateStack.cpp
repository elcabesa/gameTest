#include <cassert>
#include "stateStack.h"

StateStack::StateStack(State::Context context)
: _stack()
, _pendingList()
, _context(context)
, _factories() {
}

void StateStack::update(sf::Time dt) {
	// Iterate from top to bottom, stop as soon as update() returns false
	for (auto itr = _stack.rbegin(); itr != _stack.rend(); ++itr) {
		if ((*itr)->update(dt) == State::PROPAGATE::STOP) {
			break;
        }
	}

	applyPendingChanges();
}

void StateStack::render() {
	// Draw all active states from bottom to top
	for (auto& state: _stack) {
		state->render();
    }
}

void StateStack::handleEvent(const sf::Event& event) {
	// Iterate from top to bottom, stop as soon as handleEvent() returns false
	for (auto itr = _stack.rbegin(); itr != _stack.rend(); ++itr) {
		if ((*itr)->handleEvent(event) == State::PROPAGATE::STOP) {
			break;
        }
	}

	applyPendingChanges();
}

void StateStack::pushState(States::ID stateID) {
	_pendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState() {
	_pendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates() {
	_pendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const {
	return _stack.empty();
}

State::Ptr StateStack::createState(States::ID stateID) {
	auto found = _factories.find(stateID);
	assert(found != _factories.end());

	return found->second();
}

void StateStack::applyPendingChanges() {
	for(auto& change: _pendingList) {
		switch (change.action) {
			case Push:
				_stack.push_back(createState(change.stateID));
				break;
			case Pop:
				_stack.pop_back();
				break;
			case Clear:
				_stack.clear();
				break;
		}
	}

	_pendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action_, States::ID stateID_)
: action(action_)
, stateID(stateID_) {
}
