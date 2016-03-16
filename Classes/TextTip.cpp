#include "TextTip.h"

NS_CC_BEGIN

namespace ui
{

    TextTip::TextTip()
    {
    }

    TextTip::~TextTip()
    {
    }

    void TextTip::setContentSize(const Size & contentSize)
    {
    }

    void TextTip::setString(const std::string & text)
    {
        _content->setString(text);
        adjustContentSize();
        setOpacity(255);
        _bg->setOpacity(255);
        _content->setOpacity(255);

        runAction(Sequence::create(FadeIn::create(0.5f), FadeTo::create(2.0f, 255), FadeOut::create(1.0f), nullptr));
        _bg->runAction(Sequence::create(FadeIn::create(0.5f), FadeTo::create(2.0f, 255), FadeOut::create(1.0f), nullptr));
        _content->runAction(Sequence::create(FadeIn::create(0.5f), FadeTo::create(2.0f, 255), FadeOut::create(1.0f), nullptr));
    }

    const std::string & TextTip::getString() const
    {
        return _content->getString();
    }

    ssize_t TextTip::getStringLength() const
    {
        return _content->getStringLength();
    }

    void TextTip::setFontSize(int size)
    {
        _content->setFontSize(size);

        adjustContentSize();
    }

    int TextTip::getFontSize() const
    {
        return _content->getFontSize();
    }

    void TextTip::setFontName(const std::string & name)
    {
        _content->setFontName(name);
        adjustContentSize();
    }

    const std::string & TextTip::getFontName() const
    {
        return _content->getFontName();
    }

    void TextTip::setBackGroundImage(const std::string & img, const Rect capInsets)
    {
        _bg->loadTexture(img);
        _bg->setScale9Enabled(true);
        _bg->setCapInsets(capInsets);
        // adjustContentSize();
    }

    bool TextTip::init()
    {
        if (!Layout::init())
            return false;
        
        // set anchor point to center
        setAnchorPoint(Vec2(0.5f, 0.5f));

        // add bg image
        _bg = ImageView::create();
        _bg->setScale9Enabled(true);
        addChild(_bg);
        _bg->setPosition(Vec2(0.0f, 0.0f));
        
        // add text
        _content = Text::create();
        addChild(_content);
        _content->setPosition(Vec2(0.0f, 0.0f));

        return true;
    }

    void TextTip::onEnter()
    {
        Layout::onEnter();
    }

    void TextTip::onExit()
    {
        unscheduleAllCallbacks();
        unscheduleUpdate();

        Layout::onExit();
    }

    void TextTip::update(float value)
    {
        Layout::update(value);
    }

    void TextTip::adjustContentSize()
    {
        // get calculated content size
        Size size = _content->getContentSize();
        size.width += 20;
        size.height += 30;

        // updata content size
        setAnchorPoint(Vec2(0.5f, 0.5f));
        setContentSize(size);

        // update bg content size
        _bg->setContentSize(size);

        // update bg position
        _bg->setPosition(Vec2(0.0f, 0.0f));

        // update content position
        _content->setPosition(Vec2(0.0f, 0.0f));
    }

};

NS_CC_END
