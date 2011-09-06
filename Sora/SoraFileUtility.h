#ifndef SORA_FILE_UTILITY_H
#define SORA_FILE_UTILITY_H

#include "SoraPlatform.h"

#include <fstream>

namespace sora {

	class SORA_API SoraFileUtility {
	public:
		static bool fileExists(const util::String& filepath);
		
		static util::String getAbsolutePath(const util::String& filePath);
		
		/* 
		 get path in which can use to write file
		 specified for ios,
		 because ios limit writting files in app
		 also some part of osx
		 */
		static util::String getWrittablePath(const util::String& filePath);
		
		static util::String getApplicationPath();

		static util::String getFontPath(const util::String& file);

		static util::String getFullPath(const util::String& file);
		// get file name from path
		static util::String getFileName(const util::String& file);
		
		static util::String getFileName(const util::String& file);
		
		static bool isFont(const util::String& file);
	};

} // namespace sora

#endif