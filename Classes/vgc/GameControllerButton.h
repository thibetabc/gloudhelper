#ifndef __GAMECONTROLLERBUTTON_H__
#define __GAMECONTROLLERBUTTON_H__

#include "GameControllerDef.h"

NS_CC_BEGIN

namespace ui
{
    class GameControllerButton : public ui::Button
    {
    public:
        virtual ~GameControllerButton();
        CREATE_FUNC(GameControllerButton);

        void setObject(GameControllerDef::OBJECT object);
        void loadTextures(const std::string & normal, const std::string & press, const std::string & disable = "");
        void addButtonEventListener(std::function<void (GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> handler);

    protected:
        GameControllerButton();

        virtual bool init();
        virtual void onEnter();
        virtual void onExit();

    private:
        GameControllerDef::TYPE _type = GameControllerDef::TYPE::BUTTON;
        GameControllerDef::OBJECT _object = GameControllerDef::OBJECT::UNKNOWN;
        std::function<void (GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> _handler = nullptr;
    };
}

NS_CC_END

#endif
