#include "SoraFileUtility.h"
#include "SoraStringConv.h"

#ifdef OS_WIN32
#include <Windows.h>
 #include "Shlwapi.h"
 #pragma comment(lib,"shlwapi.lib")
#endif

#ifdef OS_OSX
#include "SoraOSXFileUtility.h"
#endif

#ifdef OS_IOS
#include "soraiOSFileUtility.h"
#endif

#include "SoraPath.h"


namespace sora {

 bool SoraFileUtility::fileExists(const wchar_t* filepath) {
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
		
	 SoraWString SoraFileUtility::getAbsolutePath(const wchar_t* filePath) {
#ifdef WIN32
			wchar_t buffer[MAX_PATH];
			GetCurrentDirectoryW(MAX_PATH, buffer);
			
			return SoraWString(buffer)+L"/"+filePath;
#elif defined(OS_OSX)
			return osxApplicationPath()+L"/"+filePath;
#elif defined(OS_IOS)
			return appResourcePath()+L"/"+filePath;
#endif
			return L"./"+filePath;
		}
		
		/* 
		 get path in which can use to write file
		 specified for ios,
		 because ios limit writting files in app
		 also some part of osx
		 */
	 SoraWString SoraFileUtility::getWrittablePath(const wchar_t* filePath) {
#ifndef OS_IOS
			return getAbsolutePath(filePath);
#else
			return appDocumentPath()+L"/"+filePath;
#endif
			return L"\0";
		}
		
	 SoraWString SoraFileUtility::getApplicationPath() {
#ifdef WIN32
			wchar_t buffer[MAX_PATH];
			GetCurrentDirectoryW(MAX_PATH, buffer);
			
			return SoraWString(buffer)+L"/";
#elif defined(OS_OSX)
			return osxApplicationPath()+L"/";
#elif defined(OS_IOS)
			return appResourcePath()+L"/";
#endif
			return L"./";
		}

	 SoraWString SoraFileUtility::getFontPath(const wchar_t* file) {
         std::wstring fontPath = SoraPath::fontW();
         return fontPath + file;
     }

	 SoraWString SoraFileUtility::getFullPath(const wchar_t* file) {
			return (getApplicationPath() + L"/" + file);
		}
		
		// get file name from path
	 SoraWString SoraFileUtility::getFileName(const wchar_t* file) {
			SoraWString op = file;
			for(size_t i=0; i<op.size(); ++i)
				if(op[i] == L'\\')
					op[i] = L'/';
			size_t rspos = op.rfind(L"/");
			if(rspos != SoraWString::npos)
				op.erase(0, rspos+1);
			return op;
		}
		
	 SoraString SoraFileUtility::getFileName(const char* file) {
			SoraString op = file;
			for(size_t i=0; i<op.size(); ++i)
				if(op[i] == '\\')
					op[i] = '/';
			size_t rspos = op.rfind("/");
			if(rspos != SoraString::npos)
				op.erase(0, rspos+1);
			return op;
		}
		
	 void* SoraFileUtility::readFile(const char* pstrFilePath, ulong32& size) {
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

		
	 void* SoraFileUtility::readFileWithSize(const char* pstrFile, ulong32 size) {
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
		
	 bool SoraFileUtility::isFont(const wchar_t* file) {
			if(file.find(L".ttf") != std::string::npos ||
			   file.find(L".TTF") != std::string::npos ||
			   file.find(L".otf") != std::string::npos ||
			   file.find(L".ttc") != std::string::npos)
				return true;
			return false;
		}

} // namespace sora