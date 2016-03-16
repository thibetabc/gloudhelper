#ifndef __VIRTUALGAMECONTROLLER_H__
#define __VIRTUALGAMECONTROLLER_H__

#include "Global.h"
#include "GameControllerButton.h"
#include "GameControllerJoystick.h"
#include "GameControllerDpad.h"
#include "GameControllerTrigger.h"
#include "GameControllerImage.h"
#include "GameControllerSwitch.h"

NS_CC_BEGIN

namespace ui
{

    class VirtualGameController : public ImageView
    {
	public:
		VirtualGameController();
        ~VirtualGameController();
		CREATE_FUNC(VirtualGameController);
        static VirtualGameController * getInstance();

        bool loadConfig(std::string json, int basew, int baseh);
        void unloadConfig();

		//close button 
		void setButton(Button *button);
		Button *getButton();

    protected:
        virtual bool init();
        virtual void onEnter();
        virtual void onExit();

        static std::vector<Controller*>::iterator findController(const std::string& deviceName, int deviceId);
        void onConnected(const std::string& deviceName, int deviceId);
        void onDisconnected(const std::string& deviceName, int deviceId);
        void onButtonEvent(GameControllerDef::OBJECT obj, GameControllerDef::EVENT event, float val);
        void onJoyStickEvent(GameControllerDef::OBJECT obj0, GameControllerDef::EVENT event0, float val0, GameControllerDef::OBJECT obj1, GameControllerDef::EVENT event1, float val1);
        void onDpadEvent(GameControllerDef::OBJECT obj, GameControllerDef::EVENT event, float val);
        void onTriggerEvent(GameControllerDef::OBJECT obj, GameControllerDef::EVENT event, float val);
        void onSwitchEvent(GameControllerDef::OBJECT obj, GameControllerDef::EVENT event, float val);
        

    private:
        std::mutex _mutex;

        std::string _deviceName = "virtualgamecontroller";
        int _deviceId = 0;
        int _basew = 0; 
        int _baseh = 0;

        ui::Text * _tip = nullptr;
        std::vector<GameControllerButton *> _buttons;
        std::vector<GameControllerJoystick *> _joysticks;
        std::vector<GameControllerDpad *> _dpads;
        std::vector<GameControllerTrigger *> _triggers;
        std::vector<GameControllerImage *> _images;
        std::vector<GameControllerSwitch *> _switchs;
		Button *mButton;
		int mSceneRotation;
        
    };

};

NS_CC_END

#endif
