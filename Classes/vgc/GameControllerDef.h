#ifndef __GAMECONTROLLERITEM_H__
#define __GAMECONTROLLERITEM_H__

#include "Global.h"
#include "cocos/base/CCController.h"

NS_CC_BEGIN

namespace ui
{

    class GameControllerDef
    {
    public:
        enum TYPE
        {
            BUTTON = 0, 
            JOYSTICK, 
            DPAD, 
            TRIGGER, 
            IMAGE, 
            SWITCH, 
        };

        enum OBJECT
        {
            UNKNOWN = 0, 

            // button
            CLOSE, 
            SELECT,
            START,
            DPAD_UP,
            DPAD_DOWN,
            DPAD_LEFT,
            DPAD_RIGHT,
            A,
            B,
            X,
            Y,
            LB, 
            RB,
            LTHUMB, 
            RTHUMB, 

            // trigger
            LT,
            RT,

            // axis
            LX,
            LY,
            RX,
            RY,
        };

        enum EVENT
        {
            DOWN = 0, 
            UP, 
            AXIS
        };
    };

}

NS_CC_END

#endif
