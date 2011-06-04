/*
 *  SoraCommandLine.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/4/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraConsole.h"

#include "SoraEventManager.h"
#include "SoraCore.h"

#include "SoraStringConv.h"

namespace sora {

	SoraConsole::SoraConsole() {
		registerEventFunc(this, &SoraConsole::onKeyEvent);
		sora::SORA_EVENT_MANAGER->registerInputEventHandler(this);
		
		mCaretRow = 0;
		mCaretPosition = 0;
		
		mCaretColor = 0xFFFFFFFF;
		mBackgroundColor = 0x99000000;
		mCmdColor = 0xFFFFFFFF;
		
		mActive = false;
		mActiveKey = SORA_KEY_F1;
		
		mPositionX = 1;
		mPositionY = 1;
		
		mWidth = SoraCore::Instance()->getScreenWidth()-2;
		mHeight = 300;
		
		mFont = NULL;
		
		mCaretShow = 0.f;
	}
	
	SoraConsole::~SoraConsole() {
	}
	
	void SoraConsole::render() {
		if(mActive) {
			drawCmds();
			drawCaret();
			
			mCaretShow += SORA->getDelta();
		}
	}
	
	void SoraConsole::drawCmds() {
		SORA->renderBox(mPositionX, mPositionY, mPositionX+mWidth, mPositionY+mHeight, mCaretColor);
		SORA->renderRect(mPositionX, mPositionY, mPositionX+mWidth, mPositionY+mHeight, mWidth, mBackgroundColor);
		if(mFont) {
			int32 maxRow = mHeight / mFontHeight - 2;
			int32 startRow;
			if(mHistory.size() > maxRow) {
				startRow = mHistory.size() - maxRow;
				mCaretRow = maxRow + 1;
			}
			else {
				startRow = 0;
				mCaretRow = mHistory.size();
			}
			int32 x = mPositionX + 1;
			int32 y = mPositionY + 1;
			for(size_t i=startRow; i<mHistory.size(); ++i) {
				mFont->render(x, y, FONT_ALIGNMENT_LEFT, s2wsfast(">>"+mHistory[i]).c_str());
				y += mFontHeight;
			}
			std::wstring currLine = s2wsfast(">>"+mCurrentLine);
			mFont->render(x, y, FONT_ALIGNMENT_LEFT, currLine.c_str());
			
			if(mCaretShow <= 0.5f) {
				mFont->render(mFont->getStringWidth(currLine.c_str()), y, FONT_ALIGNMENT_LEFT, L"|");
			} else if(mCaretShow >= 1.0f)
				mCaretShow = 0.0f;
		}
	}
	
	void SoraConsole::drawCaret() {
		if(mFont) {
		
		}
	}
	
	void SoraConsole::registerCmdHandler(SoraEventHandler* handler, const std::string& cmd) {
		mHandlers[cmd] = handler;
	}
	
	void SoraConsole::setFont(const std::wstring& font, int32 fontSize) {
		mFont = SORA->createFont(font, fontSize);
		mFont->setColor(mCaretColor);
		mFontHeight = fontSize;
	}
	
	void SoraConsole::publishCmd(const std::string& cmd, const char* params) {
		CMD_HANDLER_MAP::iterator itHandler = mHandlers.find(cmd);
		if(itHandler != mHandlers.end()) {
			SoraConsoleEvent cev;
			
			cev.setCmd(cmd);
			if(params)
				cev.setParams(params);
			
			itHandler->second->handleEvent(&cev);
		}
	}
	
	void SoraConsole::onKeyEvent(const SoraKeyEvent* kev) {
		if(!mActive) {
			if(kev->isKeyPressed(mActiveKey))
				mActive = true;
			return;
		}
		
		if(kev->isKeyDown()) {
			int32 key = kev->getKey();
			if(key == mActiveKey) {
				mActive = false;
				return;
			}
			if(key >= 0 && key <= 255) {
				mCurrentLine += kev->chr;
			} 
			
			if(key == SORA_KEY_BACKSPACE){
				if(mCurrentLine.size() > 0)
					mCurrentLine.erase(mCurrentLine.size()-1);
			} else if(key == SORA_KEY_ENTER) {
				std::string cmd;
				
				size_t spacePos = mCurrentLine.find(' ');
				if(spacePos != std::string::npos) {
					cmd = mCurrentLine.substr(0, spacePos);
					size_t nextPos = spacePos;
					while(mCurrentLine[nextPos] == ' ' && nextPos < mCurrentLine.size())
						++nextPos;
					publishCmd(cmd, mCurrentLine.substr(nextPos, mCurrentLine.size()).c_str());
				}
				else {
					cmd = mCurrentLine;
					publishCmd(cmd, NULL);
				}
				
				mHistory.push_back(mCurrentLine);
				mCurrentLine.clear();
			}
		}
	}
	



} // namespace sora