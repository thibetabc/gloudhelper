#include "GameControllerJoystick.h"

NS_CC_BEGIN

namespace ui
{

    GameControllerJoystick::GameControllerJoystick()
    {

    }

    GameControllerJoystick::~GameControllerJoystick()
    {

    }

    bool GameControllerJoystick::init()
    {
        if (!ImageView::init())
            return false;

        ignoreContentAdaptWithSize(false);
        setAnchorPoint(Vec2(0.5f, 0.5f));

        _joystick = Button::create();
        _joystick->ignoreContentAdaptWithSize(false);
        addChild(_joystick);
        _joystick->setSwallowTouches(false);
        setTouchEnabled(true);

		cocos2d::Size size = Size::ZERO;
		cocos2d::Size jssize = Size::ZERO;
		Vec2 anchor = Vec2::ZERO;
		Vec2 beganpos = Vec2::ZERO;
		Vec2 movepos = Vec2::ZERO;
		float r = 0.0f;
		float ratio = 0.0f;
		float valx = 0.0f;
		float valy = 0.0f;

		addTouchEventListener([&](Ref * ref, Widget::TouchEventType type) {
            if (type == Widget::TouchEventType::BEGAN)
            {
                // set trigger state
                _joystick->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);

                size = getContentSize();
                jssize = _joystick->getContentSize();
                anchor = this->getPosition();
                beganpos = this->getTouchBeganPosition();
                beganpos.x -= anchor.x;
                beganpos.y -= anchor.y;
                r = std::sqrt(beganpos.x * beganpos.x + beganpos.y * beganpos.y);
                ratio = (r + jssize.width * 0.5f > size.width * 0.5f) ? ((size.width * 0.5f - jssize.width * 0.5f) / r) : 1.0f;
                _joystick->setPosition(Vec2(beganpos.x * ratio + size.width * 0.5f, beganpos.y * ratio + size.height * 0.5f));
                if (_handler != nullptr)
                {
                    valx = 0.0f;
                    valy = 0.0f;
                    if (r > _blind * sqrt(size.width * 0.5f * size.width * 0.5f + size.height * 0.5f * size.height * 0.5f))
                    {
                        valx = beganpos.x / r;
                        valy = beganpos.y / r;
                    }

//#ifdef VGC_SCENE_ROTATION_LEFT
//					valy = -valy;//tag:ROTATION
//#endif
                    _handler(_objx, GameControllerDef::EVENT::AXIS, valx, _objy, GameControllerDef::EVENT::AXIS, valy);
                }

                if (isScheduled(schedule_selector(GameControllerJoystick::btnTimer)))
                {
                    unschedule(schedule_selector(GameControllerJoystick::btnTimer));
                    _btndown = true;
                    this->loadTexture(_effect);
                    if (_btnhandler != nullptr)
                        _btnhandler(_objbtn, GameControllerDef::EVENT::DOWN, 1.0f);
                }
                else
                    scheduleOnce(schedule_selector(GameControllerJoystick::btnTimer), 0.4f);
            }
            else if (type == Widget::TouchEventType::ENDED || type == Widget::TouchEventType::CANCELED)
            {
                // reset button state
				this->loadTexture(_bg);
				if (_btndown && _btnhandler != nullptr)
                    _btnhandler(_objbtn, GameControllerDef::EVENT::UP, 0.0f);
                _btndown = false;

                // set trigger state
                _joystick->setBrightStyle(Widget::BrightStyle::NORMAL);

                size = getContentSize();
                _joystick->setPosition(Vec2(size.width * 0.5f, size.height * 0.5f));
                if (_handler != nullptr)
                    _handler(_objx, GameControllerDef::EVENT::AXIS, 0.0f, _objy, GameControllerDef::EVENT::AXIS, 0.0f);
            }
            else
            {
                // set trigger state
                _joystick->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);

                size = getContentSize();
                jssize = _joystick->getContentSize();
                anchor = this->getPosition();
                movepos = this->getTouchMovePosition();
                movepos.x -= anchor.x;
                movepos.y -= anchor.y;
                r = std::sqrt(movepos.x * movepos.x + movepos.y * movepos.y);
                ratio = (r + jssize.width * 0.5f > size.width * 0.5f) ? ((size.width * 0.5f - jssize.width * 0.5f) / r) : 1.0f;
                _joystick->setPosition(Vec2(movepos.x * ratio + size.width * 0.5f, movepos.y * ratio + size.height * 0.5f));
                if (_handler != nullptr)
                {
                    
                    valx = 0.0f;
                    valy = 0.0f;
                    if (r > _blind * sqrt(size.width * 0.5f * size.width * 0.5f + size.height * 0.5f * size.height * 0.5f))
                    {
                        valx = movepos.x / r;
                        valy = movepos.y / r;
					}
//#ifdef VGC_SCENE_ROTATION_LEFT
//					valy = -valy;//tag:ROTATION
//#endif
                    _handler(_objx, GameControllerDef::EVENT::AXIS, valx, _objy, GameControllerDef::EVENT::AXIS, valy);
                }
            }
        });

        return true;
    }

    void GameControllerJoystick::onEnter()
    {
        ImageView::onEnter();

        setTouchEnabled(true);
    }

    void GameControllerJoystick::onExit()
    {
        setTouchEnabled(false);

        ImageView::onExit();
    }

    void GameControllerJoystick::setObject(GameControllerDef::OBJECT objx, GameControllerDef::OBJECT objy, GameControllerDef::OBJECT objdbclk)
    {
        _objx = objx;
        _objy = objy;
        _objbtn = objdbclk;
    }

    void GameControllerJoystick::loadTextures(const std::string & bg, const std::string & effect, const std::string & normal, const std::string & press, const std::string & disable)
    {
        _bg = bg;
        _effect = effect;
        ImageView::loadTexture(bg);
        _joystick->setContentSize(cocos2d::Size(_contentSize.width / 2, _contentSize.height / 2));
        _joystick->loadTextures(normal, press, disable);
        _joystick->setPosition(Vec2(_contentSize.width / 2, _contentSize.height / 2));
    }

    void GameControllerJoystick::addJoystickEventListener(std::function<void (GameControllerDef::OBJECT, GameControllerDef::EVENT, float, GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> handler)
    {
        _handler = handler;
    }

    void GameControllerJoystick::addButtonEventListener(std::function<void(GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> handler)
    {
        _btnhandler = handler;
    }

    void GameControllerJoystick::btnTimer(float val)
    {
        // unschedule double click timer
        unschedule(schedule_selector(GameControllerJoystick::btnTimer));
    }

};

NS_CC_END
