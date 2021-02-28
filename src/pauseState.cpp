#include "parameters.h"
#include "pauseState.h"

PauseState::PauseState(StateStack& stack, Context context)
: State(stack, context)
, _gui(*context.target) {
    _gui.setAbsoluteViewport({0, 0, dimX, dimY});
    tgui::Theme theme{"data/gui/Black.txt"};
    auto button = tgui::Button::create();
    button->setRenderer(theme.getRenderer("Button"));
    button->setPosition(_gui.getView().getSize().x / 2.f - 50.f, _gui.getView().getSize().y /2.f - 20.f);
    button->setText("Pause");
    button->setSize(100, 40);
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
