/*
 *  SoraCommandLine.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/4/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraConsole.h"

#include "../SoraEventManager.h"
#include "../SoraCore.h"

#include "../SoraStringConv.h"
#include "../Debug/SoraInternalLogger.h"

namespace sora {
    
    namespace {
        static SoraConsole* mInstance = NULL;
    }
    
    SoraConsole* SoraConsole::Instance() {
        if(!mInstance)
            mInstance = new SoraConsole;
        return mInstance;
    }
    
    void SoraConsole::Destroy() {
        if(mInstance) {
            delete mInstance;
            mInstance = NULL;
        }
    }

	SoraConsole::SoraConsole(): mStartLine(0) {
		registerEventFunc(this, &SoraConsole::onKeyEvent);
        registerEventFunc(this, &SoraConsole::onHotkeyEvent);
		SORA_EVENT_MANAGER->registerInputEventHandler(this, 9999);
        
        SoraCore::Instance()->addFrameListener(this);
        
		reset();
	}
	
	SoraConsole::~SoraConsole() {
        SoraCore::Instance()->delFrameListener(this);
	}
	
	void SoraConsole::reset() {
		mCaretRow = 0;
		mCaretPosition = 0;
		
		mCaretColor = 0xFFFFFFFF;
		mBackgroundColor = 0xAA000000;
		mCmdColor = 0xFFFFFFFF;
		mResultColor = 0xFF22DD00;
		
		mActive = false;
		mActiveKeyId = SORA->registerGlobalHotkey(SoraHotkey(SORA_KEY_GRAVE), this);
		
		mBackspaceDown = false;
		mBackspaceTime = 0.f;
		
		mUpDown = mDownDown = false;
		mUpDownTime = mDownDownTime = 0.f;
		
		mMssgReachTop = true;
        mUseSysTerm = false;
		
		mPositionX = 1;
		mPositionY = 1;
		
		mWidth = SoraCore::Instance()->getScreenWidth()-2.f;
		mHeight = 300.f;
		if(mHeight > SoraCore::Instance()->getScreenHeight())
			mHeight = (float32)SoraCore::Instance()->getScreenHeight();
		
		mFont = NULL;
		mFontHeight = 20;
		
		mCaretShow = 0.f;
		
		mTab = TAB_CMDLINE;
		
		mCurrLine = 0;
		mStartLine = 0;
		mCurrHeight = 0;
	}
	
	void SoraConsole::render() {
		if(mActive) {
			float32 delta = SORA->getDelta();
			if(mTab == TAB_CMDLINE) {
				drawCmds();
				
				mCaretShow += delta;
			} else if(mTab == TAB_MSSG) {
				drawMssg();
			}
			drawTab();
			
			if(mBackspaceDown) {
				if(mBackspaceTime >= 0.1f) {
					if(mCurrentLine.size() > 0)
						mCurrentLine.erase(mCurrentLine.size()-1);
					mBackspaceTime = 0.f;
				} else 
					mBackspaceTime += delta;
			}
			
			if(mUpDown) {
				if(mUpDownTime >= 0.2f) {
					if(mTab == TAB_CMDLINE) {
						if(mStartLine > 0) {
							mCurrLine -= 1;
							mStartLine -= 1;
						} 
					} else if(mTab == TAB_MSSG) {
						if(mCurrLine > 0 && !mMssgReachTop) {
							mCurrLine -= 1;
						} 
					}
				} else 
					mUpDownTime += delta;
			} 
			if(mDownDown) {
				if(mDownDownTime >= 0.2f) {
					if(mTab == TAB_CMDLINE) {
						if(mCurrLine < mHistory.size()) {
							++mCurrLine;
							++mStartLine;
						}
					} else if(mTab == TAB_MSSG) {
						if(mCurrLine < DebugPtr->logSize()-1) {
							++mCurrLine;
						}
					}
				} else 
					mDownDownTime += delta;
			}
		}
	}
	
	void SoraConsole::drawTab() {
		uint32 hCol = 0xFF909090;
		SORA->fillBox(mPositionX+mWidth-120.f, 
						 mPositionY+mHeight-mFontHeight, 
						 mPositionX+mWidth-60.f,
						 mPositionY+mHeight,
						 mTab==TAB_CMDLINE?hCol:mBackgroundColor);
		SORA->fillBox(mPositionX+mWidth-60.f, 
						 mPositionY+mHeight-mFontHeight, 
						 mPositionX+mWidth,
						 mPositionY+mHeight,
						 mTab==TAB_MSSG?hCol:mBackgroundColor);
		SORA->renderBox(mPositionX+mWidth-120.f, 
						mPositionY+mHeight-mFontHeight, 
						mPositionX+mWidth-60.f,
						mPositionY+mHeight,
						mCaretColor);
		SORA->renderBox(mPositionX+mWidth-60.f, 
						mPositionY+mHeight-mFontHeight, 
						mPositionX+mWidth,
						mPositionY+mHeight,
						mCaretColor);
		if(mFont) {
			mFont->setColor(mCaretColor);
			mFont->render(mPositionX+mWidth-90.f, mPositionY+mHeight-mFontHeight*1.2f, FONT_ALIGNMENT_CENTER, L"Cmd");
			mFont->render(mPositionX+mWidth-30.f, mPositionY+mHeight-mFontHeight*1.2f, FONT_ALIGNMENT_CENTER, L"Log");
		}
	}
					  
	
	void SoraConsole::drawCmds() {
		SORA->renderBox(mPositionX, mPositionY, mPositionX+mWidth, mPositionY+mHeight, mCaretColor);
		SORA->fillBox(mPositionX, mPositionY, mPositionX+mWidth, mPositionY+mHeight, mBackgroundColor);
		if(mFont) {			
			mFont->setLineWidth(mWidth);
			float32 x = mPositionX + 1.f;
			float32 y = mPositionY + 1.f;
			
			if(mHistory.size() != 0) {
				for(size_t i=mStartLine; i<mCurrLine; ++i) {
					mFont->setColor(mCaretColor);
					mFont->render(x, y, FONT_ALIGNMENT_LEFT, (L"> "+mHistory[i].mCmd).c_str());
					y += mFontHeight;
					
					if(mHistory[i].mResult.size() != 0) {
						mFont->setColor(mResultColor);
						mFont->render(x, y, FONT_ALIGNMENT_LEFT, mHistory[i].mResult.c_str());
						y += mFont->getStringHeight(mHistory[i].mResult.c_str());
					}
				}
			}
			
			mFont->setColor(mCaretColor);
			std::wstring currLine = s2wsfast("> "+mCurrentLine);
			mFont->render(x, y, FONT_ALIGNMENT_LEFT, currLine.c_str());
			
			if(mCaretShow <= 0.5f) {
				mFont->render(mFont->getStringWidth(currLine.c_str()), y, FONT_ALIGNMENT_LEFT, L"|");
			} else if(mCaretShow >= 1.0f)
				mCaretShow = 0.0f;
			mFont->setLineWidth(0.f);
		}
	}
	
	void SoraConsole::drawMssg() {
		SORA->renderBox(mPositionX, mPositionY, mPositionX+mWidth, mPositionY+mHeight, mCaretColor);
		SORA->fillBox(mPositionX, mPositionY, mPositionX+mWidth, mPositionY+mHeight, mBackgroundColor);
		if(mFont) {
			mFont->setLineWidth(mWidth);
			float32 x = mPositionX + 1.f;
			float32 y = mPositionY + mHeight - mFontHeight*2 - 1.f;
			
			std::vector<SoraInternalLogger::LogMssg> debugMssg = DebugPtr->get();
			
			float32 starty = y;
			for(int32 i=mCurrLine; i>=0; --i) {
				starty -= mFont->getStringHeight(s2wsfast(debugMssg[i].mLog).c_str());
				if(starty <= mPositionY + mFontHeight)
					break;
			}
			if(starty > mPositionY + mFontHeight) {
				y -= (starty - mPositionY - mFontHeight);
				y -= mFontHeight;
				mMssgReachTop = true;
			} else {
				mMssgReachTop = false;
			}


			for(int32 i=mCurrLine; i>=0; --i) {
				switch(debugMssg[i].mLogLevel) {
					case LOG_LEVEL_NORMAL:	mFont->setColor(0xFFFFFFFF); break;
					case LOG_LEVEL_NOTICE:	mFont->setColor(0xFF30AAFF); break;
					case LOG_LEVEL_ERROR:	mFont->setColor(0xFFFF0000); break;
					case LOG_LEVEL_WARNING: mFont->setColor(0xFFFFCC00); break;
				}
				mFont->render(x, y, FONT_ALIGNMENT_LEFT, s2wsfast("> "+debugMssg[i].mLog).c_str());
				if(i > 0)
					y -= mFont->getStringHeight(s2wsfast(debugMssg[i-1].mLog).c_str());
			
				if(y <= mPositionY + 1)
					break;
			}
			mFont->setLineWidth(0.f);
		}
	}
	
	void SoraConsole::setTab(int32 tab) {
		mTab = tab;
		mCaretShow = 0.f;
		if(mTab == TAB_CMDLINE) {
			mCurrLine = mHistory.size();
		} else if(mTab == TAB_MSSG) {
			mCurrLine = DebugPtr->logSize()-1;
		}
	}
    
    void SoraConsole::setActiveKey(const SoraHotkey& key) {
        sora::SORA->setGlobalHotkey(mActiveKeyId, key);
    }
	
	void SoraConsole::registerCmdHandler(SoraEventHandler* handler, const std::string& cmd) {
		mHandlers[cmd] = handler;
	}
	
	void SoraConsole::setFont(SoraFont* font) {
		mFont = font;
		if(mFont) {
			mFont->setColor(mCaretColor);
			mFontHeight = (int32)mFont->getHeight();
		}
	}
	
	void SoraConsole::publishCmd(const std::string& cmd, const char* params) {
		if(cmd.compare("clear") == 0) {
			mHistory.clear();
			mCurrLine = 0;
			mStartLine = 0;
			mCurrHeight = 0;
			return;
		}
		
		CMD_HANDLER_MAP::iterator itHandler = mHandlers.find(cmd);
		
		CmdHistory history;
		if(params != NULL)
			history.mCmd = s2wsfast(cmd+" "+params);
		else
			history.mCmd = s2wsfast(cmd);
		if(itHandler != mHandlers.end()) {
			SoraConsoleEvent cev;
			
			cev.setCmd(cmd);
			if(params)
				cev.setParams(params);
			
			itHandler->second->handleEvent(&cev);

			history.mResult = s2wsfast(cev.getResults());
		} else {
            if(mUseSysTerm)
                SORA->execute(cmd, params!=NULL?params:"");
		}
		
		mHistory.push_back(history);
		mCurrLine += 1;
		mCurrHeight += mFontHeight;
		if(mFont) {
			if(history.mResult.size() != 0)
				mCurrHeight += (int32)mFont->getStringHeight(history.mResult.c_str());
		}
		if(mCurrHeight >= mHeight-1-mFontHeight) {
			mStartLine += 1;
			mCurrHeight = (int32)(mHeight-1-mFontHeight);
		}
	}
    
    void SoraConsole::onHotkeyEvent(SoraHotkeyEvent* hev) {
        if(hev->getHotkeyId() == mActiveKeyId) {
            if(mActive)
                mActive = false;
            else
                mActive = true;
            hev->consume();
        }
    }
    
    void SoraConsole::setUseSysTerm(bool flag) {
        mUseSysTerm = flag;
    }
	
	void SoraConsole::onKeyEvent(SoraKeyEvent* kev) {
        if(!mActive)
            return;
		
		kev->consume();

		if(kev->isKeyDown()) {
			if(kev->isKeyPressed(SORA_KEY_TAB)) {
				if(mTab == TAB_CMDLINE) 
					setTab(TAB_MSSG);
				else if(mTab == TAB_MSSG)
					setTab(TAB_CMDLINE);
			}
		}
		
		int32 key = kev->getKey();
		if(key == SORA_KEY_BACKSPACE) {
			if(kev->isKeyDown()) {
				mBackspaceDown = true;
			}
			else {
				mBackspaceDown = false;
				mBackspaceTime = 0.f;
				if(mCurrentLine.size() > 0)
					mCurrentLine.erase(mCurrentLine.size()-1, 1);
			}
		}
		
		if(key == SORA_KEY_UP) {
			if(kev->isKeyDown()) {
				mUpDown = true;
				mUpDownTime = 0.f;
			} else {
				if(mTab == TAB_CMDLINE) {
					if(mStartLine > 0) {
						mCurrLine -= 1;
						mStartLine -= 1;
					} 
				} else if(mTab == TAB_MSSG) {
					if(mCurrLine > 0 && !mMssgReachTop) {
						mCurrLine -= 1;
					} 
				}
				mUpDown = false;
			}
		} else if(key == SORA_KEY_DOWN) {
			if(kev->isKeyDown()) {
				mDownDown = true;
				mDownDownTime = 0.f;
			} else {
				if(mTab == TAB_CMDLINE) {
					if(mCurrLine < mHistory.size()) {
						++mCurrLine;
						++mStartLine;
					}
				} else if(mTab == TAB_MSSG) {
					if(mCurrLine < DebugPtr->logSize()-1) {
						++mCurrLine;
					}
				}
				mDownDown = false;
			}
		}
		
		if(mTab == TAB_CMDLINE) {
			if(kev->isKeyDown()) {
				if(isKeyPrintable(key)) {
					mCurrentLine += kev->chr;
				} 
				
				if(key == SORA_KEY_ENTER) {
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
					mCurrentLine.clear();
				} 
			}
		} else if(mTab == TAB_MSSG) {
			if(kev->isKeyPressed(SORA_KEY_S) && kev->isCtrlFlag())
				DebugPtr->writeToFile("./SoraLog.log");
		}
	}
	
    void SoraConsole::onFrameStart() {
    //    update(SoraCore::Instance()->getDelta());
    }
    
    void SoraConsole::onFrameEnd() {
        render();
    }



} // namespace sora