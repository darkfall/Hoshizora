/*
 *  SoraInternalLogger.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/28/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "SoraInternalLogger.h"

#include "SoraEventManager.h"
#include "SoraEvent.h"

namespace sora {
	
	SoraInternalLogger::SoraInternalLogger(): currLogIndex(0), logInterval(1.f) {
		logFile.open("SoraLog.txt");
		if(!logFile.is_open()) 
			log("LOGGER: cannot open log file. all logs would only remain while excuting time");
		
		registerEventFunc(this, &SoraInternalLogger::onLogTimerEvent);
		SORA_EVENT_MANAGER->createTimerEvent(this, logInterval, true);
	}
	SoraInternalLogger::~SoraInternalLogger() {
		logFile.close();
	}
	

	void SoraInternalLogger::log(const SoraString& mssg) {
		vMssg.push_back("LOG: "+mssg);
	}
	
	SoraInternalLogger& SoraInternalLogger::operator<<(SoraString& mssg) {
		vMssg.push_back("LOG: "+mssg);
		return *this;
	}
	
	SoraInternalLogger& SoraInternalLogger::operator<<(SoraWString& mssg) {
		vMssg.push_back("LOG: "+ws2s(mssg));
		return *this;
	}
	
	void SoraInternalLogger::printf(const char* format, ...) {
		va_list	ArgPtr;
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, format, ArgPtr);
		va_end(ArgPtr);
		
		::printf("LOG: %s", Message);
	}
	
	void SoraInternalLogger::printf(const wchar_t* format, ...) {
		va_list	ArgPtr;
		wchar_t Message[1024] = {0};
		va_start(ArgPtr, format);
		vswprintf(Message, 1024, format, ArgPtr);
		va_end(ArgPtr);
		
		::wprintf(L"LOG: %s", Message);
	}
	
	void SoraInternalLogger::logf(const char* format, ...) {
		va_list	ArgPtr;
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, format, ArgPtr);
		va_end(ArgPtr);
		
		printf(Message);
		vMssg.push_back(SoraString("LOG: ")+Message);
	}
	
	void SoraInternalLogger::logf(const wchar_t* format, ...) {
		va_list	ArgPtr;
		wchar_t Message[1024] = {0};
		va_start(ArgPtr, format);
		vswprintf(Message, 1024, format, ArgPtr);
		va_end(ArgPtr);
		
		printf(Message);
		SoraWString wstrLog = L"LOG: ";
		vMssg.push_back(ws2s(wstrLog+Message));
	}
	
	void SoraInternalLogger::debugPrintf(const char* format, ...) {
#ifdef _DEBUG
		va_list	ArgPtr;
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, format, ArgPtr);
		va_end(ArgPtr);
		
		::printf("DEBUG: %s", Message);
#endif
	}
	
	void SoraInternalLogger::debugLogf(const char* format, ...) {
#ifdef _DEBUG
		va_list	ArgPtr;
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, format, ArgPtr);
		va_end(ArgPtr);
		
		debugPrintf(Message);
		vMssg.push_back(SoraString("DEBUG: ")+Message);
#endif
	}
	
	void SoraInternalLogger::writeToFile(const char* fileName) {
		std::ofstream file(fileName);
		
		if(file.is_open()) {
			std::vector<SoraString>::iterator itLog = vMssg.begin();
			while(itLog != vMssg.end()) {
				file<<*itLog<<std::endl;
				++itLog;
			}
		}
		
		file.close();
	}
	
	/*
	 set the interval that the log writes to SoraLog.txt
	 default 1.f
	 */
	void SoraInternalLogger::setLogInternval(float32 interval) { 
		logInterval = interval;
	}
	
	void SoraInternalLogger::onLogTimerEvent(const SoraTimerEvent* tev) {
		if(!logFile.is_open())
			return;
				
		for(uint32 i=currLogIndex; i<vMssg.size(); ++i) {
			logFile<<vMssg[i]<<std::endl;
		}
		currLogIndex = vMssg.size();
	}
	
	const std::vector<SoraString>& SoraInternalLogger::get() const { 
		return vMssg; 
	}
	
} // namespace sora
