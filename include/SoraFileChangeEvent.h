/*
 *  SoraFileChangeEvent.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_FILE_CHANGE_EVENT_H_
#define SORA_FILE_CHANGE_EVENT_H_

#include "SoraEvent.h"
#include "SoraTimerEvent.h"

#include "support/md5lib.h"

#include <list>

namespace sora {
		
	class SORA_API SoraFileChangeEvent: public SoraEvent {
	public:
		SoraFileChangeEvent() {}
		SoraFileChangeEvent(const SoraWString& file): mChangedFile(file) {}
		
		void setChangedFile(const SoraWString& file) {
			mChangedFile = file;
		}
		
		SoraWString getChangedFile() const {
			return mChangedFile;
		}
		
#ifndef SORA_USE_RTTI
		SORA_EVENT_IDENTIFIER(18446744073686077189ULL);
#endif
	
	private:
		SoraWString mChangedFile;
	};
	
	class SORA_API SoraFileChangeEventPublisher: public SoraEventHandler {
	public:
		SoraFileChangeEventPublisher();
		
		void setInterval(float32 interval);
		void addEventHandler(const SoraWString& file, SoraEventHandler* handler);
		void delEventHandler(SoraEventHandler* handler);
		void onCheckTimerEvent(SoraTimerEvent* event);
		
	private:
		struct FileChangeInfo {			
			std::string mMD5;
			
			typedef std::list<SoraEventHandler*> CHANGE_HANDLERS;
			CHANGE_HANDLERS mHandlers;
		};
		
		typedef std::map<SoraWString, FileChangeInfo> FILE_CHANGE_MAP;
		FILE_CHANGE_MAP mChangeListeners;
		
		float32 mCheckInternval;
	};
	
	
} // namespace sora


#endif // SORA_FILE_CHANGE_EVENT_H_