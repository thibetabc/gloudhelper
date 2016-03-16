#ifndef __GAMECONTROLLERSWITCH_H__
#define __GAMECONTROLLERSWITCH_H__

#include "GameControllerDef.h"

NS_CC_BEGIN

namespace ui
{
    class GameControllerSwitch : public Button
    {
    public:
        virtual ~GameControllerSwitch();
        CREATE_FUNC(GameControllerSwitch);

        void setObject(GameControllerDef::OBJECT object);
        void loadTextures(const std::string & normal, const std::string & press, const std::string & disable = "");
        void addSwitchEventListener(std::function<void(GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> handler);

    protected:
        GameControllerSwitch();

        virtual bool init();
        virtual void onEnter();
        virtual void onExit();

    private:
        bool _on = false;
        GameControllerDef::TYPE _type = GameControllerDef::TYPE::SWITCH;
        GameControllerDef::OBJECT _object = GameControllerDef::OBJECT::UNKNOWN;
        std::function<void(GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> _handler = nullptr;
    };
}

NS_CC_END

#endif
