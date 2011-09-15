#ifndef SORA_ZIP_FILE
#define SORA_ZIP_FILE

#include "SoraStringConv.h"
#include "SoraFileBase.h"

#include "ZLIB/unzip.h"

#include <map>

namespace sora {
    
	class SoraZipFile: public SoraFileBase {
	public:
		SoraZipFile();
		SoraZipFile(const SoraWString& filePath);
		~SoraZipFile();
        
        bool open(const SoraWString& filePath);
        bool open(void* lpszPackData, uLong nPackSize);
        void close();
        
        void setPackCode(const char* lpszPackCode);
        
        void* getFile(const SoraWString& filename, uLong readsize);
        
        ulong32 getFileSize(const SoraWString& filename);
        ulong32 getFileCRC(const SoraWString& filename);
		
        bool enumFiles(std::vector<SoraWString>& cont, const SoraWString& folder);
        
	private:
		void _releaseFile();
		void* _getfile(const SoraWString& filename, uLong readsize);
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