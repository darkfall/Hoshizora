#ifndef SORA_ZIP_FILE
#define SORA_ZIP_FILE

#include "SoraFileBase.h"
#include "SoraStringConv.h"

#include "PackReader.h"
#include "MyFileReader.h"

namespace sora {

	class SoraZipFile: public SoraFileBase {
	public:
		SoraZipFile();
		SoraZipFile(const SoraWString& filePath);
		~SoraZipFile();

		virtual int32 readFile(const SoraWString& filePath);
		virtual int32 readFileMem(void* lpszPackData, uLong nPackSize);
		virtual void  closeFile();
	
		virtual void setPackCode(const char* lpszPackCode);

		virtual void* getFile(const SoraWString& filename, uLong readsize);

		virtual uLong getFileSize(const SoraWString& filename);
		virtual uLong getFileCRC(const SoraWString& filename);

	private:
		void _releaseFile();
		void* _getfile(const SoraWString& filename, uLong readsize);

		std::string packCode;

		BOGY::PackReader* pReader;
	};
} //namespace sora

#endif