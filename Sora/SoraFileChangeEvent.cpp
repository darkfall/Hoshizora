/*
 *  SoraFileChangeEvent.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraFileChangeEvent.h"
#include "SoraEventManager.h"

#include "SoraLogger.h"

namespace sora {
	
	SoraFileChangeEventPublisher::SoraFileChangeEventPublisher(): mCheckInternval(2.f) {
		registerEventFunc(this, &SoraFileChangeEventPublisher::onCheckTimerEvent);
		
		SoraEventManager::Instance()->createTimerEvent(this, mCheckInternval, true);
	}
	
	void SoraFileChangeEventPublisher::setInterval(float32 interval) {
		mCheckInternval = interval;
		SoraEventManager::Instance()->unregisterTimerEvent(this);
		SoraEventManager::Instance()->createTimerEvent(this, mCheckInternval, true);
	}
	
	void SoraFileChangeEventPublisher::addEventHandler(const SoraWString& file, SoraEventHandler* handler) {	
		FILE_CHANGE_MAP::iterator itFile = mChangeListeners.find(file);
		if(itFile != mChangeListeners.end()) {
			mChangeListeners[file].mHandlers.push_back(handler);
		} else {
			FileChangeInfo info;
			FILE* fl = sora_fopenw(file.c_str(), "rb");
			if(fl) {
				MD5 md5(fl);
				FileChangeInfo info;
				info.mMD5 = md5.hex_digest();
				info.mHandlers.push_back(handler);
				
				mChangeListeners.insert(std::make_pair<SoraWString, FileChangeInfo>(file, info));
                
                fclose(fl);
			} else {
				log_mssg(vamssg("unable to open file %s", ws2s(file).c_str()));
			}
		}
	}
	
	void SoraFileChangeEventPublisher::delEventHandler(SoraEventHandler* handler) {
		FILE_CHANGE_MAP::iterator itFile = mChangeListeners.begin();
		while(itFile != mChangeListeners.end()) {
			FileChangeInfo::CHANGE_HANDLERS::iterator itHandler = itFile->second.mHandlers.begin();
			while(itHandler != itFile->second.mHandlers.end()) {
				if((*itHandler) == handler) {
					itFile->second.mHandlers.erase(itHandler);
					break;
				}
				++itHandler;
			}
			++itFile;
		}
	}
	
	void SoraFileChangeEventPublisher::onCheckTimerEvent(SoraTimerEvent* event) {
		FILE_CHANGE_MAP::iterator itFile = mChangeListeners.begin();
		while(itFile != mChangeListeners.end()) {
			FILE* file = sora_fopenw(itFile->first.c_str(), "rb");
			if(file) {
				MD5 nowmd5(file);
				if(nowmd5.hex_digest() != itFile->second.mMD5) {
					SoraFileChangeEvent event(itFile->first);
					
					FileChangeInfo::CHANGE_HANDLERS::iterator itHandler = itFile->second.mHandlers.begin();
					while(itHandler != itFile->second.mHandlers.end()) {
						(*itHandler)->handleEvent(&event);
						++itHandler;
                        
                        if(event.isConsumed())
                            break;
					}
					
					itFile->second.mMD5 = nowmd5.hex_digest();
				}
                fclose(file);
			}
            
			++itFile;
		}
	}
	
	
} // namespace sora

