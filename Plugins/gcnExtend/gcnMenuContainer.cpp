/*
 *  gcnMenuContainer.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/23/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "gcnMenuContainer.h"

namespace gcn {

	MenuContainer::MenuContainer(): bottomPosX(0), bottomPosY(0) {
		titleImage = NULL;
	}
	
	MenuContainer::MenuContainer(const std::string& title, bool bIsImage) {
		if(bIsImage)
			titleImage = Image::load(title);
		else
			titleString = title;
	}
	
	MenuContainer::~MenuContainer() {
		if(titleImage)
			delete titleImage;
	}
	
	void MenuContainer::setTitleImage(const std::string& title, bool bIsImage) {
		if(titleImage) {
			delete titleImage;
			titleImage = NULL;
		}
		
		if(bIsImage) {
			titleImage = Image::load(title);
			titleString.clear();
		}
		else {
			titleString = title;
		}
	}
	
	void MenuContainer::keyPressed(KeyEvent& keyEvent) {
		if(keyEvent.getKey() == Key::DOWN) {
			focusNext();
		} else if(keyEvent.getKey() == Key::UP) {
			focusPrevious();
		}
	}
	
	void MenuContainer::keyReleased(KeyEvent& keyEvent) {
	}
	
	void MenuContainer::add(Widget* widget) {
	}
	
	void MenuContainer::setAlignment(MenuItemAlignment align) {
		alignment = align;
	}
	
	MenuContainer::MenuItemAlignment MenuContainer::getAlignment() const {
		return alignment;
	}

} // namespace gcn