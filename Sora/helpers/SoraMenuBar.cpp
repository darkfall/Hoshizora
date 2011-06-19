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
			if(x >= mPosX && x <= mPosX+mWidth) {
				if(y >= mParent->getMenuBarHeight() && y <= mParent->getMenuBarHeight()*(mItems.size()+1)) {
					y -= mParent->getMenuBarHeight();
					mCurrentItem = y / mItemHeight;
					
					mItems[mCurrentItem].onClick();
					mActive = false;
					return true;
				}
				
				if(SORA->keyDown(SORA_KEY_LBUTTON)) {
					if(y >= 0.f && y <= mParent->getMenuBarHeight()) {
						mActive = false;
					}
				}
			}
		}
		return false;
	}
	
	void SoraMenuBarMenu::render() {
		if(mFont) {
			mFont->render(mPosX, 0.f, mBarName.c_str(), false, true);
			
			if(mActive) {
				float32 posy = mParent->getMenuBarHeight();
				
				SORA->renderRect(mPosX, 0.f, mPosX+mWidth, posy, 0x2020AAFF, mWidth);
				SORA->renderRect(mPosX, posy, mPosX+mWidth, posy*mItems.size(), 0xAAAAAAFF, mWidth);
				
				for(size_t i=0; i<mItems.size(); ++i) {
					if(mItems[i].isAvailable())
						mFont->setColor(0xFFFFFFFF);
					else
						mFont->setColor(0xDDDDDDDD);
					mFont->render(mPosX, posy, mItems[i].getName().c_str(), false, true);
					if(mCurrentItem == i) {
						SORA->renderRect(mPosX, posy, mPosX+mWidth, mParent->getMenuBarHeight(), 0x2020AAFF, mWidth);
					}
					
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
		return (x >= mPosX && x <= mWidth && y >= 0.f && y <= mItemHeight);
	}
	
	void SoraMenuBarMenu::setFont(SoraFont* font) {
		mFont = font;
		if(mFont) {
			mWidth = font->getStringWidth(mBarName.c_str());
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
	
	SoraMenuBar::SoraMenuBar(int32 height): 
	mFont(NULL),
	mMenuBarHeight(height),
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
		
		mMenus.push_back(bar);
	}
	
	void SoraMenuBar::delMenu(SoraMenuBarMenu* bar) {
		mMenus.remove(bar);
	}
	
	void SoraMenuBar::render() {
		if(!mEnabled)
			return;
		
		if(mActive || mShowAlways) {
			float32 screenWidth = SORA->getScreenWidth();
			SORA->renderRect(0.f, 0.f, screenWidth, mMenuBarHeight, screenWidth, 0x88AAAAAA);
			SORA->renderBox(0.f, 0.f, screenWidth-1, mMenuBarHeight-1, 0xDDDDDDDD);
			
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
			if((*itMenu)->testPoint(x, y)) {
				(*itMenu)->setActive(true);
				return true;
			}
			
			++itMenu;
		}
		return false;
	}
	
	void SoraMenuBar::update() {
		if(!mEnabled)
			return;
		
		float32 posx, posy;
		SORA->getMousePos(&posx, &posy);
		
		if(mActive || mShowAlways) {
			MENUBAR_LIST::iterator itMenu = mMenus.begin();
			bool menuClicked = false;
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
					if(activeMenu(posx, posy))
						menuClicked = true;
					
					mMenuClicked = true;
					
				} else if(SORA->keyDown(SORA_KEY_RBUTTON)) {
					diactiveMenus();
				}
			}
			
			if(!menuClicked) {
				diactiveMenus();
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
	
	int32 SoraMenuBar::getMenuBarHeight() const {
		return mMenuBarHeight;
	}
	
	void SoraMenuBar::setMenuBarHeight(int32 height) {
		mMenuBarHeight = height;
	}
	
	void SoraMenuBar::setShowAlways(bool flag) {
		mShowAlways = flag;
		if(flag)
			mActive = true;
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