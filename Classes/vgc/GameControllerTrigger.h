#ifndef __GAMECONTROLLERTRIGGER_H__
#define __GAMECONTROLLERTRIGGER_H__

#include "GameControllerDef.h"

NS_CC_BEGIN

namespace ui
{

    class GameControllerTrigger : public ImageView
    {
    public:
        virtual ~GameControllerTrigger();
        CREATE_FUNC(GameControllerTrigger);

        void setObject(GameControllerDef::OBJECT object);
        void loadTextures(const std::string & bg, const std::string & normal, const std::string & press, const std::string & disable = "");
        void addTriggerEventListener(std::function<void(GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> handler);

    protected:
        GameControllerTrigger();
    
        virtual bool init();
        // virtual void onEnter();
        // virtual void onExit();

    private:
        GameControllerDef::TYPE _type = GameControllerDef::TYPE::TRIGGER;
        GameControllerDef::OBJECT _object = GameControllerDef::OBJECT::UNKNOWN;
        std::function<void(GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> _handler = nullptr;
        Button * _trigger = nullptr;
    };

}

NS_CC_END

#endif