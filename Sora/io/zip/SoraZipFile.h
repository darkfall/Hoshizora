#ifndef SORA_ZIP_FILE
#define SORA_ZIP_FILE

#include "SoraStringConv.h"
#include "io/SoraFileBase.h"

#include "ZLIB/unzip.h"

#include <map>

namespace sora {

	class SoraZipFile: public SoraFileBase {
	public:
		SoraZipFile();
		SoraZipFile(const util::String& filePath);
		~SoraZipFile();

        bool open(const util::String& filePath);
        bool open(void* lpszPackData, uLong nPackSize);
        void close();
	
        void setPackCode(const char* lpszPackCode);

        void* getFile(const util::String& filename, uLong readsize);

        ulong32 getFileSize(const util::String& filename);
        ulong32 getFileCRC(const util::String& filename);
		
        bool enumFiles(std::vector<util::String>& cont, const util::String& folder);

	private:
		void _releaseFile();
		void* _getfile(const util::String& filename, uLong readsize);
        void _cache();
        unz_file_info* _getUnzFileInfo(const char* lpszFileName);
        
		std::string packCode;
        
        unzFile m_UnzFile;		// modeRead
        unz_file_info	m_UnzFileInfo;	
        
        struct fileInfo {
            std::string fileName;
            uLong fileSize;
            uLong seekPos;
            
            fileInfo(const std::string& _fileName, uLong _fileSize, uLong _seekPos): fileName(_fileName), fileSize(_fileSize), seekPos(_seekPos) {}
        };
        
        typedef std::map<std::string, fileInfo> TSEEKER; 
        TSEEKER m_Seeker;
	};
} //namespace sora

#endif