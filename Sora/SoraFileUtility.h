#ifndef SORA_FILE_UTILITY_H
#define SORA_FILE_UTILITY_H

#include "SoraPlatform.h"

#include <fstream>

namespace sora {

	class SORA_API SoraFileUtility {
	public:
		static bool fileExists(const wchar_t* filepath);
		
		static SoraWString getAbsolutePath(const wchar_t* filePath);
		
		/* 
		 get path in which can use to write file
		 specified for ios,
		 because ios limit writting files in app
		 also some part of osx
		 */
		static SoraWString getWrittablePath(const wchar_t* filePath);
		
		static SoraWString getApplicationPath();

		static SoraWString getFontPath(const wchar_t* file);

		static SoraWString getFullPath(const wchar_t* file);
		// get file name from path
		static SoraWString getFileName(const wchar_t* file);
		
		static SoraString getFileName(const char* file);
		
		static void* readFile(const char* pstrFilePath, ulong32& size);

		
		static void* readFileWithSize(const char* pstrFile, ulong32 size);
		static bool isFont(const wchar_t* file);
	};

} // namespace sora

#endif