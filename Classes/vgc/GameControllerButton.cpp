#include "GameControllerButton.h"

NS_CC_BEGIN

namespace ui
{
    GameControllerButton::GameControllerButton()
    {

    }

    GameControllerButton::~GameControllerButton()
    {

    }

    bool GameControllerButton::init()
    {
        if (!Button::init())
            return false;

        ignoreContentAdaptWithSize(false);
        setTouchEnabled(false);
        addTouchEventListener([&](Ref * ref, Widget::TouchEventType event) {

            if (_handler == nullptr)
                return;

            if (event == Widget::TouchEventType::BEGAN)
            {
                setBrightStyle(BrightStyle::HIGHLIGHT);
                _handler(_object, GameControllerDef::EVENT::DOWN, 1.0f);
            }
            else if (event == Widget::TouchEventType::ENDED || event == Widget::TouchEventType::CANCELED)
            {
                setBrightStyle(BrightStyle::NORMAL);
                _handler(_object, GameControllerDef::EVENT::UP, 0.0f);
            }
            else
                setBrightStyle(BrightStyle::HIGHLIGHT);
        });

        return true;
    }

    void GameControllerButton::onEnter()
    {
        Button::onEnter();

        setTouchEnabled(true);
    }

    void GameControllerButton::onExit()
    {
        setTouchEnabled(false);

        Button::onExit();
    }

    void GameControllerButton::setObject(GameControllerDef::OBJECT object)
    {
        _object = object;
    }

    void GameControllerButton::loadTextures(const std::string & normal, const std::string & press, const std::string & disable)
    {
        Button::loadTextures(normal, press, disable);
    }

    void GameControllerButton::addButtonEventListener(std::function<void (GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> handler)
    {
        _handler = handler;
    }

};

NS_CC_END
