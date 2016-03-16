/****************************************************************************
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "UIPageViewStandard.h"

NS_CC_BEGIN
namespace ui {

	IMPLEMENT_CLASS_GUI_INFO(PageViewStandard)

		PageViewStandard::PageViewStandard() :
		_isAutoScrolling(false),
		_autoScrollDistance(0.0f),
		_autoScrollSpeed(0.0f),
		_autoScrollDirection(AutoScrollDirection::LEFT),
		_curPageIdx(0),
		_touchMoveDirection(TouchDirection::LEFT),
		_leftBoundaryChild(nullptr),
		_rightBoundaryChild(nullptr),
		_leftBoundary(0.0f),
		_rightBoundary(0.0f),
		_customScrollThreshold(0.0),
		_usingCustomScrollThreshold(false),
		_childFocusCancelOffset(5.0f),
		_pageViewEventListener(nullptr),
		_pageViewEventSelector(nullptr),
		_eventCallback(nullptr),
		_touchListenerDirection(TouchListenerDirection::HORIZONTAL)
	{
		this->setTouchEnabled(true);
	}

	PageViewStandard::~PageViewStandard()
	{
		_pageViewEventListener = nullptr;
		_pageViewEventSelector = nullptr;
	}

	PageViewStandard* PageViewStandard::create()
	{
		PageViewStandard* widget = new (std::nothrow) PageViewStandard();
		if (widget && widget->init())
		{
			widget->autorelease();
			return widget;
		}
		CC_SAFE_DELETE(widget);
		return nullptr;
	}

	void PageViewStandard::onEnter()
	{
#if CC_ENABLE_SCRIPT_BINDING
		if (_scriptType == kScriptTypeJavascript)
		{
			if (ScriptEngineManager::sendNodeEventToJSExtended(this, kNodeOnEnter))
				return;
		}
#endif

		Layout::onEnter();
		scheduleUpdate();
	}

	bool PageViewStandard::init()
	{
		if (Layout::init())
		{
			setClippingEnabled(true);
			return true;
		}
		return false;
	}

	void PageViewStandard::addWidgetToPage(Widget *widget, ssize_t pageIdx, bool forceCreate)
	{
		if (!widget || pageIdx < 0)
		{
			return;
		}

		ssize_t pageCount = this->getPageCount();
		if (pageIdx < 0 || pageIdx >= pageCount)
		{
			if (forceCreate)
			{
				if (pageIdx > pageCount)
				{
					CCLOG("pageIdx is %d, it will be added as page id [%d]", static_cast<int>(pageIdx), static_cast<int>(pageCount));
				}
				Layout* newPage = createPage();
				newPage->addChild(widget);
				addPage(newPage);
			}
		}
		else
		{
			Node * page = _pages.at(pageIdx);
			page->addChild(widget);
		}
	}

	Layout* PageViewStandard::createPage()
	{
		Layout* newPage = Layout::create();
		newPage->setContentSize(getContentSize());
		return newPage;
	}

	void PageViewStandard::addPage(Layout* page)
	{
		if (!page || _pages.contains(page))
		{
			return;
		}


		addChild(page);
		_pages.pushBack(page);

		_doLayoutDirty = true;
	}

	void PageViewStandard::insertPage(Layout* page, int idx)
	{
		if (idx < 0 || !page || _pages.contains(page))
		{
			return;
		}


		ssize_t pageCount = this->getPageCount();
		if (idx >= pageCount)
		{
			addPage(page);
		}
		else
		{
			_pages.insert(idx, page);
			addChild(page);

		}

		_doLayoutDirty = true;
	}

	void PageViewStandard::removePage(Layout* page)
	{
		if (!page)
		{
			return;
		}
		removeChild(page);
		_pages.eraseObject(page);

		_doLayoutDirty = true;
	}

	void PageViewStandard::removePageAtIndex(ssize_t index)
	{
		if (index < 0 || index >= this->getPages().size())
		{
			return;
		}
		Layout* page = _pages.at(index);
		removePage(page);
	}

	void PageViewStandard::removeAllPages()
	{
		for (const auto& node : _pages)
		{
			removeChild(node);
		}
		_pages.clear();
	}

	void PageViewStandard::updateBoundaryPages()
	{
		if (_pages.size() <= 0)
		{
			_leftBoundaryChild = nullptr;
			_rightBoundaryChild = nullptr;
			return;
		}
		_leftBoundaryChild = _pages.at(0);
		_rightBoundaryChild = _pages.at(this->getPageCount() - 1);
	}

	ssize_t PageViewStandard::getPageCount()const
	{
		return _pages.size();
	}

	float PageViewStandard::getPositionXByIndex(ssize_t idx)const
	{
		return (getContentSize().width * (idx - _curPageIdx));
	}

	void PageViewStandard::onSizeChanged()
	{
		Layout::onSizeChanged();
		_rightBoundary = getContentSize().width;

		_doLayoutDirty = true;
	}

	void PageViewStandard::updateAllPagesSize()
	{
		Size selfSize = getContentSize();
		for (auto& page : _pages)
		{
			page->setContentSize(selfSize);
		}
	}

	void PageViewStandard::updateAllPagesPosition()
	{
		ssize_t pageCount = this->getPageCount();

		if (pageCount <= 0)
		{
			_curPageIdx = 0;
			return;
		}

		if (_curPageIdx >= pageCount)
		{
			_curPageIdx = pageCount - 1;
		}

		float pageWidth = getContentSize().width;
		for (int i = 0; i<pageCount; i++)
		{
			Layout* page = _pages.at(i);
			page->setPosition(Vec2((i - _curPageIdx) * pageWidth, 0));

		}
	}


	void PageViewStandard::scrollToPage(ssize_t idx)
	{
		if (idx < 0 || idx >= this->getPageCount())
		{
			return;
		}
		_curPageIdx = idx;
		Layout* curPage = _pages.at(idx);
		_autoScrollDistance = -(curPage->getPosition().x);
		_autoScrollSpeed = fabs(_autoScrollDistance) / 0.2f;
		_autoScrollDirection = _autoScrollDistance > 0 ? AutoScrollDirection::RIGHT : AutoScrollDirection::LEFT;
		_isAutoScrolling = true;
	}

	void PageViewStandard::update(float dt)
	{
		if (_isAutoScrolling)
		{
			this->autoScroll(dt);
		}
	}

	void PageViewStandard::autoScroll(float dt)
	{
		switch (_autoScrollDirection)
		{
		case AutoScrollDirection::LEFT:
		{
			float step = _autoScrollSpeed*dt;
			if (_autoScrollDistance + step >= 0.0f)
			{
				step = -_autoScrollDistance;
				_autoScrollDistance = 0.0f;
				_isAutoScrolling = false;
			}
			else
			{
				_autoScrollDistance += step;
			}
			scrollPages(-step);
			if (!_isAutoScrolling)
			{
				pageTurningEvent();
			}
			break;
		}
			break;
		case AutoScrollDirection::RIGHT:
		{
			float step = _autoScrollSpeed*dt;
			if (_autoScrollDistance - step <= 0.0f)
			{
				step = _autoScrollDistance;
				_autoScrollDistance = 0.0f;
				_isAutoScrolling = false;
			}
			else
			{
				_autoScrollDistance -= step;
			}
			scrollPages(step);

			if (!_isAutoScrolling)
			{
				pageTurningEvent();
			}

			break;
		}
		default:
			break;
		}

	}

	bool PageViewStandard::onTouchBegan(Touch *touch, Event *unusedEvent)
	{
		bool pass = Layout::onTouchBegan(touch, unusedEvent);
		return pass;
	}

	void PageViewStandard::onTouchMoved(Touch *touch, Event *unusedEvent)
	{
		Layout::onTouchMoved(touch, unusedEvent);
		if (!_isInterceptTouch)
		{
			handleMoveLogic(touch);
		}
	}

	void PageViewStandard::onTouchEnded(Touch *touch, Event *unusedEvent)
	{
		Layout::onTouchEnded(touch, unusedEvent);
		if (!_isInterceptTouch)
		{
			handleReleaseLogic(touch);
		}
		_isInterceptTouch = false;
	}

	void PageViewStandard::onTouchCancelled(Touch *touch, Event *unusedEvent)
	{
		Layout::onTouchCancelled(touch, unusedEvent);
		if (!_isInterceptTouch)
		{
			handleReleaseLogic(touch);
		}
		_isInterceptTouch = false;
	}

	void PageViewStandard::doLayout()
	{
		if (!_doLayoutDirty)
		{
			return;
		}

		updateAllPagesPosition();
		updateAllPagesSize();
		updateBoundaryPages();


		_doLayoutDirty = false;
	}

	void PageViewStandard::movePages(float offset)
	{
		for (auto& page : this->getPages())
		{
			page->setPosition(Vec2(page->getPosition().x + offset,
				page->getPosition().y));
		}
	}

	bool PageViewStandard::scrollPages(float touchOffset)
	{
		if (this->getPageCount() <= 0)
		{
			return false;
		}

		if (!_leftBoundaryChild || !_rightBoundaryChild)
		{
			return false;
		}

		float realOffset = touchOffset;

		switch (_touchMoveDirection)
		{
		case TouchDirection::LEFT: // left

			if (_rightBoundaryChild->getRightBoundary() + touchOffset <= _rightBoundary)
			{
				realOffset = _rightBoundary - _rightBoundaryChild->getRightBoundary();
				movePages(realOffset);
				return false;
			}
			break;

		case TouchDirection::RIGHT: // right

			if (_leftBoundaryChild->getLeftBoundary() + touchOffset >= _leftBoundary)
			{
				realOffset = _leftBoundary - _leftBoundaryChild->getLeftBoundary();
				movePages(realOffset);
				return false;
			}
			break;
		default:
			break;
		}

		movePages(realOffset);
		return true;
	}


	void PageViewStandard::handleMoveLogic(Touch *touch)
	{
		Vec2 touchPoint = touch->getLocation();
		float offset = 0.0;
		if (TouchListenerDirection::HORIZONTAL == _touchListenerDirection)
		{
			offset = touchPoint.x - touch->getPreviousLocation().x;
		}
		else
		{
			offset = touchPoint.y - touch->getPreviousLocation().y;
		}

		if (offset < 0)
		{
			_touchMoveDirection = TouchDirection::LEFT;
		}
		else if (offset > 0)
		{
			_touchMoveDirection = TouchDirection::RIGHT;
		}
		scrollPages(offset);
	}

	void PageViewStandard::setCustomScrollThreshold(float threshold)
	{
		CCASSERT(threshold > 0, "Invalid threshold!");
		_customScrollThreshold = threshold;
		this->setUsingCustomScrollThreshold(true);
	}

	float PageViewStandard::getCustomScrollThreshold()const
	{
		return _customScrollThreshold;
	}

	void PageViewStandard::setUsingCustomScrollThreshold(bool flag)
	{
		_usingCustomScrollThreshold = flag;
	}

	bool PageViewStandard::isUsingCustomScrollThreshold()const
	{
		return _usingCustomScrollThreshold;
	}

	void PageViewStandard::setTouchListenerDirection(TouchListenerDirection dir)
	{
		_touchListenerDirection = dir;
	}
	PageViewStandard::TouchListenerDirection PageViewStandard::getTouchListenerDirection()
	{
		return _touchListenerDirection;
	}
	void PageViewStandard::handleReleaseLogic(Touch *touch)
	{
		if (this->getPageCount() <= 0)
		{
			return;
		}
		Widget* curPage = dynamic_cast<Widget*>(this->getPages().at(_curPageIdx));
		if (curPage)
		{
			Vec2 curPagePos = curPage->getPosition();
			ssize_t pageCount = this->getPageCount();
			float curPageLocation = 0.0f;
			curPageLocation = curPagePos.x;
			float pageWidth = getContentSize().width;
			if (!_usingCustomScrollThreshold) {
				_customScrollThreshold = pageWidth / 2.0;
			}
			float boundary = _customScrollThreshold;
			if (curPageLocation <= -boundary)
			{
				if (_curPageIdx >= pageCount - 1)
				{
					scrollPages(-curPageLocation);
				}
				else
				{
					scrollToPage(_curPageIdx + 1);
				}
			}
			else if (curPageLocation >= boundary)
			{
				if (_curPageIdx <= 0)
				{
					scrollPages(-curPageLocation);
				}
				else
				{
					scrollToPage(_curPageIdx - 1);
				}
			}
			else
			{
				scrollToPage(_curPageIdx);
			}
		}
	}


	void PageViewStandard::interceptTouchEvent(TouchEventType event, Widget *sender, Touch *touch)
	{
		Vec2 touchPoint = touch->getLocation();

		switch (event)
		{
		case TouchEventType::BEGAN:
		{
			_touchBeganPosition = touch->getLocation();
			_isInterceptTouch = true;
		}
			break;
		case TouchEventType::MOVED:
		{
			float offset = 0;
			if (TouchListenerDirection::HORIZONTAL == _touchListenerDirection)
			{
				offset = fabs(sender->getTouchBeganPosition().x - touchPoint.x);
			}
			else
			{
				offset = fabs(sender->getTouchBeganPosition().y - touchPoint.y);
			}
			_touchMovePosition = touch->getLocation();
			if (offset > _childFocusCancelOffset)
			{
				sender->setHighlighted(false);
				handleMoveLogic(touch);
			}
		}
			break;
		case TouchEventType::CANCELED:
		case TouchEventType::ENDED:
		{
			_touchEndPosition = touch->getLocation();
			handleReleaseLogic(touch);
			if (sender->isSwallowTouches())
			{
				_isInterceptTouch = false;
			}
		}
			break;
		}
	}

	void PageViewStandard::pageTurningEvent()
	{
		this->retain();
		if (_pageViewEventListener && _pageViewEventSelector)
		{
			(_pageViewEventListener->*_pageViewEventSelector)(this, PAGEVIEWS_STANDARD_EVENT_TURNING);
		}
		if (_eventCallback) {
			_eventCallback(this, EventType::TURNING);
		}
		if (_ccEventCallback)
		{
			_ccEventCallback(this, static_cast<int>(EventType::TURNING));
		}
		this->release();
	}

	void PageViewStandard::addEventListenerPageView(Ref *target, SEL_PageViewStandardEvent selector)
	{
		_pageViewEventListener = target;
		_pageViewEventSelector = selector;
	}

	void PageViewStandard::addEventListener(const ccPageViewCallback& callback)
	{
		_eventCallback = callback;
	}

	ssize_t PageViewStandard::getCurPageIndex() const
	{
		return _curPageIdx;
	}

	Vector<Layout*>& PageViewStandard::getPages()
	{
		return _pages;
	}

	Layout* PageViewStandard::getPage(ssize_t index)
	{
		if (index < 0 || index >= this->getPages().size())
		{
			return nullptr;
		}
		return _pages.at(index);
	}

	std::string PageViewStandard::getDescription() const
	{
		return "PageViewStandard";
	}

	Widget* PageViewStandard::createCloneInstance()
	{
		return PageViewStandard::create();
	}

	void PageViewStandard::copyClonedWidgetChildren(Widget* model)
	{
		auto modelPages = static_cast<PageViewStandard*>(model)->getPages();
		for (auto& page : modelPages)
		{
			addPage(static_cast<Layout*>(page->clone()));
		}
	}

	void PageViewStandard::copySpecialProperties(Widget *widget)
	{
		PageViewStandard* pageView = dynamic_cast<PageViewStandard*>(widget);
		if (pageView)
		{
			Layout::copySpecialProperties(widget);
			_eventCallback = pageView->_eventCallback;
			_ccEventCallback = pageView->_ccEventCallback;
			_pageViewEventListener = pageView->_pageViewEventListener;
			_pageViewEventSelector = pageView->_pageViewEventSelector;
			_usingCustomScrollThreshold = pageView->_usingCustomScrollThreshold;
			_customScrollThreshold = pageView->_customScrollThreshold;
		}
	}

}

NS_CC_END
