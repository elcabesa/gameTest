#include "titleState.h"

TitleState::TitleState(StateStack& stack, Context context)
: State(stack, context)
, _gui(*context.target) {
    _gui.setAbsoluteViewport({0, 0, static_cast<float>(context.target->getSize().x), static_cast<float>(context.target->getSize().y)});
    tgui::Theme theme{"data/gui/Black.txt"};
    auto label = tgui::Label::create();
    label->setRenderer(theme.getRenderer("Label"));
    label->setText("SFML\nV0.0.1");
    label->setPosition(_gui.getView().getSize().x / 2.f - 50.f, _gui.getView().getSize().y /2.f - 20.f);
    label->setTextSize(18);
    _gui.add(label);
    //TODO set the string centered
}

TitleState::~TitleState() {
}

void TitleState::render() {
    _gui.draw();
}

State::PROPAGATE TitleState::update(sf::Time) {
    return State::PROPAGATE::STOP;
}

State::PROPAGATE TitleState::handleEvent(const sf::Event& event) {
    _gui.handleEvent(event);

    if (event.type == sf::Event::KeyReleased) {
        requestStackPop();
        requestStackPush(States::ID::Game);
    }
	return State::PROPAGATE::STOP;
}