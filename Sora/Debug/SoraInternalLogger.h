#ifndef SORA_CORE_LOGGER_H
#define SORA_CORE_LOGGER_H

#include "SoraPlatform.h"
#include "SoraSingleton.h"

#include <vector>

namespace sora {
	//namespace sorainternal {
	class SoraCore;

		class SoraInternalLogger: public SoraSingleton<SoraInternalLogger> {
			friend class SoraSingleton<SoraInternalLogger>;
			friend class SoraCore;

		public:
			void log(const SoraString& mssg) {
				vMssg.push_back("_DEBUG: "+mssg);
			}

			SoraInternalLogger& operator<<(SoraString& mssg) {
				vMssg.push_back("_DEBUG: "+mssg);
				return *this;
			}
            
            static void printf(const char* format, ...) {
				va_list	ArgPtr;
				char Message[1024] = {0};
				va_start(ArgPtr, format);
				vsprintf(Message, format, ArgPtr);
				va_end(ArgPtr);
				
				::printf("LOG: %s", Message);
			}
			
            void logf(const char* format, ...) {
				va_list	ArgPtr;
				char Message[1024] = {0};
				va_start(ArgPtr, format);
				vsprintf(Message, format, ArgPtr);
				va_end(ArgPtr);
				
				printf(Message);
                vMssg.push_back(SoraString("LOG: ")+Message);
			}
            
			static void debugPrintf(const char* format, ...) {
#ifdef _DEBUG
				va_list	ArgPtr;
				char Message[1024] = {0};
				va_start(ArgPtr, format);
				vsprintf(Message, format, ArgPtr);
				va_end(ArgPtr);
				
				::printf("_DEBUG: %s", Message);
#endif
			}
			
			void debugLogf(const char* format, ...) {
#ifdef _DEBUG
				va_list	ArgPtr;
				char Message[1024] = {0};
				va_start(ArgPtr, format);
				vsprintf(Message, format, ArgPtr);
				va_end(ArgPtr);
				
				debugPrintf(Message);
                vMssg.push_back(SoraString("_DEBUG: ")+Message);
#endif
			}
			
			void writeToFile(const char* fileName) {
				FILE* file = fopen(fileName, "w");
				if(file) {
					std::vector<SoraString>::iterator itLog = vMssg.begin();
					while(itLog != vMssg.end()) {
						fwrite(itLog->c_str(), itLog->size(), 1, file);
						++itLog;
					}
				}
				fclose(file);
			}

			const std::vector<SoraString>& get() const { return vMssg; }
		
		private:
			std::vector<SoraString> vMssg;

			void clear() { vMssg.clear(); }

		};

		static SoraInternalLogger* LOG = SoraInternalLogger::Instance();
#define INT_LOG SoraInternalLogger
#define INT_LOG_HANDLE SoraInternalLogger::Instance()
	//} // namespace internal
} // namespace sora

#endif