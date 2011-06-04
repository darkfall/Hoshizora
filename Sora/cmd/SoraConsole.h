/*
 *  SoraCommandLine.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/4/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_COMMAND_LINE_H_
#define SORA_COMMAND_LINE_H_

#include "SoraPlatform.h"
#include "SoraSingleton.h"

#include "SoraEvent.h"
#include "SoraKeyInfo.h"

#include "SoraFont.h"

namespace sora {
	
	class SoraConsoleEvent: public SoraEvent {
	public:
		SoraConsoleEvent() {}
		SoraConsoleEvent(const std::string& cmd, const char* params):
		mCmd(cmd),
		mParams(params) {}
		
		void setCmd(const std::string& cmd) { mCmd = cmd; }
		void setParams(const std::string& params) { mParams = params; }
		
		std::string getCmd() const { return mCmd; }
		std::string getParams() const { return mParams; }
	
	private:
		std::string mCmd;
		std::string mParams;
	};
	
	class SoraConsole: public SoraSingleton<SoraConsole>, public SoraEventHandler {
	protected:
		friend class SoraSingleton<SoraConsole>;
		
		SoraConsole();
		~SoraConsole();
	
	public:	
		void render();
		
		void publishCmd(const std::string& cmd, const char* params);
		
		void registerCmdHandler(SoraEventHandler* handler, const std::string& cmd);
		void onKeyEvent(const SoraKeyEvent* kev);
		
		std::string getCurrentLine() const { return mCurrentLine; }
		
		uint32 getCaretRow() const { return mCaretRow; }
		uint32 getCaretPosition() const { return mCaretPosition; }
		
		void setCmdColor(uint32 color) { mCmdColor = color; }
		void setBackgroundColor(uint32 color) { mBackgroundColor = color; }
		void setCaretColor(uint32 color) { mCaretColor = color; }
		
		uint32 getCmdColor() const { return mCmdColor; }
		uint32 getBackgroundColor() const { return mBackgroundColor; }
		uint32 getCaretColor() const { return mCaretColor; }
		
		bool isActive() const { return mActive; }
		void setActiveKey(int32 key) { mActiveKey = key; }
		
		void setPosition(int32 posx, int32 posy) { mPositionX = posx; mPositionY = posy; }
		void setSize(int32 width, int32 height) { mWidth = width; mHeight = height; }
		
		void setFont(const std::wstring& font, int32 fontSize);
		
	private:
		void drawCaret();
		void drawCmds();
		
		bool mActive;
		int32 mActiveKey;
		
		int32 mPositionX, mPositionY;
		int32 mWidth, mHeight;
		
		typedef std::map<std::string, SoraEventHandler*> CMD_HANDLER_MAP;
		CMD_HANDLER_MAP mHandlers;
	
		std::string mCurrentLine;
		typedef std::vector<std::string> CMD_HISTORY;
		CMD_HISTORY mHistory;
		
		uint32 mCaretRow;
		uint32 mCaretPosition;
		
		uint32 mCmdColor;
		uint32 mBackgroundColor;
		uint32 mCaretColor;
		
		SoraFont* mFont;
		int32 mFontHeight;
		
		float32 mCaretShow;
	};
	
} // namespace sora

#endif // SORA_COMMAND_LINE_H_
