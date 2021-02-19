#ifndef GUI_H
#define GUI_H

#include <TGUI/TGUI.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Gui {
public:
    Gui(sf::RenderWindow & win);
    bool handleEvent(sf::Event ev);
    void draw();
    void manageTransitions();
    void setHealthInfo(unsigned int healty = 0, unsigned int ill = 0, unsigned int death = 0, unsigned int recovered = 0);
private:
    tgui::Gui _gui;
    int _buttonPosition;
    bool _animateButton;
    bool _hideButton;
};

#endif