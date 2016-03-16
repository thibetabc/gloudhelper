#include "GameControllerTrigger.h"

NS_CC_BEGIN

namespace ui
{

    GameControllerTrigger::GameControllerTrigger()
    {

    }

    GameControllerTrigger::~GameControllerTrigger()
    {

    }

    bool GameControllerTrigger::init()
    {
        if (!ImageView::init())
            return false;

        ignoreContentAdaptWithSize(false);
        _trigger = Button::create();
        addChild(_trigger);

        if (_trigger != nullptr)
            _trigger->setSwallowTouches(false);

        setTouchEnabled(true);
        addTouchEventListener([&](Ref * ref, Widget::TouchEventType type) {
            if (type == Widget::TouchEventType::BEGAN)
            {
                // set trigger state
                _trigger->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);

                Size size = this->getContentSize();
                Vec2 anchor = this->getPosition();
                Vec2 beganpos = this->getTouchBeganPosition();
                beganpos.x -= anchor.x - size.width / 2;
                beganpos.y -= anchor.y - size.height / 2;
                if (_trigger != nullptr)
                    _trigger->setPosition(Vec2(size.width / 2, beganpos.y));
                _handler(_object, GameControllerDef::EVENT::AXIS, beganpos.y / size.height);
            }
            else if (type == Widget::TouchEventType::ENDED || type == Widget::TouchEventType::CANCELED)
            {
                // set trigger state
                _trigger->setBrightStyle(Widget::BrightStyle::NORMAL);

                Size size = this->getContentSize();
                if (_trigger != nullptr)
                    _trigger->setPosition(Vec2(size.width / 2, _trigger->getContentSize().height / 2));
            }
            else
            {
                Size size = this->getContentSize();
                Size triggersize = this->_trigger->getContentSize();
                Vec2 anchor = this->getPosition();
                Vec2 movepos = this->getTouchMovePosition();
                movepos.x = size.width / 2;
                movepos.y -= anchor.y - size.height / 2;
                if (movepos.y < triggersize.height / 2)
                    movepos.y = triggersize.height / 2;
                else if (movepos.y > size.height - triggersize.height / 2)
                    movepos.y = size.height - triggersize.height / 2;
                if (_trigger != nullptr)
                    _trigger->setPosition(Vec2(movepos.x, movepos.y));
                _handler(_object, GameControllerDef::EVENT::AXIS, (movepos.y - triggersize.height / 2) / (size.height - triggersize.height));
            }
        });

        return true;
    }

    void GameControllerTrigger::setObject(GameControllerDef::OBJECT object)
    {
        _object = object;
    }

    void GameControllerTrigger::loadTextures(const std::string & bg, const std::string & normal, const std::string & press, const std::string & disable)
    {
        ImageView::loadTexture(bg);
        _trigger->setContentSize(Size(_contentSize.width / 2, _trigger->getContentSize().height / 2));
        _trigger->loadTextures(normal, press);
        _trigger->setPosition(Vec2(_contentSize.width / 2, _trigger->getContentSize().height / 2));
    }

    void GameControllerTrigger::addTriggerEventListener(std::function<void(GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> handler)
    {
        _handler = handler;
    }

}

NS_CC_END
