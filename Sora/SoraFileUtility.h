#ifndef SORA_FILE_UTILITY_H
#define SORA_FILE_UTILITY_H

#include "SoraPlatform.h"

#include <fstream>

namespace sora {

	class SoraFileUtility {
	public:
		static bool fileExists(const SoraWString& filepath);
		
		static SoraWString getAbsolutePath(const SoraWString& filePath);
		
		/* 
		 get path in which can use to write file
		 specified for ios,
		 because ios limit writting files in app
		 also some part of osx
		 */
		static SoraWString getWrittablePath(const SoraWString& filePath);
		
		static SoraWString getApplicationPath();

		static SoraWString getFontPath(const SoraWString& file);

		static SoraWString getFullPath(const SoraWString& file);
		// get file name from path
		static SoraWString getFileName(const SoraWString& file);
		
		static SoraString getFileName(const SoraString& file);
		
		static void* readFile(const char* pstrFilePath, ulong32& size);

		
		static void* readFileWithSize(const char* pstrFile, ulong32 size);
		static bool isFont(const SoraWString& file);
	};

} // namespace sora

#endif