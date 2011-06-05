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
	
	SoraInternalLogger::SoraInternalLogger() {
	}
	
	SoraInternalLogger::~SoraInternalLogger() {
	}
	
	SoraInternalLogger& SoraInternalLogger::operator<<(SoraString& mssg) {
		vMssg.push_back(mssg);
		return *this;
	}
	
	SoraInternalLogger& SoraInternalLogger::operator<<(SoraWString& mssg) {
		vMssg.push_back(ws2s(mssg));
		return *this;
	}
	
	void SoraInternalLogger::printf(const char* format, ...) {
		va_list	ArgPtr;
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, format, ArgPtr);
		va_end(ArgPtr);
		
		::printf("%s", Message);
	}
	
	void SoraInternalLogger::printf(const wchar_t* format, ...) {
		va_list	ArgPtr;
		wchar_t Message[1024] = {0};
		va_start(ArgPtr, format);
		vswprintf(Message, 1024, format, ArgPtr);
		va_end(ArgPtr);
		
		::wprintf(L"%s", Message);
	}
	
	void SoraInternalLogger::log(const std::string& log, int32 logLevel) {
		vMssg.push_back(LogMssg(log, logLevel));
	}
	
	void SoraInternalLogger::log(const std::wstring& log, int32 logLevel) {
		vMssg.push_back(LogMssg(ws2s(log), logLevel));
	}
	
	void SoraInternalLogger::writeToFile(const char* fileName) {
		std::ofstream file(fileName);
		
		if(file.is_open()) {
			std::vector<LogMssg>::iterator itLog = vMssg.begin();
			while(itLog != vMssg.end()) {
				file<<itLog->mLog<<std::endl;
				++itLog;
			}
		}
		
		file.close();
	}
	
	
	size_t SoraInternalLogger::logSize() const {
		return vMssg.size();
	}
	
	const std::vector<SoraInternalLogger::LogMssg>& SoraInternalLogger::get() const { 
		return vMssg; 
	}
	
} // namespace sora
