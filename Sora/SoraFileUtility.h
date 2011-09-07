#ifndef SORA_FILE_UTILITY_H
#define SORA_FILE_UTILITY_H

#include "SoraPlatform.h"
#include "SoraString.h"
#include <fstream>

namespace sora {

	class SORA_API SoraFileUtility {
	public:
		static bool fileExists(const StringType& filepath);
		
		static StringType getAbsolutePath(const StringType& filePath);
		
		/* 
		 get path in which can use to write file
		 specified for ios,
		 because ios limit writting files in app
		 also some part of osx
		 */
		static StringType getWrittablePath(const StringType& filePath);
		
		static StringType getApplicationPath();

		static StringType getFontPath(const StringType& file);

		static StringType getFullPath(const StringType& file);
		
		static StringType getFileName(const StringType& file);
    };

} // namespace sora

#endif