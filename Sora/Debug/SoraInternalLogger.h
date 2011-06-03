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

		class SoraInternalLogger: public SoraSingleton<SoraInternalLogger>, public SoraEventHandler {
			friend class SoraSingleton<SoraInternalLogger>;
			friend class SoraCore;
			
		protected:
			SoraInternalLogger();
			~SoraInternalLogger();

		public:
			void log(const SoraString& mssg);

			SoraInternalLogger& operator<<(SoraString& mssg);
			SoraInternalLogger& operator<<(SoraWString& mssg);
            
            static void printf(const char* format, ...);
			static void printf(const wchar_t* format, ...);			
			static void debugPrintf(const char* format, ...);

            void logf(const char* format, ...);			
            void logf(const wchar_t* format, ...);
			void debugLogf(const char* format, ...);
			
			void writeToFile(const char* fileName);
			
			/*
			 set the interval that the log writes to SoraLog.txt
			 default 1.f
			 */
			void setLogInternval(float32 interval);
			
			void onLogTimerEvent(const SoraTimerEvent* tev);

			const std::vector<SoraString>& get() const;
			
			void clearLogs();
		
		private:
			std::vector<SoraString> vMssg;
			
			std::ofstream logFile;
			
			int32 currLogIndex;
			float32 logInterval;
		};
	
	typedef SoraInternalLogger Debug;
	
#define INT_LOG_HANDLE SoraInternalLogger::Instance()
	//} // namespace internal
} // namespace sora

#endif