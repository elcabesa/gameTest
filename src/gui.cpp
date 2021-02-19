#include "gui.h"
#include "parameters.h"

Gui::Gui(sf::RenderTarget & win)
: _gui(win)
, _buttonPosition{0}
, _animateButton{false}
, _hideButton{false}
{
    _gui.setAbsoluteViewport({0, 0, dimX, dimY});
    tgui::Theme theme{"data/gui/Black.txt"};
    tgui::Panel::Ptr container = tgui::Panel::create();
    container->setRenderer(theme.getRenderer("Panel"));
    container->setPosition(_buttonPosition, 0);
    container->setSize(300, 100);
    _gui.add(container, "container");

    auto layout = tgui::VerticalLayout::create();
    container->add(layout, "verticalLayout");


    tgui::Label::Ptr lab = tgui::Label::create();
    //lab->setText("Sani:");
    lab->setRenderer(theme.getRenderer("Label"));
    layout->add(lab, "labHealthy");
    tgui::Label::Ptr lab2 = tgui::Label::create();
    //lab2->setText("Malati:");
    lab2->setRenderer(theme.getRenderer("Label"));
    layout->add(lab2, "labIll");
    tgui::Label::Ptr lab3 = tgui::Label::create();
    //lab3->setText("Morti:");
    lab3->setRenderer(theme.getRenderer("Label"));
    layout->add(lab3, "labDeath");
    tgui::Label::Ptr lab4 = tgui::Label::create();
    //lab4->setText("Guariti:");
    lab4->setRenderer(theme.getRenderer("Label"));
    layout->add(lab4, "labRecovered");

    container->onClick([&]{ _hideButton = !_hideButton; _animateButton = true; });

    setHealthInfo();
    
}

bool Gui::handleEvent(sf::Event ev) {
    return _gui.handleEvent(ev);
}

void Gui::draw() {
    _gui.draw();
}

void Gui::manageTransitions() {
    // TODO rewrite!!
    if (_animateButton) {
        auto widget = _gui.get("container");
        if (_hideButton) {
            _buttonPosition -= 10; 
            if (_buttonPosition <= -230) {
                _buttonPosition = -230;
               _animateButton = false; 
            }
        } else {
            _buttonPosition += 10;
            if (_buttonPosition >= 0) {
                _buttonPosition = 0;
               _animateButton = false; 
            }
        }
        widget->setPosition(_buttonPosition, 0);
    }
}

void Gui::setHealthInfo(unsigned int healty, unsigned int ill, unsigned int death, unsigned int recovered) {
    if (!_animateButton && _hideButton) {
        _gui.get<tgui::Label>("labHealthy")  ->setText("S:" + std::to_string(healty)   );
        _gui.get<tgui::Label>("labIll")      ->setText("M:" + std::to_string(ill)      );
        _gui.get<tgui::Label>("labDeath")    ->setText("M:" + std::to_string(death)    );
        _gui.get<tgui::Label>("labRecovered")->setText("G:" + std::to_string(recovered));
        _gui.get<tgui::VerticalLayout>("verticalLayout")->setPosition(230, 0);

    } else {
        _gui.get<tgui::Label>("labHealthy")  ->setText("Sani:   " + std::to_string(healty)   );
        _gui.get<tgui::Label>("labIll")      ->setText("Malati: " + std::to_string(ill)      );
        _gui.get<tgui::Label>("labDeath")    ->setText("Morti:  " + std::to_string(death)    );
        _gui.get<tgui::Label>("labRecovered")->setText("Guariti:" + std::to_string(recovered));
        _gui.get<tgui::VerticalLayout>("verticalLayout")->setPosition(0, 0);
    }
    
}