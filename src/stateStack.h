#ifndef STATE_STACK_H
#define STATE_STACK_H

#include <functional>
#include <map>
#include <vector>
#include <SFML/System/NonCopyable.hpp>

#include "state.h"
#include "stateIdentifiers.h"

class StateStack : private sf::NonCopyable
{
public:
    enum Action
    {
        Push,
        Pop,
        Clear
    };


public:		
    explicit StateStack(State::Context context);

    template <typename T>
    void registerState(States::ID stateID);

    void update(sf::Time dt);
    void render();
    void handleEvent(const sf::Event& event);

    void pushState(States::ID stateID);
    void popState();
    void clearStates();

    bool isEmpty() const;


private:
    State::Ptr createState(States::ID stateID);
    void applyPendingChanges();

private:
    struct PendingChange {
        explicit PendingChange(Action action_, States::ID stateID_ = States::None);

        Action action;
        States::ID stateID;
    };


private:
    std::vector<State::Ptr> _stack;
    std::vector<PendingChange> _pendingList;

    State::Context _context;
    std::map<States::ID, std::function<State::Ptr()>> _factories;
};


template <typename T>
void StateStack::registerState(States::ID stateID) {
	_factories[stateID] = [this] ()	{
		return State::Ptr(new T(*this, _context));
	};
}

#endif