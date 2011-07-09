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

#include "../SoraPlatform.h"
#include "../SoraSingleton.h"

#include "../SoraEvent.h"
#include "../SoraKeyInfo.h"
#include "../SoraHotkey.h"

#include "../SoraFont.h"

namespace sora {
	
	class SoraConsoleEvent: public SoraEvent {
	public:
		SoraConsoleEvent() {}
		SoraConsoleEvent(const std::string& cmd, const char* params):
		mCmd(cmd),
		mParams(params) {}
		
#ifndef SORA_USE_RTTI
		SORA_EVENT_IDENTIFIER(18446744073250990306ULL);
#endif
		
		void setCmd(const std::string& cmd) { mCmd = cmd; }
		void setParams(const std::string& params) { mParams = params; }
		
		/**
		 * Set the return result of the executed cmd, would be displayed in the console
		 * @parem results, the result of the cmd
		 */
		void setResults(const std::string& results) { mResults = results; }
		
		std::string getCmd() const { return mCmd; }
		std::string getParams() const { return mParams; }
		std::string getResults() const { return mResults; }
	
	private:
		std::string mCmd;
		std::string mParams;
		std::string mResults;
	};
	
	class SoraConsole: public SoraSingleton<SoraConsole>, public SoraEventHandler {
	protected:
		friend class SoraSingleton<SoraConsole>;
		
		SoraConsole();
		~SoraConsole();
	
	public:	
		typedef enum {
			TAB_CMDLINE = 1,
			TAB_MSSG = 2,
		};
		
		void render();
		
		void publishCmd(const std::string& cmd, const char* params);
		void outputMessage(const std::string& msg);
		
		void setTab(int32 tabid);
		int32 getTab() const { return mTab; }
		
		void registerCmdHandler(SoraEventHandler* handler, const std::string& cmd);
		void onKeyEvent(SoraKeyEvent* kev);
        void onHotkeyEvent(SoraHotkeyEvent* hev);
		
		std::string getCurrentLine() const { return mCurrentLine; }
		
		uint32 getCaretRow() const { return mCaretRow; }
		uint32 getCaretPosition() const { return mCaretPosition; }
		
		void setCmdColor(uint32 color) { mCmdColor = color; }
		void setBackgroundColor(uint32 color) { mBackgroundColor = color; }
		void setCaretColor(uint32 color) { mCaretColor = color; }
		void setResultColor(uint32 color) { mResultColor = color; }
		
		uint32 getCmdColor() const { return mCmdColor; }
		uint32 getBackgroundColor() const { return mBackgroundColor; }
		uint32 getCaretColor() const { return mCaretColor; }
		uint32 getResultColor() const { return mResultColor; }
		
		bool isActive() const { return mActive; }
		void setActiveKey(const SoraHotkey& key);
		
		void setPosition(float32 posx, float32 posy) { mPositionX = posx; mPositionY = posy; }
		void setSize(float32 width, float32 height) { mWidth = width; mHeight = height; }
		float32 getWidth() const { return mWidth; }
		float32 getHeight() const { return mHeight; }
		
		void setFont(SoraFont* font);
		SoraFont* getFont() { return mFont;}
		void reset();
        
        void setUseSysTerm(bool flag);
		
	private:
		void drawCmds();
		void drawMssg();
		void drawTab();
		
		bool mActive;
		int32 mActiveKeyId;
		
		bool mBackspaceDown;
		float32 mBackspaceTime;
		
		bool mUpDown, mDownDown;
		float32 mUpDownTime, mDownDownTime;
		
		bool mMssgReachTop;
        
        bool mUseSysTerm;
		
		int32 mTab;
		
		float32 mPositionX, mPositionY;
		float32 mWidth, mHeight;
		
		typedef std::map<std::string, SoraEventHandler*> CMD_HANDLER_MAP;
		CMD_HANDLER_MAP mHandlers;
	
		std::string mCurrentLine;
		typedef struct {
			std::wstring mCmd;
			std::wstring mResult;
		} CmdHistory;
		typedef std::vector<CmdHistory> CMD_HISTORY;
		CMD_HISTORY mHistory;
		
		uint32 mCaretRow;
		uint32 mCaretPosition;
		
		uint32 mCmdColor;
		uint32 mBackgroundColor;
		uint32 mCaretColor;
		uint32 mResultColor;
		
		SoraFont* mFont;
		int32 mFontHeight;
		
		int32 mCurrLine;
		int32 mStartLine;
		int32 mCurrHeight;
		float32 mCaretShow;
	};
	
} // namespace sora

#endif // SORA_COMMAND_LINE_H_
