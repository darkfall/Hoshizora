#ifndef SORA_CORE_LOGGER_H
#define SORA_CORE_LOGGER_H

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "SoraStringConv.h"

#include <vector>
#include <fstream>

namespace sora {
	//namespace sorainternal {
	class SoraCore;
	
	
    enum {
		LOG_LEVEL_NORMAL = 1,
		LOG_LEVEL_ERROR = 2, 
		LOG_LEVEL_WARNING = 3,
		LOG_LEVEL_NOTICE = 4,
	};

	class SORA_API SoraInternalLogger {
		friend class SoraCore;
		
	protected:
		SoraInternalLogger();
		~SoraInternalLogger();
		
	public:
        static SoraInternalLogger* Instance();
        static void Destroy();
        
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
        
        void normal(const std::string& log);
        void error(const std::string& log);
        void warning(const std::string& log);
        void notice(const std::string& lob);
        
        void normal(const std::wstring& log);
        void error(const std::wstring& log);
        void warning(const std::wstring& log);
        void notice(const std::wstring& lob);
		
		void writeToFile(const char* fileName);
		
		const std::vector<LogMssg>& get() const;
		
		void clearLogs();
		
		size_t logSize() const;
		
	private:
        static SoraInternalLogger* mInstance;
        
		std::vector<LogMssg> vMssg;
	};
	
	static std::wstring vamssgw(const wchar_t* format, ...) {
		va_list	ArgPtr;
#ifdef HAS_WSTRING
		wchar_t Message[1024] = {0};
		va_start(ArgPtr, format);
		vswprintf(Message, 1024, format, ArgPtr);
		va_end(ArgPtr);
		return Message;

#else
		char Message[1024] = {0};
		va_start(ArgPtr, format);
		vsprintf(Message, ws2sfast(format).c_str(), ArgPtr);
		va_end(ArgPtr);
		return s2wsfast(Message);

#endif
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
    
    inline void log_mssg(const std::string& log, int32 level=LOG_LEVEL_NORMAL) {
        DebugPtr->log(log, level);
    }
    
    inline void log_mssg(const std::wstring& log, int32 level=LOG_LEVEL_NORMAL) {
        DebugPtr->log(log, level);
    }
    
    inline void log_error(const std::string& log) {
        DebugPtr->error(log);
    }
    
    inline void log_warning(const std::string& log) {
        DebugPtr->warning(log);
    }
    
    inline void log_notice(const std::string& log) {
        DebugPtr->notice(log);
    }
    
    inline void log_normal(const std::string& log) {
        DebugPtr->normal(log);
    }
	
#define DEBUG_LOG_HANDLE SoraInternalLogger::Instance()
	//} // namespace internal
} // namespace sora

#endif