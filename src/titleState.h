#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include <TGUI/TGUI.hpp>
#include "state.h"

class TitleState : public State {
public:
    TitleState(StateStack& stack, Context context);
    ~TitleState();

    virtual void render();
    virtual State::PROPAGATE update(sf::Time dt);
    virtual State::PROPAGATE handleEvent(const sf::Event& event);
private:
    tgui::Gui _gui;
};

#endif