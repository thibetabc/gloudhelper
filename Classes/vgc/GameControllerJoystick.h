#ifndef __GAMECONTROLLERJOYSTICK_H__
#define __GAMECONTROLLERJOYSTICK_H__

#include "GameControllerDef.h"

NS_CC_BEGIN

namespace ui
{

    class GameControllerJoystick : public ImageView
    {
    public:
        virtual ~GameControllerJoystick();
        CREATE_FUNC(GameControllerJoystick);

        void setObject(GameControllerDef::OBJECT objx, GameControllerDef::OBJECT objy, GameControllerDef::OBJECT objbtn);
        void loadTextures(const std::string & bg, const std::string & effect, const std::string & normal, const std::string & press, const std::string & disable = "");
        void addJoystickEventListener(std::function<void (GameControllerDef::OBJECT, GameControllerDef::EVENT, float, GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> handler);
        void addButtonEventListener(std::function<void(GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> handler);

    protected:
        GameControllerJoystick();

        virtual bool init();
        virtual void onEnter();
        virtual void onExit();

        void btnTimer(float val);

    private:
        float _blind = 0.3f;
        GameControllerDef::TYPE _type = GameControllerDef::TYPE::JOYSTICK;
        GameControllerDef::OBJECT _objx = GameControllerDef::OBJECT::UNKNOWN;
        GameControllerDef::OBJECT _objy = GameControllerDef::OBJECT::UNKNOWN;
        GameControllerDef::OBJECT _objbtn = GameControllerDef::OBJECT::UNKNOWN;
        std::function<void (GameControllerDef::OBJECT, GameControllerDef::EVENT, float, GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> _handler = nullptr;
        std::function<void (GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> _btnhandler = nullptr;
        std::string _bg = "";
        std::string _effect = "";
        Button * _joystick = nullptr;
        bool _btndown = false;
    };

};

NS_CC_END

#endif
