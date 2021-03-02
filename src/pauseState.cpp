
#include "pauseState.h"

PauseState::PauseState(StateStack& stack, Context context)
: State(stack, context)
, _gui(*context.target) {
    const int dimx = 100;
    const int dimy = 40;
    _gui.setAbsoluteViewport({0, 0, static_cast<float>(context.target->getSize().x), static_cast<float>(context.target->getSize().y)});
    tgui::Theme theme{"data/gui/Black.txt"};
    auto button = tgui::Button::create();
    button->setRenderer(theme.getRenderer("Button"));
    button->setPosition((_gui.getView().getSize().x- dimx) / 2.f, (_gui.getView().getSize().y -dimy) /2.f);
    button->setText("Pause");
    button->setSize(dimx, dimy);
    button->onPress([=]{ requestStackPop(); });
    _gui.add(button);
}

PauseState::~PauseState() {
}

void PauseState::render() {
    _gui.draw();
}

State::PROPAGATE PauseState::update(sf::Time) {
    return State::PROPAGATE::STOP;
}

State::PROPAGATE PauseState::handleEvent(const sf::Event& event) {
    _gui.handleEvent(event);

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        requestStackPop();
    }
	return State::PROPAGATE::STOP;
}
