#ifndef __TEXTTIP_H__
#define __TEXTTIP_H__

#include "Global.h"

NS_CC_BEGIN

namespace ui
{

    class TextTip : public ui::Layout
    {
    public:
        ~TextTip();
        CREATE_FUNC(TextTip);

        virtual void setContentSize(const Size& contentSize);

        void setString(const std::string & text);
        const std::string & getString() const;
        ssize_t getStringLength() const;
        void setFontSize(int size);
        int getFontSize() const;
        void setFontName(const std::string & name);
        const std::string & getFontName() const;
        void setBackGroundImage(const std::string & img, const Rect capInsets);

    protected:
        TextTip();

        virtual bool init();
        virtual void onEnter();
        virtual void onExit();
        virtual void update(float value);

        void adjustContentSize();

    private:
        ui::ImageView * _bg = nullptr;
        ui::Text * _content = nullptr;
    };

};

NS_CC_END

#endif