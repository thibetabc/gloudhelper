#ifndef __GAMECONTROLLERDPAD_H__
#define __GAMECONTROLLERDPAD_H__

#include "GameControllerDef.h"

NS_CC_BEGIN

namespace ui
{

    class GameControllerDpad : public ImageView
    {
    public:
        virtual ~GameControllerDpad();
        CREATE_FUNC(GameControllerDpad);

        void setObject(GameControllerDef::OBJECT objup, GameControllerDef::OBJECT objdown, GameControllerDef::OBJECT objleft, GameControllerDef::OBJECT objright);
        void loadTextures(const std::string & bg, const std::string & normal, const std::string & press, \
                          const std::string & upnormal, const std::string & uppress, const std::string & downnormal, const std::string & downpress, \
                          const std::string & leftnormal, const std::string & leftpress, const std::string & rightnormal, const std::string & rightpress);
        void addDpadEventListener(std::function<void(GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> handler);

    protected:
        GameControllerDpad();

        virtual bool init();
        virtual void onEnter();
        virtual void onExit();

    private:
        float _blind = 0.2f;
        GameControllerDef::TYPE _type = GameControllerDef::TYPE::DPAD;
        GameControllerDef::OBJECT _objup = GameControllerDef::OBJECT::DPAD_UP;
        GameControllerDef::OBJECT _objdown = GameControllerDef::OBJECT::DPAD_DOWN;
        GameControllerDef::OBJECT _objleft = GameControllerDef::OBJECT::DPAD_LEFT;
        GameControllerDef::OBJECT _objright = GameControllerDef::OBJECT::DPAD_RIGHT;
        std::function<void(GameControllerDef::OBJECT, GameControllerDef::EVENT, float)> _handler = nullptr;
        Button * _effect = nullptr;
        Button * _up = nullptr;
        bool _upbright = false;
        Button * _down = nullptr;
        bool _downbright = false;
        Button * _left = nullptr;
        bool _leftbright = false;
        Button * _right = nullptr;
        bool _rightbright = false;
    };

};

NS_CC_END

#endif
