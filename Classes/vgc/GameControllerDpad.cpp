#include "GameControllerDpad.h"

NS_CC_BEGIN

namespace ui
{

#define PI 3.1415926535897932385f

    static float sin_22_5 = sin(PI / 8);
    static float cos_67_5 = cos(PI * 3 / 8);

    GameControllerDpad::GameControllerDpad()
    {

    }

    GameControllerDpad::~GameControllerDpad()
    {

    }

    bool GameControllerDpad::init()
    {
        if (!ImageView::init())
            return false;
		int mSceneRotation = 0;

#ifdef VGC_SCENE_ROTATION_LEFT
		mSceneRotation = UserDefault::getInstance()->getIntegerForKey(SCENE_ROTATION_KEY, VGC_SCENE_ROTATION_LEFT);
#endif
        ignoreContentAdaptWithSize(false);
        setAnchorPoint(Vec2(0.5f, 0.5f));

        // up
        _up = Button::create();
        _up->ignoreContentAdaptWithSize(false);
        addChild(_up);
        _up->setSwallowTouches(false);
        _upbright = false;

        // down
        _down = Button::create();
        _down->ignoreContentAdaptWithSize(false);
        addChild(_down);
        _down->setSwallowTouches(false);
        _downbright = false;

        // left
        _left = Button::create();
        _left->ignoreContentAdaptWithSize(false);
        addChild(_left);
        _left->setSwallowTouches(false);
        _leftbright = false;

        // right
        _right = Button::create();
        _right->ignoreContentAdaptWithSize(false);
        addChild(_right);
        _right->setSwallowTouches(false);
        _rightbright = false;

        // center
        _effect = Button::create();
        _effect->ignoreContentAdaptWithSize(false);
        addChild(_effect);
        _effect->setSwallowTouches(false);

        setTouchEnabled(true);
        addTouchEventListener([&](Ref * ref, Widget::TouchEventType type) {
            if (type == Widget::TouchEventType::BEGAN)
            {
                Size size = this->getContentSize();
                Vec2 anchor = this->getPosition();
                Vec2 beganpos = this->getTouchBeganPosition();
                beganpos.x -= anchor.x;
                beganpos.y -= anchor.y;
                float r = std::sqrt(beganpos.x * beganpos.x + beganpos.y * beganpos.y);
                if (r > 0.0f)
                {
                    if (beganpos.y > 0.0f)
                        _effect->setRotation(360 - acos(beganpos.x / r) * 180 / PI);
                    else
                        _effect->setRotation(acos(beganpos.x / r) * 180 / PI);
                }

                float valx = 0.0f;
                float valy = 0.0f;
                if (r > _blind * sqrt(size.width * 0.5f * size.width * 0.5f + size.height * 0.5f * size.height * 0.5f))
                {
                    // set effect on
                    _effect->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);

                    valx = beganpos.x / r;
					valy = beganpos.y / r;

                    // up
                    if (valy > sin_22_5)
                    {
                        if (!_upbright)
                        {
                            _up->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);
                            _upbright = true;
                            if (_handler != nullptr)
                                _handler(_objup, GameControllerDef::EVENT::DOWN, 1.0f);
                        }
                    }
                    else
                    {
                        if (_upbright)
                        {
                            _up->setBrightStyle(Widget::BrightStyle::NORMAL);
                            _upbright = false;
                            if (_handler != nullptr)
                                _handler(_objup, GameControllerDef::EVENT::UP, 0.0f);
                        }
                    }

                    // down
                    if (valy < -sin_22_5)
                    {
                        if (!_downbright)
                        {
                            _down->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);
                            _downbright = true;
                            if (_handler != nullptr)
                                _handler(_objdown, GameControllerDef::EVENT::DOWN, 1.0f);
                        }
                    }
                    else
                    {
                        if (_downbright)
                        {
                            _down->setBrightStyle(Widget::BrightStyle::NORMAL);
                            _downbright = false;
                            if (_handler != nullptr)
                                _handler(_objdown, GameControllerDef::EVENT::UP, 0.0f);
                        }
                    }

                    // left
                    if (valx < -cos_67_5)
                    {
                        if (!_leftbright)
                        {
                            _left->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);
                            _leftbright = true;
                            if (_handler != nullptr)
                                _handler(_objleft, GameControllerDef::EVENT::DOWN, 1.0f);
                        }
                    }
                    else
                    {
                        if (_leftbright)
                        {
                            _left->setBrightStyle(Widget::BrightStyle::NORMAL);
                            _leftbright = false;
                            if (_handler != nullptr)
                                _handler(_objleft, GameControllerDef::EVENT::UP, 0.0f);
                        }
                    }

                    // right
                    if (valx > cos_67_5)
                    {
                        if (!_rightbright)
                        {
                            _right->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);
                            _rightbright = true;
                            if (_handler != nullptr)
                                _handler(_objright, GameControllerDef::EVENT::DOWN, 1.0f);
                        }
                    }
                    else
                    {
                        if (_rightbright)
                        {
                            _right->setBrightStyle(Widget::BrightStyle::NORMAL);
                            _rightbright = false;
                            if (_handler != nullptr)
                                _handler(_objright, GameControllerDef::EVENT::UP, 0.0f);
                        }
                    }
                }
                else
                {
                    // set effect off
                    _effect->setBrightStyle(Widget::BrightStyle::NORMAL);
                    _effect->setRotation(0.0f);

                    // up
                    if (_upbright)
                    {
                        _up->setBrightStyle(Widget::BrightStyle::NORMAL);
                        _upbright = false;
                        if (_handler != nullptr)
                            _handler(_objup, GameControllerDef::EVENT::UP, 0.0f);
                    }

                    // down
                    if (_downbright)
                    {
                        _down->setBrightStyle(Widget::BrightStyle::NORMAL);
                        _downbright = false;
                        if (_handler != nullptr)
                            _handler(_objdown, GameControllerDef::EVENT::UP, 0.0f);
                    }

                    // left
                    if (_leftbright)
                    {
                        _left->setBrightStyle(Widget::BrightStyle::NORMAL);
                        _leftbright = false;
                        if (_handler != nullptr)
                            _handler(_objleft, GameControllerDef::EVENT::UP, 0.0f);
                    }

                    // right
                    if (_rightbright)
                    {
                        _right->setBrightStyle(Widget::BrightStyle::NORMAL);
                        _rightbright = false;
                        if (_handler != nullptr)
                            _handler(_objright, GameControllerDef::EVENT::UP, 0.0f);
                    }
                }
            }
            else if (type == Widget::TouchEventType::ENDED || type == Widget::TouchEventType::CANCELED)
            {
                // set effect off
                _effect->setBrightStyle(Widget::BrightStyle::NORMAL);
                _effect->setRotation(0.0f);

                // up
                if (_upbright)
                {
                    _up->setBrightStyle(Widget::BrightStyle::NORMAL);
                    _upbright = false;
                    if (_handler != nullptr)
                        _handler(_objup, GameControllerDef::EVENT::UP, 0.0f);
                }

                // down
                if (_downbright)
                {
                    _down->setBrightStyle(Widget::BrightStyle::NORMAL);
                    _downbright = false;
                    if (_handler != nullptr)
                        _handler(_objdown, GameControllerDef::EVENT::UP, 0.0f);
                }

                // left
                if (_leftbright)
                {
                    _left->setBrightStyle(Widget::BrightStyle::NORMAL);
                    _leftbright = false;
                    if (_handler != nullptr)
                        _handler(_objleft, GameControllerDef::EVENT::UP, 0.0f);
                }

                // right
                if (_rightbright)
                {
                    _right->setBrightStyle(Widget::BrightStyle::NORMAL);
                    _rightbright = false;
                    if (_handler != nullptr)
                        _handler(_objright, GameControllerDef::EVENT::UP, 0.0f);
                }
            }
            else
            {
                Size size = this->getContentSize();
                Vec2 anchor = this->getPosition();
                Vec2 movepos = this->getTouchMovePosition();
                movepos.x -= anchor.x;
                movepos.y -= anchor.y;

//#ifdef VGC_SCENE_ROTATION_LEFT
//				if (mSceneRotation == VGC_SCENE_ROTATION_RIGHT)
//				{
//					movepos.x = -movepos.x;
//					movepos.y = -movepos.y;
//				}
//#endif
                float r = std::sqrt(movepos.x * movepos.x + movepos.y * movepos.y);
                if (r > 0.0f)
                {
                    if (movepos.y > 0.0f)
                        _effect->setRotation(360 - acos(movepos.x / r) * 180 / PI);
                    else
                        _effect->setRotation(acos(movepos.x / r) * 180 / PI);
                }

                float valx = 0.0f;
                float valy = 0.0f;
                if (r > _blind * sqrt(size.width * 0.5f * size.width * 0.5f + size.height * 0.5f * size.height * 0.5f))
                {
                    // set effect on
                    _effect->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);

                    valx = movepos.x / r;
					valy = movepos.y / r;

                    // up
                    if (valy > sin_22_5)
                    {
                        _up->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);
                        if (!_upbright)
                        {
                            _upbright = true;
                            if (_handler != nullptr)
                                _handler(_objup, GameControllerDef::EVENT::DOWN, 1.0f);
                        }
                    }
                    else
                    {
                        if (_upbright)
                        {
                            _up->setBrightStyle(Widget::BrightStyle::NORMAL);
                            _upbright = false;
                            if (_handler != nullptr)
                                _handler(_objup, GameControllerDef::EVENT::UP, 0.0f);
                        }
                    }

                    // down
                    if (valy < -sin_22_5)
                    {
                        _down->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);
                        if (!_downbright)
                        {
                            _downbright = true;
                            if (_handler != nullptr)
                                _handler(_objdown, GameControllerDef::EVENT::DOWN, 1.0f);
                        }
                    }
                    else
                    {
                        if (_downbright)
                        {
                            _down->setBrightStyle(Widget::BrightStyle::NORMAL);
                            _downbright = false;
                            if (_handler != nullptr)
                                _handler(_objdown, GameControllerDef::EVENT::UP, 0.0f);
                        }
                    }

                    // left
                    if (valx < -cos_67_5)
                    {
                        _left->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);
                        if (!_leftbright)
                        {
                            _leftbright = true;
                            if (_handler != nullptr)
                                _handler(_objleft, GameControllerDef::EVENT::DOWN, 1.0f);
                        }
                    }
                    else
                    {
                        if (_leftbright)
                        {
                            _left->setBrightStyle(Widget::BrightStyle::NORMAL);
                            _leftbright = false;
                            if (_handler != nullptr)
                                _handler(_objleft, GameControllerDef::EVENT::UP, 0.0f);
                        }
                    }

                    // right
                    if (valx > cos_67_5)
                    {
                        _right->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);
                        if (!_rightbright)
                        {
                            _rightbright = true;
                            if (_handler != nullptr)
                                _handler(_objright, GameControllerDef::EVENT::DOWN, 1.0f);
                        }
                    }
                    else
                    {
                        if (_rightbright)
                        {
                            _right->setBrightStyle(Widget::BrightStyle::NORMAL);
                            _rightbright = false;
                            if (_handler != nullptr)
                                _handler(_objright, GameControllerDef::EVENT::UP, 0.0f);
                        }
                    }
                }
                else
                {
                    // set effect on
                    _effect->setBrightStyle(Widget::BrightStyle::NORMAL);
                    _effect->setRotation(0.0f);

                    // up
                    if (_upbright)
                    {
                        _up->setBrightStyle(Widget::BrightStyle::NORMAL);
                        _upbright = false;
                        if (_handler != nullptr)
                            _handler(_objup, GameControllerDef::EVENT::UP, 0.0f);
                    }

                    // down
                    if (_downbright)
                    {
                        _down->setBrightStyle(Widget::BrightStyle::NORMAL);
                        _downbright = false;
                        if (_handler != nullptr)
                            _handler(_objdown, GameControllerDef::EVENT::UP, 0.0f);
                    }

                    // left
                    if (_leftbright)
                    {
                        _left->setBrightStyle(Widget::BrightStyle::NORMAL);
                        _leftbright = false;
                        if (_handler != nullptr)
                            _handler(_objleft, GameControllerDef::EVENT::UP, 0.0f);
                    }

                    // right
                    if (_rightbright)
                    {
                        _right->setBrightStyle(Widget::BrightStyle::NORMAL);
                        _rightbright = false;
                        if (_handler != nullptr)
                            _handler(_objright, GameControllerDef::EVENT::UP, 0.0f);
                    }
                }
            }
        });

        return true;
    }

    void GameControllerDpad::onEnter()
    {
        ImageView::onEnter();

        setTouchEnabled(true);
    }

    void GameControllerDpad::onExit()
    {
        setTouchEnabled(false);
        _effect->setBrightStyle(Widget::BrightStyle::NORMAL);
        _effect->setRotation(0.0f);
        _up->setBrightStyle(Widget::BrightStyle::NORMAL);
        _down->setBrightStyle(Widget::BrightStyle::NORMAL);
        _left->setBrightStyle(Widget::BrightStyle::NORMAL);
        _right->setBrightStyle(Widget::BrightStyle::NORMAL);

        ImageView::onExit();
    }

    void GameControllerDpad::setObject(GameControllerDef::OBJECT objup, GameControllerDef::OBJECT objdown, GameControllerDef::OBJECT objleft, GameControllerDef::OBJECT objright)
    {
        _objup = objup;
        _objdown = objdown;
        _objleft = objleft;
        _objright = objright;
    }

    void GameControllerDpad::loadTextures(const std::string & bg, const std::string & normal, const std::string & press, \
                                          const std::string & upnormal, const std::string & uppress, const std::string & downnormal, const std::string & downpress, \
                                          const std::string & leftnormal, const std::string & leftpress, const std::string & rightnormal, const std::string & rightpress)
    {
        // load bg
        ImageView::loadTexture(bg);

        // load center
        _effect->setContentSize(cocos2d::Size(_contentSize.width, _contentSize.height));
        _effect->loadTextures(normal, press);
        _effect->setPosition(Vec2(_contentSize.width / 2, _contentSize.height / 2));

        // load up
        _up->setContentSize(cocos2d::Size(_contentSize.width / 3, _contentSize.height / 3));
        _up->loadTextures(upnormal, uppress);
        _up->setPosition(Vec2(_contentSize.width / 2, _contentSize.height * 5 / 6));

        // load down
        _down->setContentSize(cocos2d::Size(_contentSize.width / 3, _contentSize.height / 3));
        _down->loadTextures(downnormal, downpress);
        _down->setPosition(Vec2(_contentSize.width / 2, _contentSize.height / 6));

        // load left
        _left->setContentSize(cocos2d::Size(_contentSize.width / 3, _contentSize.height / 3));
        _left->loadTextures(leftnormal, leftpress);
        _left->setPosition(Vec2(_contentSize.width / 6, _contentSize.height / 2));

        // load right
        _right->setContentSize(cocos2d::Size(_contentSize.width / 3, _contentSize.height / 3));
        _right->loadTextures(rightnormal, rightpress);
        _right->setPosition(Vec2(_contentSize.width * 5 / 6, _contentSize.height / 2));
    }

    void GameControllerDpad::addDpadEventListener(std::function<void(GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> handler)
    {
        _handler = handler;
    }

};

NS_CC_END
