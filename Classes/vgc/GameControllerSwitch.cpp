#include "GameControllerSwitch.h"

NS_CC_BEGIN

namespace ui
{

    GameControllerSwitch::GameControllerSwitch()
    {

    }

    GameControllerSwitch::~GameControllerSwitch()
    {

    }

    bool GameControllerSwitch::init()
    {
        if (!Button::init())
            return false;

        ignoreContentAdaptWithSize(false);
        setTouchEnabled(false);
        addTouchEventListener([&](Ref * ref, Widget::TouchEventType event) {
            if (!isEnabled())
                return;

            if (event == Widget::TouchEventType::BEGAN)
            {
                _on = !_on;

                if (_on)
                {
                    setBrightStyle(BrightStyle::HIGHLIGHT);
                    if (_handler != nullptr)
                        _handler(_object, GameControllerDef::EVENT::DOWN, 1.0f);
                }
                else
                {
                    setBrightStyle(BrightStyle::NORMAL);
                    if (_handler != nullptr)
                        _handler(_object, GameControllerDef::EVENT::UP, 0.0f);
                }
            }
        });

        return true;
    }

    void GameControllerSwitch::onEnter()
    {
        Button::onEnter();

        setTouchEnabled(true);
    }

    void GameControllerSwitch::onExit()
    {
        setTouchEnabled(false);

        Button::onExit();
    }

    void GameControllerSwitch::setObject(GameControllerDef::OBJECT object)
    {
        _object = object;
    }

    void GameControllerSwitch::loadTextures(const std::string & normal, const std::string & press, const std::string & disable /*= ""*/)
    {
        Button::loadTextures(normal, press, disable);
    }

    void GameControllerSwitch::addSwitchEventListener(std::function<void(GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> handler)
    {
        _handler = handler;
    }
};

NS_CC_END
