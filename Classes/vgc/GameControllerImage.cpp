#include "GameControllerImage.h"

NS_CC_BEGIN

namespace ui
{
    GameControllerImage::GameControllerImage()
    {

    }

    GameControllerImage::~GameControllerImage()
    {

    }

    bool GameControllerImage::init()
    {
        if (!ImageView::init())
            return false;

        ignoreContentAdaptWithSize(false);
        setTouchEnabled(false);

        return true;
    }

    void GameControllerImage::onEnter()
    {
        ImageView::onEnter();

    }

    void GameControllerImage::onExit()
    {
        
        ImageView::onExit();
    }

};

NS_CC_END
