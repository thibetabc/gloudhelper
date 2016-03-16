#include "VirtualGameController.h"
#include "json/json.h"

NS_CC_BEGIN

namespace ui
{
    VirtualGameController::VirtualGameController()
    {
		mButton = nullptr;
		mSceneRotation = 0;
    }

    VirtualGameController::~VirtualGameController()
    {
        onDisconnected(_deviceName, _deviceId);
        unloadConfig();
    }

    VirtualGameController * VirtualGameController::getInstance()
    {
        static VirtualGameController * gc = nullptr;

        if (gc == nullptr)
        {
            gc = VirtualGameController::create();
            if (gc != nullptr)
                gc->retain();
        }

        return gc;
    }

    bool VirtualGameController::init()
    {
        if (!ImageView::init())
            return false;

		setSwallowTouches(true);

        return true;
    }

    void VirtualGameController::onEnter()
    {
        ImageView::onEnter();
    }

    void VirtualGameController::onExit()
    {
        getEventDispatcher()->dispatchCustomEvent("virtualgamecontroller_close", nullptr);

        ImageView::onExit();
    }

    bool VirtualGameController::loadConfig(std::string json, int basew, int baseh)
    {
        std::lock_guard<std::mutex> lock(_mutex);
#ifdef VGC_SCENE_ROTATION_LEFT
		mSceneRotation = UserDefault::getInstance()->getIntegerForKey(SCENE_ROTATION_KEY, VGC_SCENE_ROTATION_LEFT);
#endif
        if (/*_tip != nullptr || */!_buttons.empty() || !_joysticks.empty() || !_dpads.empty() || !_triggers.empty() || !_images.empty() || !_switchs.empty() || \
            json.empty() || basew < 1 || baseh < 1)
            return false;

        Json::Reader reader;
        Json::Value root;
        if (!reader.parse(json, root))
            return false;
        _basew = basew;
        _baseh = baseh;

        char * type = nullptr;
        char * name = nullptr;
        char * tip = nullptr;
        char * bg = nullptr;
        char * normal = nullptr;
        char * press = nullptr;
        char * disable = nullptr;
        float x = 0;
        float y = 0;
        float w = 0;
        float h = 0;
        float up = 0;
        float down = 0;
        float left = 0;
        float right = 0;

        // parse vgc pos
        Json::Value jsonvgc = root["vgc"];
        bg = (char *)jsonvgc["bg"].asCString();
        x = atof(jsonvgc["x"].asCString()) * _basew;
        y = atof(jsonvgc["y"].asCString()) * _baseh;
        w = atof(jsonvgc["w"].asCString()) * _basew;
        h = atof(jsonvgc["h"].asCString()) * _baseh;
		Sprite *sp = Sprite::create(bg);
		if (sp)
		{
#ifdef VGC_SCENE_ROTATION_LEFT
			sp->setRotation(mSceneRotation);
#endif
			sp->setPosition(Vec2(_basew / 2, _baseh / 2));
			addChild(sp);
		}
		/*setContentSize(Size(w, h));
		loadTexture(bg);*/

        // parse vgc tip pos
        Json::Value jsontip = root["tip"];
        bg = (char *)jsontip["bg"].asCString();
        x = atof(jsontip["x"].asCString()) * _basew;
        y = atof(jsontip["h"].asCString()) * _baseh;
        w = atof(jsontip["w"].asCString()) * _basew;
        h = atof(jsontip["h"].asCString()) * _baseh;
        up = atoi(jsontip["bgup"].asCString());
        down = atoi(jsontip["bgdown"].asCString());
        left = atoi(jsontip["bgleft"].asCString());
        right = atoi(jsontip["bgright"].asCString());
        //_tip = ui::Text::create("", "", 20.0f);
        //_tip->ignoreContentAdaptWithSize(false);
        //_tip->setContentSize(Size(w, h));
        //addChild(_tip);
        //_tip->setPosition(Vec2(x, y));

        // parse vgc items
        Json::Value jsonitems = root["items"];
		for (unsigned int i = 0; i < jsonitems.size(); i++)
        {
            Json::Value jsonitem = jsonitems[i];
            type = jsonitem.isMember("type") ? (char *)jsonitem["type"].asCString() : (char *)"";
            name = jsonitem.isMember("name") ? (char *)jsonitem["name"].asCString() : (char *)"";
            tip = jsonitem.isMember("tip") ? (char *)jsonitem["tip"].asCString() : (char *)"";
            bg = jsonitem.isMember("bg") ? (char *)jsonitem["bg"].asCString() : (char *)"";
            normal = jsonitem.isMember("normal") ? (char *)jsonitem["normal"].asCString() : (char *)"";
            press = jsonitem.isMember("press") ? (char *)jsonitem["press"].asCString() : (char *)"";
            disable = jsonitem.isMember("disable") ? (char *)jsonitem["disable"].asCString() : (char *)"";
            x = atof(jsonitem["x"].asCString()) * _basew;
            y = atof(jsonitem["y"].asCString()) * _baseh;
            w = atof(jsonitem["w"].asCString()) * _basew;
            h = atof(jsonitem["h"].asCString()) * _baseh;

			if (strcmp(type, "button") == 0 || strcmp(type, "button_dpad") == 0)
			{
				GameControllerDef::OBJECT obj = GameControllerDef::OBJECT::UNKNOWN;
				std::string objname = name;
				if (objname == "close")
					obj = GameControllerDef::OBJECT::CLOSE;
				else if (objname == "select")
					obj = GameControllerDef::OBJECT::SELECT;
				else if (objname == "start")
					obj = GameControllerDef::OBJECT::START;
				else if (objname == "dpad_up")
					obj = GameControllerDef::OBJECT::DPAD_UP;
				else if (objname == "dpad_down")
					obj = GameControllerDef::OBJECT::DPAD_DOWN;
				else if (objname == "dpad_left")
					obj = GameControllerDef::OBJECT::DPAD_LEFT;
				else if (objname == "dpad_right")
					obj = GameControllerDef::OBJECT::DPAD_RIGHT;
				else if (objname == "a")
					obj = GameControllerDef::OBJECT::A;
				else if (objname == "b")
					obj = GameControllerDef::OBJECT::B;
				else if (objname == "x")
					obj = GameControllerDef::OBJECT::X;
				else if (objname == "y")
					obj = GameControllerDef::OBJECT::Y;
				else if (objname == "lb")
					obj = GameControllerDef::OBJECT::LB;
				else if (objname == "rb")
					obj = GameControllerDef::OBJECT::RB;
				else if (objname == "lthumb")
					obj = GameControllerDef::OBJECT::LTHUMB;
				else if (objname == "rthumb")
					obj = GameControllerDef::OBJECT::RTHUMB;
				else if (objname == "lt")
					obj = GameControllerDef::OBJECT::LT;
				else if (objname == "rt")
					obj = GameControllerDef::OBJECT::RT;
				else if (objname == "lx")
					obj = GameControllerDef::OBJECT::LX;
				else if (objname == "rx")
					obj = GameControllerDef::OBJECT::RX;
				else if (objname == "ly")
					obj = GameControllerDef::OBJECT::LY;
				else if (objname == "ry")
					obj = GameControllerDef::OBJECT::RY;
				else
					obj = GameControllerDef::OBJECT::UNKNOWN;

				GameControllerButton * button = GameControllerButton::create();
				button->setContentSize(Size(w, h));
				button->setObject(obj);
				button->loadTextures(normal, press, disable);
				button->addButtonEventListener([&](GameControllerDef::OBJECT obj, GameControllerDef::EVENT event, float val) {
					onButtonEvent(obj, event, val);
				});
#ifdef VGC_SCENE_ROTATION_LEFT
				if (mSceneRotation == VGC_SCENE_ROTATION_RIGHT)
				{
					if (strcmp(type, "button_dpad") == 0)
					{
						button->setRotation(-180);
					}
					else if (strcmp(type, "button") == 0)
					{
						button->setRotation(mSceneRotation);
					}
				}
				else
				{
					//VGC_SCENE_ROTATION_LEFT
					if (strcmp(type, "button_dpad") != 0)
					{
						button->setRotation(mSceneRotation);
					}
				}
#endif
                button->setPosition(Vec2(x, y));
                addChild(button);
                _buttons.push_back(button);


				//close button
				if (obj == GameControllerDef::OBJECT::CLOSE)
				{
					setButton(button);
				}
            }
            else if (strcmp(type, "joystick") == 0)
            {
                std::string effect = jsonitem.isMember("effect") ? (char *)jsonitem["effect"].asCString() : (char *)"";
                GameControllerDef::OBJECT objx = GameControllerDef::OBJECT::UNKNOWN;
                GameControllerDef::OBJECT objy = GameControllerDef::OBJECT::UNKNOWN;
                GameControllerDef::OBJECT objbtn = GameControllerDef::OBJECT::UNKNOWN;
                std::string objname = name;
                if (objname == "l")
                {
                    objx = GameControllerDef::OBJECT::LX;
                    objy = GameControllerDef::OBJECT::LY;
                    objbtn = GameControllerDef::OBJECT::LTHUMB;
                }
                else if (objname == "r")
                {
                    objx = GameControllerDef::OBJECT::RX;
                    objy = GameControllerDef::OBJECT::RY;
                    objbtn = GameControllerDef::OBJECT::RTHUMB;
                }

                GameControllerJoystick * joystick = GameControllerJoystick::create();
                joystick->setContentSize(Size(w, h));
                joystick->setObject(objx, objy, objbtn);
                joystick->loadTextures(bg, effect, normal, press, disable);
                joystick->addJoystickEventListener([&](GameControllerDef::OBJECT obj0, GameControllerDef::EVENT event0, float val0, \
                    GameControllerDef::OBJECT obj1, GameControllerDef::EVENT event1, float val1) {
                    onJoyStickEvent(obj0, event0, val0, obj1, event1, val1);
                });
                joystick->addButtonEventListener([&](GameControllerDef::OBJECT obj, GameControllerDef::EVENT event, float val) {
                    onButtonEvent(obj, event, val);
                });
				joystick->setPosition(Vec2(x, y));
                addChild(joystick);
                _joysticks.push_back(joystick);
            }
            else if (strcmp(type, "dpad") == 0)
            {
                std::string upnormal = jsonitem.isMember("up_normal") ? (char *)jsonitem["up_normal"].asCString() : (char *)"";
                std::string uppress = jsonitem.isMember("up_press") ? (char *)jsonitem["up_press"].asCString() : (char *)"";
                std::string downnormal = jsonitem.isMember("down_normal") ? (char *)jsonitem["down_normal"].asCString() : (char *)"";
                std::string downpress = jsonitem.isMember("down_press") ? (char *)jsonitem["down_press"].asCString() : (char *)"";
                std::string leftnormal = jsonitem.isMember("left_normal") ? (char *)jsonitem["left_normal"].asCString() : (char *)"";
                std::string leftpress = jsonitem.isMember("left_press") ? (char *)jsonitem["left_press"].asCString() : (char *)"";
                std::string rightnormal = jsonitem.isMember("right_normal") ? (char *)jsonitem["right_normal"].asCString() : (char *)"";
                std::string rightpress = jsonitem.isMember("right_press") ? (char *)jsonitem["right_press"].asCString() : (char *)"";
                GameControllerDef::OBJECT objup = GameControllerDef::OBJECT::UNKNOWN;
                GameControllerDef::OBJECT objdown = GameControllerDef::OBJECT::UNKNOWN;
                GameControllerDef::OBJECT objleft = GameControllerDef::OBJECT::UNKNOWN;
                GameControllerDef::OBJECT objright = GameControllerDef::OBJECT::UNKNOWN;
                std::string objname = name;
                if (objname == "direction")
				{
                    objup = GameControllerDef::OBJECT::DPAD_UP;
                    objdown = GameControllerDef::OBJECT::DPAD_DOWN;
                    objleft = GameControllerDef::OBJECT::DPAD_LEFT;
                    objright = GameControllerDef::OBJECT::DPAD_RIGHT;
                }
                else if (objname == "abxy")
                {
                    objup = GameControllerDef::OBJECT::A;
                    objdown = GameControllerDef::OBJECT::B;
                    objleft = GameControllerDef::OBJECT::X;
                    objright = GameControllerDef::OBJECT::Y;
                }
                GameControllerDpad * dpad = GameControllerDpad::create();
                dpad->setContentSize(Size(w, h));
                dpad->setObject(objup, objdown, objleft, objright);
                dpad->loadTextures(bg, normal, press, \
                                   upnormal, uppress, \
                                   downnormal, downpress, \
                                   leftnormal, leftpress, \
                                   rightnormal, rightpress);
                dpad->addDpadEventListener([&](GameControllerDef::OBJECT obj, GameControllerDef::EVENT event, float val) {
                    onDpadEvent(obj, event, val);
                });
				dpad->setPosition(Vec2(x, y));
               addChild(dpad);
                _dpads.push_back(dpad);
            }
            else if (strcmp(type, "trigger") == 0)
            {
                GameControllerDef::OBJECT obj = GameControllerDef::OBJECT::UNKNOWN;
                std::string objname = name;
                if (objname == "lt")
                    obj = GameControllerDef::OBJECT::LT;
                else if (objname == "rt")
                    obj = GameControllerDef::OBJECT::RT;

                GameControllerTrigger * trigger = GameControllerTrigger::create();
                trigger->setContentSize(Size(w, h));
                trigger->setObject(obj);
                trigger->loadTextures(normal, press, disable);
                trigger->addTriggerEventListener([&](GameControllerDef::OBJECT obj, GameControllerDef::EVENT event, float val) {
                    onTriggerEvent(obj, event, val);
                });
                addChild(trigger);
                trigger->setPosition(Vec2(x, y));

                _triggers.push_back(trigger);
            }
			else if (strcmp(type, "image") == 0)//dpad_center
			{
				GameControllerImage * image = GameControllerImage::create();
				image->setContentSize(Size(w, h));
				image->loadTexture(normal);
				image->setPosition(Vec2(x, y));
#ifdef VGC_SCENE_ROTATION_LEFT
				if (mSceneRotation == VGC_SCENE_ROTATION_RIGHT)
				{
					image->setRotation(-180);
				}
#endif
				addChild(image);
                _images.push_back(image);
            }
            else if (strcmp(type, "switch") == 0)
            {
                GameControllerDef::OBJECT obj = GameControllerDef::OBJECT::UNKNOWN;
                std::string objname = name;
                if (objname == "select")
                    obj = GameControllerDef::OBJECT::SELECT;
                else if (objname == "start")
                    obj = GameControllerDef::OBJECT::START;
                else if (objname == "dpad_up")
                    obj = GameControllerDef::OBJECT::DPAD_UP;
                else if (objname == "dpad_down")
                    obj = GameControllerDef::OBJECT::DPAD_DOWN;
                else if (objname == "dpad_left")
                    obj = GameControllerDef::OBJECT::DPAD_LEFT;
                else if (objname == "dpad_right")
                    obj = GameControllerDef::OBJECT::DPAD_RIGHT;
                else if (objname == "a")
                    obj = GameControllerDef::OBJECT::A;
                else if (objname == "b")
                    obj = GameControllerDef::OBJECT::B;
                else if (objname == "x")
                    obj = GameControllerDef::OBJECT::X;
                else if (objname == "y")
                    obj = GameControllerDef::OBJECT::Y;
                else if (objname == "lb")
                    obj = GameControllerDef::OBJECT::LB;
                else if (objname == "rb")
                    obj = GameControllerDef::OBJECT::RB;
                else if (objname == "lthumb")
                    obj = GameControllerDef::OBJECT::LTHUMB;
                else if (objname == "rthumb")
                    obj = GameControllerDef::OBJECT::RTHUMB;
                else if (objname == "lt")
                    obj = GameControllerDef::OBJECT::LT;
                else if (objname == "rt")
                    obj = GameControllerDef::OBJECT::RT;
                else if (objname == "lx")
                    obj = GameControllerDef::OBJECT::LX;
                else if (objname == "rx")
                    obj = GameControllerDef::OBJECT::RX;
                else if (objname == "ly")
                    obj = GameControllerDef::OBJECT::LY;
                else if (objname == "ry")
                    obj = GameControllerDef::OBJECT::RY;
                else
                    obj = GameControllerDef::OBJECT::UNKNOWN;

                GameControllerSwitch * switche = GameControllerSwitch::create();
                switche->setContentSize(Size(w, h));
                switche->setObject(obj);
                switche->loadTextures(normal, press, disable);
                switche->addSwitchEventListener([&](GameControllerDef::OBJECT obj, GameControllerDef::EVENT event, float val) {
                    onSwitchEvent(obj, event, val);
                });
                addChild(switche);
                switche->setPosition(Vec2(x, y));

                _switchs.push_back(switche);
            }
        }

        return true;
    }

    void VirtualGameController::unloadConfig()
    {
        std::lock_guard<std::mutex> lock(_mutex);

        while (!_buttons.empty())
        {
            GameControllerButton * button = _buttons.at(0);
            _buttons.erase(_buttons.begin());
            button->removeFromParent();
            //button->release();
        }

        while (!_joysticks.empty())
        {
            GameControllerJoystick * joystick = _joysticks.at(0);
            _joysticks.erase(_joysticks.begin());
            joystick->removeFromParent();
            //joystick->release();
        }

        while (!_dpads.empty())
        {
            GameControllerDpad * dpad = _dpads.at(0);
            _dpads.erase(_dpads.begin());
            dpad->removeFromParent();
            //dpad->release();
        }

        while (!_triggers.empty())
        {
            GameControllerTrigger * trigger = _triggers.at(0);
            _triggers.erase(_triggers.begin());
            trigger->removeFromParent();
            //trigger->release();
        }

        while (!_images.empty())
        {
            GameControllerImage * image = _images.at(0);
            _images.erase(_images.begin());
            image->removeFromParent();
            //image->release();
        }

        while (!_switchs.empty())
        {
            GameControllerSwitch * switche = _switchs.at(0);
            _switchs.erase(_switchs.begin());
            switche->removeFromParent();
            //switche->release();
        }

        //if (_tip != nullptr)
        //{
        //    _tip->removeFromParent();
        //    _tip->release();
        //    _tip = nullptr;
        //}
    }

    std::vector<Controller*>::iterator VirtualGameController::findController(const std::string& deviceName, int deviceId)
    {
        auto iter = std::find_if(Controller::s_allController.begin(), Controller::s_allController.end(), [&](Controller* controller){
            return (deviceName == controller->_deviceName) && (deviceId == controller->_deviceId);
        });

        return iter;
    }

    void VirtualGameController::onConnected(const std::string& deviceName, int deviceId)
    {
        // Check whether the controller is already connected.
        CCLOG("onConnected %s,%d", _deviceName.c_str(), _deviceId);

        auto iter = findController(_deviceName,_deviceId);
        if (iter != Controller::s_allController.end())
            return;

        // It's a new controller being connected.
        auto controller = new cocos2d::Controller();
        controller->_deviceId = _deviceId;
        controller->_deviceName = _deviceName;
        Controller::s_allController.push_back(controller);

        controller->onConnected();
    }

    void VirtualGameController::onDisconnected(const std::string& deviceName, int deviceId)
    {
        CCLOG("onDisconnected %s,%d", _deviceName.c_str(), _deviceId);

        auto iter = findController(_deviceName, _deviceId);
        if (iter == Controller::s_allController.end())
        {
            CCLOGERROR("Could not find the controller!");
            return;
        }

        (*iter)->onDisconnected();
        Controller::s_allController.erase(iter);
    }

    void VirtualGameController::onButtonEvent(GameControllerDef::OBJECT obj, GameControllerDef::EVENT event, float val)
    {
        // CCLOG("onButtonEvent(%d, %d, %f)", obj, event, val);
        auto iter = findController(_deviceName, _deviceId);
        if (iter == Controller::s_allController.end())
        {
            CCLOG("onButtonEvent:connect new controller.");
            onConnected(_deviceName, _deviceId);
            iter = findController(_deviceName, _deviceId);
        }

        if (obj > GameControllerDef::OBJECT::UNKNOWN && obj < GameControllerDef::OBJECT::LT)
        {
            int keyCode = 0;
            switch (obj)
            {
                case GameControllerDef::OBJECT::CLOSE:
					keyCode = Controller::Key::BUTTON_PAUSE;
					break;
                case GameControllerDef::OBJECT::SELECT:
                    keyCode = Controller::Key::BUTTON_SELECT;
                    break;
                case GameControllerDef::OBJECT::START:
                    keyCode = Controller::Key::BUTTON_START;
                    break;
                case GameControllerDef::OBJECT::DPAD_UP:
                    keyCode = Controller::Key::BUTTON_DPAD_UP;
                    break;
                case GameControllerDef::OBJECT::DPAD_DOWN:
                    keyCode = Controller::Key::BUTTON_DPAD_DOWN;
                    break;
                case GameControllerDef::OBJECT::DPAD_LEFT:
                    keyCode = Controller::Key::BUTTON_DPAD_LEFT;
                    break;
                case GameControllerDef::OBJECT::DPAD_RIGHT:
                    keyCode = Controller::Key::BUTTON_DPAD_RIGHT;
                    break;
                case GameControllerDef::OBJECT::A:
                    keyCode = Controller::Key::BUTTON_A;
                    break;
                case GameControllerDef::OBJECT::B:
                    keyCode = Controller::Key::BUTTON_B;
                    break;
                case GameControllerDef::OBJECT::X:
                    keyCode = Controller::Key::BUTTON_X;
                    break;
                case GameControllerDef::OBJECT::Y:
                    keyCode = Controller::Key::BUTTON_Y;
                    break;
                case GameControllerDef::LB:
                    keyCode = Controller::Key::BUTTON_LEFT_SHOULDER;
                    break;
                case GameControllerDef::RB:
                    keyCode = Controller::Key::BUTTON_RIGHT_SHOULDER;
                    break;
                case GameControllerDef::LTHUMB:
                    keyCode = Controller::Key::BUTTON_LEFT_THUMBSTICK;
                    break;
                case GameControllerDef::RTHUMB:
                    keyCode = Controller::Key::BUTTON_RIGHT_THUMBSTICK;
                    break;
                default:
                    break;
            }
            if (keyCode == 0)
                return;

            (*iter)->onButtonEvent(keyCode, event == GameControllerDef::EVENT::DOWN ? true : false, val, false);
        }
        else if (obj >= GameControllerDef::OBJECT::LT && obj <= GameControllerDef::OBJECT::RT)
            onTriggerEvent(obj, event, val);
        else if (obj >= GameControllerDef::OBJECT::LX && obj <= GameControllerDef::OBJECT::RY)
            onJoyStickEvent(obj, event, val, GameControllerDef::OBJECT::UNKNOWN, GameControllerDef::EVENT::AXIS, 0.0f);
    }

    void VirtualGameController::onJoyStickEvent(GameControllerDef::OBJECT obj0, GameControllerDef::EVENT event0, float val0, GameControllerDef::OBJECT obj1, GameControllerDef::EVENT event1, float val1)
    {
        auto iter = findController(_deviceName, _deviceId);
        if (iter == Controller::s_allController.end())
        {
            CCLOG("onAxisEvent:connect new controller.");
            onConnected(_deviceName, _deviceId);
            iter = findController(_deviceName, _deviceId);
        }

        int axisCode = 0;
#ifdef VGC_SCENE_ROTATION_LEFT
			if (mSceneRotation == VGC_SCENE_ROTATION_LEFT)
			{
				val1 = -val1;
			}
			else if (mSceneRotation == VGC_SCENE_ROTATION_RIGHT)
			{
				val0 = -val0;
			}
			switch (obj0)
			{
			case GameControllerDef::OBJECT::LX:
				axisCode = Controller::Key::JOYSTICK_LEFT_Y; 
				break;
			case GameControllerDef::OBJECT::LY:
				axisCode = Controller::Key::JOYSTICK_LEFT_X;
				break;
			case GameControllerDef::OBJECT::RX:
				axisCode = Controller::Key::JOYSTICK_RIGHT_Y;
				break;
			case GameControllerDef::OBJECT::RY:
				axisCode = Controller::Key::JOYSTICK_RIGHT_X;
				break;
			default:
				break;
			}


			if (axisCode != 0)
				(*iter)->onAxisEvent(axisCode, val0 * 32768, false);

			switch (obj1)
			{
			case GameControllerDef::OBJECT::LX:
				axisCode = Controller::Key::JOYSTICK_LEFT_Y;
				break;
			case GameControllerDef::OBJECT::LY:
				axisCode = Controller::Key::JOYSTICK_LEFT_X;
				break;
			case GameControllerDef::OBJECT::RX:
				axisCode = Controller::Key::JOYSTICK_RIGHT_Y;
				break;
			case GameControllerDef::OBJECT::RY:
				axisCode = Controller::Key::JOYSTICK_RIGHT_X;
				break;
			default:
				break;
			}

			if (axisCode != 0)
				(*iter)->onAxisEvent(axisCode, val1 * 32767, false);

#else
			switch (obj0)
			{
			case GameControllerDef::OBJECT::LX:
				axisCode = Controller::Key::JOYSTICK_LEFT_X;
				break;
			case GameControllerDef::OBJECT::LY:
				axisCode = Controller::Key::JOYSTICK_LEFT_Y;
				break;
			case GameControllerDef::OBJECT::RX:
				axisCode = Controller::Key::JOYSTICK_RIGHT_X;
				break;
			case GameControllerDef::OBJECT::RY:
				axisCode = Controller::Key::JOYSTICK_RIGHT_Y;
				break;
			default:
				break;
			}


			if (axisCode != 0)
				(*iter)->onAxisEvent(axisCode, val0 * 32768, false);

			switch (obj1)
			{
			case GameControllerDef::OBJECT::LX:
				axisCode = Controller::Key::JOYSTICK_LEFT_X;
				break;
			case GameControllerDef::OBJECT::LY:
				axisCode = Controller::Key::JOYSTICK_LEFT_Y;
				break;
			case GameControllerDef::OBJECT::RX:
				axisCode = Controller::Key::JOYSTICK_RIGHT_X;
				break;
			case GameControllerDef::OBJECT::RY:
				axisCode = Controller::Key::JOYSTICK_RIGHT_Y;
				break;
			default:
				break;
			}
			if (axisCode != 0)
				(*iter)->onAxisEvent(axisCode, val1 * 32767, false);
#endif
    }

    void VirtualGameController::onDpadEvent(GameControllerDef::OBJECT obj, GameControllerDef::EVENT event, float val)
    {
        // CCLOG("onButtonEvent(%d, %d, %f)", obj, event, val);		
#ifdef VGC_SCENE_ROTATION_LEFT
		if (mSceneRotation == VGC_SCENE_ROTATION_LEFT)
		{

			switch (obj)
			{
			case GameControllerDef::OBJECT::DPAD_UP:
				obj = GameControllerDef::OBJECT::DPAD_LEFT;
				break;
			case GameControllerDef::OBJECT::DPAD_DOWN:
				obj = GameControllerDef::OBJECT::DPAD_RIGHT;
				break;
			case GameControllerDef::OBJECT::DPAD_LEFT:
				obj = GameControllerDef::OBJECT::DPAD_DOWN;
				break;
			case GameControllerDef::OBJECT::DPAD_RIGHT:
				obj = GameControllerDef::OBJECT::DPAD_UP;
				break;
			default:
				break;
			}
		}
		else if (mSceneRotation == VGC_SCENE_ROTATION_RIGHT)
		{
			switch (obj)
			{
			case GameControllerDef::OBJECT::DPAD_UP:
				obj = GameControllerDef::OBJECT::DPAD_RIGHT;
				break;
			case GameControllerDef::OBJECT::DPAD_DOWN:
				obj = GameControllerDef::OBJECT::DPAD_LEFT;
				break;
			case GameControllerDef::OBJECT::DPAD_LEFT:
				obj = GameControllerDef::OBJECT::DPAD_UP;
				break;
			case GameControllerDef::OBJECT::DPAD_RIGHT:
				obj = GameControllerDef::OBJECT::DPAD_DOWN;
				break;
			default:
				break;
			}

		}
#endif
        onButtonEvent(obj, event, val);
    }

    void VirtualGameController::onTriggerEvent(GameControllerDef::OBJECT obj, GameControllerDef::EVENT event, float val)
    {
        auto iter = findController(_deviceName, _deviceId);
        if (iter == Controller::s_allController.end())
        {
            CCLOG("onAxisEvent:connect new controller.");
            onConnected(_deviceName, _deviceId);
            iter = findController(_deviceName, _deviceId);
        }

        int axisCode = 0;
        switch (obj)
        {
            case GameControllerDef::OBJECT::LT:
                axisCode = Controller::Key::AXIS_LEFT_TRIGGER;
                break;
            case GameControllerDef::OBJECT::RT:
                axisCode = Controller::Key::AXIS_RIGHT_TRIGGER;
                break;
            default:
                break;
        }
        if (axisCode == 0)
            return;

        (*iter)->onAxisEvent(axisCode, val * 32767, false);
    }

    void VirtualGameController::onSwitchEvent(GameControllerDef::OBJECT obj, GameControllerDef::EVENT event, float val)
    {
        if (obj > GameControllerDef::OBJECT::UNKNOWN && obj < GameControllerDef::OBJECT::LT)
            onButtonEvent(obj, event, val);
        else if (obj >= GameControllerDef::OBJECT::LT && obj <= GameControllerDef::OBJECT::RT)
            onTriggerEvent(obj, event, val);
        else if (obj >= GameControllerDef::OBJECT::LX && obj <= GameControllerDef::OBJECT::RY)
            onJoyStickEvent(obj, event, val, GameControllerDef::OBJECT::UNKNOWN, GameControllerDef::EVENT::AXIS, 0.0f);
    }

	void VirtualGameController::setButton(Button *button)
	{
		mButton = button;
	}
	Button *VirtualGameController::getButton()
	{
		return mButton;
	}

}

NS_CC_END
