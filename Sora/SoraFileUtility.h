#ifndef SORA_FILE_UTILITY_H
#define SORA_FILE_UTILITY_H

#include "SoraException.h"
#include "SoraPlatform.h"
#include "SoraSingleton.h"

#ifdef WIN32
 #include "Shlwapi.h"
 #pragma comment(lib,"shlwapi.lib")
#endif

#ifdef OS_OSX
#include "SoraOSXFileUtility.h"
#endif

#ifdef OS_IOS
#include "soraiOSFileUtility.h"
#endif

#include <fstream>

namespace sora {

	class SoraFileUtility {
	public:
		static bool fileExists(const SoraWString& filepath) {
#ifdef WIN32
			return PathFileExists(filepath.c_str())?true:false;
#elif defined(OS_OSX)
			return osxFileExists(filepath);
#elif defined(OS_IOS)
			return appFileExists(filepath);
#endif
			std::ifstream file;
			file.open(ws2s(filepath).c_str());
			if(!file.is_open()) return false;
			file.close();
			return true;
		}
		
		static SoraWString getApplicationPath() {
#ifdef WIN32
			wchar_t buffer[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, buffer);
			
			return buffer;
#elif defined(OS_OSX)
			return osxApplicationPath();
#elif defined(OS_IOS)
			return appResourcePath();
#endif
			return L"./";
		}

		static SoraWString getFontPath(const SoraWString& file) {
#ifdef OS_OSX
			SoraWString buffer = L"/System/Library/Fonts/";
			buffer += file;
			if(fileExists(buffer)) return buffer;
			buffer = L"/Library/Fonts/";
			buffer += file;
			
			return buffer;
#elif defined(WIN32)
			SoraWString buffer = L"C:/Windows/Fonts/";
			buffer += file;
			
			return buffer;
#endif
			return FONT_PATH;
			return L"./";
		}

		static SoraWString getFullPath(const SoraWString& file) {
			return (getApplicationPath() + L"/" + file);
		}
		
		static void* readFile(const char* pstrFilePath, ulong32& size) {
			FILE* file = fopen(pstrFilePath, "rb");
			if(file) {
				fseek(file, 0L, SEEK_END);
				size = ftell(file);
				fseek(file, 0L, SEEK_SET);
				
				void* buffer = malloc(size);
				if(buffer) {
					ulong32 readsize = fread(buffer, 1, size, file);
					fclose(file);
					if(readsize != size) {
						free(buffer);
						return 0;
					}
					
					fclose(file);
					return buffer;
				}
				fclose(file);
			}
			return 0;
		}

		
		static void* readFiles(const char* pstrFile, ulong32 size) {
			FILE* file = fopen(pstrFile, "rb");
			if(file) {
				void* buffer = malloc(size);
				if(buffer) {
					ulong32 readsize = fread(buffer, 1, size, file);
					fclose(file);
					if(readsize != size) {
						free(buffer);
						return 0;
					}
					
					fclose(file);
					return buffer;
				}
				fclose(file);
			}
			return 0;
		}
		
		static bool isFont(const SoraWString& file) {
			if(file.find(L".ttf") != std::string::npos ||
			   file.find(L".TTF") != std::string::npos ||
			   file.find(L".otf") != std::string::npos ||
			   file.find(L".ttc") != std::string::npos)
				return true;
			return false;
		}
	};

} // namespace sora

#endif