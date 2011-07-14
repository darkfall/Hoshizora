#include "SoraZipFile.h"

namespace sora {

SoraZipFile::SoraZipFile() {
	pReader = 0;
	set_open(false);
}

SoraZipFile::SoraZipFile(const SoraWString& filePath) {
	pReader = new BOGY::PackReader;
	if(pReader) {
		if(pReader->Open(ws2s(filePath).c_str(), 0)) {
            set_open(true);
		}
	}
}

SoraZipFile::~SoraZipFile() {
	closeFile();
}

int32 SoraZipFile::readFile(const SoraWString& filePath) {
	if(is_open()) closeFile();
    mFilePath.clear();

	pReader = new BOGY::PackReader;
	if(pReader) {
		if(pReader->Open(ws2s(filePath).c_str(), 0)) {
            mFilePath = filePath;
            set_open(true);
			return 1;
		}
		closeFile();
		return 0;
	}
	return 0;
}

void SoraZipFile::setPackCode(const char* pstrCode) {
	packCode = pstrCode;
}

int32 SoraZipFile::readFileMem(void* lpszPackData, uLong nPackSize) {
	if(is_open()) closeFile();

	pReader = new BOGY::PackReader;
	if(pReader) {
		if(pReader->From(lpszPackData, nPackSize, packCode.size()>0?packCode.c_str():0)) {
            set_open(true);
			return 1;
		}
		closeFile();
		return 0;
	}
	return 0;
}

void SoraZipFile::closeFile() {
	if(is_open()) {
		if(pReader != 0) {
			pReader->Close();
			delete pReader;
            set_open(false);
		}
	}
}

void* SoraZipFile::getFile(const SoraWString& filename, uLong readSize) {
	if(is_open()) {
		if(readSize != 0) {
			return _getfile(filename, readSize);
		} else
			return 0;
	} else {
		return 0;
	}
	return 0;
}

void* SoraZipFile::_getfile(const SoraWString& filename, uLong readSize) {
	uint8* buffer = new uint8[readSize];
	memset(buffer, 0, readSize);
	if(buffer) {
        SoraWString rfilename = filename;
        size_t packpos = rfilename.find(mFilePath);
        if(packpos != SoraWString::npos && packpos == 0) {
            rfilename.erase(0, mFilePath.size()+1);
        }
		if(pReader->ReadFile(ws2s(filename).c_str(), buffer, readSize) == readSize) {
			return buffer;
		} else {
			delete buffer;
			return 0;
		}
	} else
		return 0;
}

uLong SoraZipFile::getFileSize(const SoraWString& filename) {
	if(is_open() && pReader) {
		return pReader->GetFileSize(ws2s(filename).c_str());
	}
	return 0;
}

uLong SoraZipFile::getFileCRC(const SoraWString& filename) {
	if(is_open() && pReader) {
		return pReader->GetFileCRC(ws2s(filename).c_str());
	}
	return 0;
}
	
	bool SoraZipFile::enumFiles(std::vector<SoraWString>& cont, const SoraWString& folder) {
		if(is_open() && pReader) {
			std::vector<std::string> tmpCont;
			pReader->GetFileList(tmpCont);
			for(size_t i=0; i<tmpCont.size(); ++i) {
				std::wstring tmpFile = s2ws(tmpCont[i]);
				if(tmpFile.find(folder) != std::wstring::npos)
					cont.push_back(tmpFile);
			}
			if(cont.size() != 0)
				return true;
		}
		return false;
	}

} // namespace sora