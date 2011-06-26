/*
 *  SoraMenuBar.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/19/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraMenuBar.h"
#include "SoraCore.h"

namespace sora {

	/*void SoraMenuBarClickEvent::setItemName(const std::string& name) {
		mItemName = name;
	}
	
	std::string SoraMenuBarClickEvent::getItemName() const {
		return mItemName;
	}*/
	
	SoraMenuBarItem* SoraMenuBarClickEvent::getItem() const {
		return (SoraMenuBarItem*)pEventSource;
	}
	
	SoraMenuBarMenu::SoraMenuBarMenu(const std::wstring& barName): 
	mFont(NULL), 
	mPosX(0.f), 
	mWidth(0.f), 
	mItemHeight(30.f), 
	mActive(false),
	mCurrentItem(-1) {
		mBarName = barName;
	}
	
	bool SoraMenuBarMenu::update() {
		if(mActive) {
			float32 x, y;
			SORA->getMousePos(&x, &y);
			mCurrentItem = -1;
			
			if(x >= mPosX && x <= mPosX+mWidth) {
				if(y >= mParent->getMenuBarHeight() && y <= mParent->getMenuBarHeight()*(mItems.size()+2)) {
					//y -= mParent->getMenuBarHeight();
					mCurrentItem = static_cast<int32>(y /  mParent->getMenuBarHeight()) - 1;
				}
				
				if(SORA->keyDown(SORA_KEY_LBUTTON)) {
					if(mCurrentItem != -1) {
						mItems[mCurrentItem].onClick();
						mParent->diactiveMenus();
					} else {
						mParent->diactiveMenus();
					}
				}
			}
		}
		return mActive;
	}
	
	std::wstring SoraMenuBarMenu::getName() const {
		return mBarName;
	}
	
	void SoraMenuBarMenu::render() {
		if(mFont) {
			float32 distHeight = (mParent->getMenuBarHeight()-mItemHeight)/2;
			float32 posy = mParent->getMenuBarHeight();

			SORA->renderBox(mPosX, 0.f, mPosX+mWidth, posy, mWidth, 0xFF000000);
			
			mFont->setColor(0xFF000000);
			if(mActive) {
				SORA->renderRect(mPosX, 0.f, mPosX+mWidth, posy, mWidth, 0x99FFFFFF);
			}
			mFont->render(mPosX+10.f, distHeight, mBarName.c_str(), false, true);
			
			if(mActive) {
				SORA->renderRect(mPosX, posy, mPosX+mWidth, posy*(mItems.size()+1), mWidth, 0x99FFFFFF);
				
				for(size_t i=0; i<mItems.size(); ++i) {
					if(mItems[i].isAvailable())
						mFont->setColor(0xFF000000);
					else
						mFont->setColor(0xFFDDDDDD);
					
					if(mCurrentItem == i && mItems[i].isAvailable()) {
						SORA->renderRect(mPosX, posy, mPosX+mWidth, posy+mParent->getMenuBarHeight(), mWidth, 0x99FFFFFF);
					}
					SORA->renderBox(mPosX, posy, mPosX+mWidth, posy+mParent->getMenuBarHeight(), 0xFF000000);
					
					mFont->render(mPosX+10.f, posy+distHeight, mItems[i].getName().c_str(), false, true);

					posy += mParent->getMenuBarHeight();
				}
			}
		}
	}
	
	void SoraMenuBarMenu::addItem(const std::wstring& name, SoraEventHandler* handler) {
		SoraMenuBarItem item(name);
		item.mEventHandler = handler;
		mItems.push_back(item);
	}
	
	SoraMenuBarMenu::MENU_ITEM_CONT::iterator SoraMenuBarMenu::getItemIterator(const std::wstring& name) {
		for(size_t i=0; i<mItems.size(); ++i) {
			if(mItems[i].getName().compare(name) == 0) {
				return mItems.begin()+i;
			}
		}
		return mItems.end();
	}
	
	void SoraMenuBarMenu::removeItem(const std::wstring& name) {
		MENU_ITEM_CONT::iterator itItem = getItemIterator(name);
		if(itItem != mItems.end())
			mItems.erase(itItem);
	}
	
	void SoraMenuBarMenu::enableItem(const std::wstring& name) {
		MENU_ITEM_CONT::iterator itItem = getItemIterator(name);
		if(itItem != mItems.end())
			itItem->setAvailable(true);
	}
	
	void SoraMenuBarMenu::disableItem(const std::wstring& name) {
		MENU_ITEM_CONT::iterator itItem = getItemIterator(name);
		if(itItem != mItems.end())
			itItem->setAvailable(false);
	}

	SoraMenuBarItem* SoraMenuBarMenu::getItem(const std::wstring& name) {
		MENU_ITEM_CONT::iterator itItem = getItemIterator(name);
		if(itItem != mItems.end())
			return &*itItem;
		return NULL;
	}
	
	SoraMenuBar* SoraMenuBarMenu::getParent() const {
		return mParent;
	}
	
	bool SoraMenuBarMenu::testPoint(float32 x, float32 y) {
		return (x >= mPosX && x <= mPosX+mWidth && y >= 0.f && y <= mItemHeight);
	}
	
	void SoraMenuBarMenu::adjustWidth() {
		if(mFont) {
			for(size_t i=0; i<mItems.size(); ++i) {
				float32 width = mFont->getStringWidth(mItems[i].getName().c_str());
				if(mWidth < width)
					mWidth = width;
			}
		}
	}
	
	float32 SoraMenuBarMenu::getWidth() const {
		return mWidth;
	}
	
	void SoraMenuBarMenu::setFont(SoraFont* font) {
		mFont = font;
		if(mFont) {
			mWidth = font->getStringWidth(mBarName.c_str()) + 20.f;
			mItemHeight = font->getHeight();
		}
	}
	
	void SoraMenuBarMenu::setActive(bool flag) {
		mActive = flag;
	}
	
	bool SoraMenuBarMenu::isActive() const {
		return mActive;
	}
	
	SoraMenuBarItem::SoraMenuBarItem(const std::wstring& name) {
		mItemName = name;
		mAvailable = true;
		mEventHandler = NULL;
	}
	
	void SoraMenuBarItem::onClick() {
		SoraMenuBarClickEvent event;
		event.setSource(this);
		if(mEventHandler)
			mEventHandler->handleEvent(&event);
	}
	
	std::wstring SoraMenuBarItem::getName() const {
		return mItemName;
	}
	
	void SoraMenuBarItem::setAvailable(bool flag) {
		mAvailable = flag;
	}
	
	bool SoraMenuBarItem::isAvailable() const {
		return mAvailable;
	}
	
	SoraMenuBar::SoraMenuBar(float32 height): 
	mFont(NULL),
	mMenuBarHeight(height),
	mMenuBarLength(0.f),
	mActive(false),
	mShowAlways(false),
	mMenuClicked(false),
	mEnabled(false) {
	}
	
	SoraMenuBar::~SoraMenuBar() {
		MENUBAR_LIST::iterator itMenu = mMenus.begin();
		while(itMenu != mMenus.end()) {
			delete (*itMenu);
			++itMenu;
		}
	}
	
	void SoraMenuBar::addMenu(SoraMenuBarMenu* bar) {
		bar->setFont(mFont);
		bar->mParent = this;
		bar->mActive = false;
		bar->mPosX = mMenuBarLength;
		bar->adjustWidth();
		
		mMenuBarLength += mFont->getStringWidth(bar->getName().c_str()) + 20.f;
		mMenus.push_back(bar);
	}
	
	void SoraMenuBar::delMenu(SoraMenuBarMenu* bar) {
		mMenus.remove(bar);
	}
	
	void SoraMenuBar::render() {
		if(!mEnabled || mMenus.size() == 0)
			return;
		
		if(mActive || mShowAlways) {
			float32 screenWidth = SORA->getScreenWidth();
			SORA->renderRect(0.f, 0.f, screenWidth, mMenuBarHeight, screenWidth, 0x77DDDDDD);
			SORA->renderBox(0.f, 0.f, screenWidth-1, mMenuBarHeight-1, 0xFFAAAAAA);
			
			MENUBAR_LIST::iterator itMenu = mMenus.begin();
			while(itMenu != mMenus.end()) {
				(*itMenu)->render();
				++itMenu;
			}
		}
	}
	
	bool SoraMenuBar::activeMenu(float32 x, float32 y) {
		MENUBAR_LIST::iterator itMenu = mMenus.begin();
		while(itMenu != mMenus.end()) {
			if(!(*itMenu)->isActive() && (*itMenu)->testPoint(x, y)) {
				MENUBAR_LIST::iterator itMenu2 = mMenus.begin();
				while(itMenu2 != mMenus.end()) {
					(*itMenu2)->setActive(false);
					++itMenu2;
				}
				
				(*itMenu)->setActive(true);
				return true;
			}
			
			++itMenu;
		}
		return false;
	}
	
	void SoraMenuBar::update() {
		if(!mEnabled || mMenus.size() == 0)
			return;
		
		float32 posx, posy;
		SORA->getMousePos(&posx, &posy);
		
		if(mActive || mShowAlways) {
			bool menuClicked = false;
			
			MENUBAR_LIST::iterator itMenu = mMenus.begin();
			while(itMenu != mMenus.end()) {
				if((*itMenu)->update())
					menuClicked = true;
				++itMenu;
			}
			
			if(mMenuClicked) {
				if(activeMenu(posx, posy))
					menuClicked = true;
				
			} else {
				if(SORA->keyDown(SORA_KEY_LBUTTON)) {
					if(activeMenu(posx, posy)) {
						menuClicked = true;
						mMenuClicked = true;
					}
				}
			}
			
			if(SORA->keyDown(SORA_KEY_RBUTTON)) {
				diactiveMenus();
			}
			
			if(!menuClicked && !mShowAlways) {
				if(!(posx >= 0.f && posx <= SORA->getScreenWidth() && posy >= 0.f && posy <= mMenuBarHeight))
					mActive = false;
			}
		} else {
			if(posx >= 0.f && posx <= SORA->getScreenWidth() && posy >= 0.f && posy <= mMenuBarHeight)
				mActive = true;
		}
	}
	
	void SoraMenuBar::diactiveMenus() {
		MENUBAR_LIST::iterator itMenu = mMenus.begin();
		while(itMenu != mMenus.end()) {
			(*itMenu)->setActive(false);
			++itMenu;
		}
		
		mMenuClicked = false;
		if(!mShowAlways)
			mActive = false;
	}
	
	void SoraMenuBar::setFont(SoraFont* font) {
		mFont = font;
		MENUBAR_LIST::iterator itMenu = mMenus.begin();
		while(itMenu != mMenus.end()) {
			(*itMenu)->setFont(font);
			++itMenu;
		}
	}
	
	SoraFont* SoraMenuBar::getFont() const {
		return mFont;
	}
	
	float32 SoraMenuBar::getMenuBarHeight() const {
		return mMenuBarHeight;
	}
	
	void SoraMenuBar::setMenuBarHeight(float32 height) {
		mMenuBarHeight = height;
	}
	
	void SoraMenuBar::setShowAlways(bool flag) {
		mShowAlways = flag;
		if(flag)
			mActive = true;
	}
	
	bool SoraMenuBar::isActive() const {
		return mActive;
	}
	
	bool SoraMenuBar::isShowAlways() const {
		return mShowAlways;
	}
	
	void SoraMenuBar::setEnabled(bool flag) {
		mEnabled = flag;
	}
	
	bool SoraMenuBar::isEnabled() const {
		return mEnabled;
	}

} // namespace sora