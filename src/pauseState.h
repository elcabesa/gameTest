#ifndef PAUSE_STATE_H
#define PAUSE STATE_H

#include <TGUI/TGUI.hpp>
#include "state.h"

class PauseState : public State {
public:
    PauseState(StateStack& stack, Context context);
    ~PauseState();

    virtual void render();
    virtual State::PROPAGATE update(sf::Time dt);
    virtual State::PROPAGATE handleEvent(const sf::Event& event);
private:
    tgui::Gui _gui;

};

#endif