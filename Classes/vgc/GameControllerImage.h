#ifndef __GAMECONTROLLERIMAGE_H__
#define __GAMECONTROLLERIMAGE_H__

#include "GameControllerDef.h"

NS_CC_BEGIN

namespace ui
{
    class GameControllerImage : public ui::ImageView
    {
    public:
        virtual ~GameControllerImage();
        CREATE_FUNC(GameControllerImage);

    protected:
        GameControllerImage();

        virtual bool init();
        virtual void onEnter();
        virtual void onExit();

    private:
        GameControllerDef::TYPE _type = GameControllerDef::TYPE::IMAGE;
    };
}

NS_CC_END

#endif
