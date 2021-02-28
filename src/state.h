#ifndef STATE_H
#define STATE_H

#include <memory>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include "resourceIdentifiers.h"
#include "stateIdentifiers.h"



// forward declaration
namespace sf {
	class RenderTarget;
}

class StateStack;

class State {
public:
	using Ptr = std::unique_ptr<State>;

    enum PROPAGATE{
        CONTINUE,
        STOP
    };

    struct Context
	{
        Context(sf::RenderTarget& target, TextureManager& textures);

        sf::RenderTarget* target;
        TextureManager* textures;
    };


    State(StateStack& stack, Context context);
	virtual	~State();

    virtual void render() = 0;
    virtual PROPAGATE update(sf::Time dt) = 0;
    virtual PROPAGATE handleEvent(const sf::Event& event) = 0;


protected:
    void requestStackPush(States::ID stateID);
    void requestStackPop();
    void requestStateClear();

    Context getContext() const;


private:
    StateStack* _stack;
    Context _context; // TODO why not a pointer?
};
#endif