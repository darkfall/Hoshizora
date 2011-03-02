#include "SoraHttpFile.h"

namespace sora {

	SoraHttpFileDownloadThread::~SoraHttpFileDownloadThread() {
		if(pFile) {
			fclose(pFile->file);
		}
		delete pFile;
	}

	void SoraHttpFileDownloadThread::execute(void* parg) {
		easy_handle = curl_easy_init();
		if(!easy_handle)
			throw SORA_EXCEPTION("unable to alloc CURL handle");

				// record thread start time
		ftime(&pFile->starttime);
		
		pHead = static_cast<SoraHttpFileHead*>(parg);
		pFile->file = fopen(SoraStringConverter::wideString2String(pHead->dest).c_str(), "wb");
		pFile->state = DOWNLOAD_DOWNLOADING;
		curl_easy_setopt(easy_handle, CURLOPT_URL, pHead->sURL.c_str());
		curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, pFile);
		CURLcode result = curl_easy_perform(easy_handle);
		if(result != CURLE_OK) {
			throw SORA_EXCEPTION("unable to perform CURL");
		}

		ftime(&pFile->endtime);

		getTime();
		pFile->state = DOWNLOAD_FINISHED;
		if(pHead->finish) pHead->finish(pFile);
		curl_easy_cleanup(easy_handle);
	}

	void SoraHttpFileDownloadThread::getTime() {
		// getdownload time
		pFile->receiveTime = (float)pFile->endtime.time - pFile->starttime.time;
		int32 mil = pFile->endtime.millitm - pFile->starttime.millitm;
		if(mil < 0) {
			pFile->receiveTime -= 1; 
			pFile->receiveTime += (float)(1000 + mil)/1000.f;
		} else pFile->receiveTime += (float)mil/1000.f;
	}

	SoraHttpFile::~SoraHttpFile() {
		if(pdownloadthread) {
			delete pdownloadthread;
		}
	}

	void SoraHttpFile::setFile(const SoraWString& file) {
		downloadfile = file;
		state = DOWNLOAD_FILE_SET;
	}

	int32 SoraHttpFile::readFile(const SoraWString& url) {
		if(state == DOWNLOAD_FILE_NO) return 0;
		// to do
		if(isopen) return 0;

		pdownloadthread = new SoraHttpFileDownloadThread;
		SoraHttpFileHead* phead = new SoraHttpFileHead;
		phead->sURL = SoraStringConverter::wideString2String(url);
		phead->finish = finish;
		phead->dest = downloadfile;
		pdownloadthread->start(phead);

		isopen = true;
			
		return 1;
	}

	ulong32 SoraHttpFile::getDownloadedSize() { 
		if(isopen) return pdownloadthread->getDownloadedSize();
		return 0;
	} 
	float32 SoraHttpFile::getDownloadTime() { 
		if(isopen) return pdownloadthread->getDownloadTime();
		return 0.f;
	}
	int32 SoraHttpFile::getState() { 
		if(isopen) return pdownloadthread->getState();
		return 0;
	}

	void SoraHttpFile::suspend() { 
		if(isopen) pdownloadthread->suspend();
	}
	void SoraHttpFile::resume() { 
		if(isopen) pdownloadthread->resume();
	}

} // namespace sora