/*
 *  SoraMenuBar.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/19/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_MENU_BAR_H_
#define SORA_MENU_BAR_H_

#include "../SoraPlatform.h"
#include "../SoraSingleton.h"
#include "../SoraEvent.h"
#include "../SoraFont.h"
#include "../SoraPlugin.h"

namespace sora {
	
	class SoraMenuBarItem;
	class SoraMenuBar;
	
	class SoraMenuBarClickEvent: public SoraEvent {
	public:		
	//	std::wstring getItemName() const;
	//	void setItemName(const std::wstring& name);
		SoraMenuBarItem* getItem() const;
		
	private:
	//	std::wstring mItemName;
	};
	
	class SoraMenuBarMenu {
		friend class SoraMenuBar;

	public:
		typedef std::vector<SoraMenuBarItem> MENU_ITEM_CONT;

		SoraMenuBarMenu(const std::wstring& barName);
		
		void render();
		bool update();
		
		void addItem(const std::wstring& itemName, SoraEventHandler* handler);
		void removeItem(const std::wstring& itemName);
		
		SoraMenuBarItem* getItem(const std::wstring& name);
		
		void enableItem(const std::wstring& itemName);
		void disableItem(const std::wstring& itemName);
		
		SoraMenuBar* getParent() const;
		
		void setActive(bool flag);
		bool isActive() const;
		
		bool testPoint(float32 x, float32 y);
		void setFont(SoraFont* font);
		
	private:
		std::wstring mBarName;
		SoraFont* mFont;
		
		float32 mPosX;
		float32 mWidth;
		float32 mItemHeight;
		
		int32 mCurrentItem;
		
		SoraMenuBar* mParent;
		
		MENU_ITEM_CONT mItems;		
		MENU_ITEM_CONT::iterator getItemIterator(const std::wstring& name);
		
		bool mActive;
	};
	
	class SoraMenuBarItem: public SoraEventHandler {
		friend class SoraMenuBarMenu;
		
	public:
		SoraMenuBarItem(const std::wstring& name);
		
		void onClick();
		
		std::wstring getName() const;
		
		void setAvailable(bool flag);
		bool isAvailable() const;
		
	private:
		bool mAvailable;
		std::wstring mItemName;
		
		SoraEventHandler* mEventHandler;
	};
	
	class SoraMenuBar: public SoraSingleton<SoraMenuBar> {
	public:
		SoraMenuBar(int32 height = 30);
		~SoraMenuBar();
		
		void addMenu(SoraMenuBarMenu* bar);
		void delMenu(SoraMenuBarMenu* bar);
		
		void render();
		void update();
		
		void setFont(SoraFont* font);
		SoraFont* getFont() const;
		
		int32 getMenuBarHeight() const;
		void setMenuBarHeight(int32 height);
		
		void setShowAlways(bool flag);
		bool isShowAlways() const;
		
		void setEnabled(bool flag);
		bool isEnabled() const;
		
		void diactiveMenus();
		
	private:
		inline bool activeMenu(float32 x, float32 y);
		
		int32 mMenuBarHeight;
		SoraFont* mFont;
		
		typedef std::list<SoraMenuBarMenu*> MENUBAR_LIST;
		MENUBAR_LIST mMenus;
		
		bool mEnabled;
		bool mActive;
		bool mShowAlways;
		bool mMenuClicked;
	};
	
	
} // namespace sora

#endif // SORA_MENU_BAR_H_