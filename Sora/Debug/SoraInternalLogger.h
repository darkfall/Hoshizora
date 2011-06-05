#ifndef SORA_CORE_LOGGER_H
#define SORA_CORE_LOGGER_H

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "SoraStringConv.h"
#include "SoraEvent.h"
#include "SoraTimerEvent.h"

#include <vector>
#include <fstream>

namespace sora {
	//namespace sorainternal {
	class SoraCore;
	
	
	typedef enum {
		LOG_LEVEL_NORMAL = 1,
		LOG_LEVEL_ERROR = 2, 
		LOG_LEVEL_WARNING = 3,
		LOG_LEVEL_NOTICE = 4,
	};

	class SoraInternalLogger: public SoraSingleton<SoraInternalLogger>, public SoraEventHandler {
		friend class SoraSingleton<SoraInternalLogger>;
		friend class SoraCore;
		
	protected:
		SoraInternalLogger();
		~SoraInternalLogger();
		
	public:
		typedef struct _LogMssg {
			SoraString mLog;
			int32 mLogLevel;
			
			_LogMssg(): mLogLevel(LOG_LEVEL_NORMAL) {}
			_LogMssg(const std::string& log, int32 level=LOG_LEVEL_NORMAL): mLog(log), mLogLevel(level) {}
			
		} LogMssg;
		
		SoraInternalLogger& operator<<(SoraString& mssg);
		SoraInternalLogger& operator<<(SoraWString& mssg);
		
		static void printf(const char* format, ...);
		static void printf(const wchar_t* format, ...);
		
		void log(const std::string& log, int32 logLevel=LOG_LEVEL_NORMAL);
		void log(const std::wstring& log, int32 logLevel=LOG_LEVEL_NORMAL);
		
		void writeToFile(const char* fileName);
		
		const std::vector<LogMssg>& get() const;
		
		void clearLogs();
		
		size_t logSize() const;
		
	private:
		std::vector<LogMssg> vMssg;
	};
	
	static std::wstring vamssgw(const wchar_t* format, ...) {
		va_list	ArgPtr;
		wchar_t Message[1024] = {0};
		va_start(ArgPtr, format);
		vswprintf(Message, 1024, format, ArgPtr);
		va_end(ArgPtr);
		return Message;
	}
	
	static std::string vamssg(const char* format, ...) {
		va_list	ArgPtr;
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, format, ArgPtr);
		va_end(ArgPtr);
		return Message;
	}
	
	typedef SoraInternalLogger Debug;
	static SoraInternalLogger* DebugPtr = SoraInternalLogger::Instance();
	
#define DEBUG_LOG_HANDLE SoraInternalLogger::Instance()
	//} // namespace internal
} // namespace sora

#endif