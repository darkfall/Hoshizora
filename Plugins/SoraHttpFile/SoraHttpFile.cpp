#include "SoraHttpFile.h"
#include "Debug/SoraInternalLogger.h"

namespace sora {
	
	SoraHttpFileDownloadThread::SoraHttpFileDownloadThread() { 
		pFile = new SoraHttpDownloadFile;
	}

	SoraHttpFileDownloadThread::~SoraHttpFileDownloadThread() {
		if(pFile) {
			delete pFile->buffer;
		}
		delete pFile;
	}
	
	int32 SoraHttpFileDownloadThread::getState() const { 
		return pFile->state;
	}
	
	ulong32 SoraHttpFileDownloadThread::getDownloadedSize() const { 
		return pFile->receivedSize;
	}
	
	float32 SoraHttpFileDownloadThread::getDownloadTime() { 
		getTime(); 
		return pFile->receiveTime; 
	}
	
	
	void SoraHttpFileDownloadThread::suspend() { 
		curl_easy_pause(easy_handle, CURLPAUSE_ALL);
	}
	
	void SoraHttpFileDownloadThread::resume() { 
		curl_easy_pause(easy_handle, CURLPAUSE_CONT);
	}
	
	SoraMemoryBuffer* SoraHttpFileDownloadThread::getMemoryBuffer() const {
		return pFile->buffer;
	}
	
	// to do
	void SoraHttpFileDownloadThread::stop() { }
	
	size_t SoraHttpFileDownloadThread::write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
		SoraHttpDownloadFile* phead = static_cast<SoraHttpDownloadFile*>(userp);
		phead->buffer->push(buffer, size*nmemb);
		phead->receivedSize += size*nmemb;
		ftime(&phead->endtime);
			
		return size*nmemb;
	}
	

	void SoraHttpFileDownloadThread::execute(void* parg) {
		easy_handle = curl_easy_init();
		if(!easy_handle)
			throw SORA_EXCEPTION("unable to alloc CURL handle");

		// record thread start time
		ftime(&pFile->starttime);
		
		pHead = static_cast<SoraHttpFileHead*>(parg);
		
		pFile->buffer = new SoraMemoryBuffer;
		//pFile->buffer->alloc(1);
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
		if(pHead->finish)
			pHead->finish(pHead->httpFile);
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

	bool SoraHttpFile::writeToFile(const SoraWString& file) {
		if(getState() == DOWNLOAD_FINISHED) {
			FILE* fp = sora_fopenw(file, "wb");
			if(fp) {
				fwrite(getMemoryBuffer()->get(), getMemoryBuffer()->realsize(), 1, fp);
				fclose(fp);
				return true;
			}
		}
		return false;
	}

	int32 SoraHttpFile::readFile(const SoraWString& url) {
		// to do
		if(isopen) return 0;

		pdownloadthread = new SoraHttpFileDownloadThread;
		SoraHttpFileHead* phead = new SoraHttpFileHead;
		phead->sURL = ws2s(url);
		phead->finish = finish;
		phead->httpFile = this;
		pdownloadthread->start(phead);

		isopen = true;
			
		return 1;
	}

	ulong32 SoraHttpFile::getDownloadedSize() const { 
		if(isopen) return pdownloadthread->getDownloadedSize();
		return 0;
	} 
	
	float32 SoraHttpFile::getDownloadTime() const { 
		if(isopen) return pdownloadthread->getDownloadTime();
		return 0.f;
	}
	
	int32 SoraHttpFile::getState() const { 
		if(isopen) return pdownloadthread->getState();
		return 0;
	}

	bool SoraHttpFile::isFinished() const {
		if(isopen)
			return pdownloadthread->getState() == DOWNLOAD_FINISHED;
		return false;
	}

	void SoraHttpFile::suspend() { 
		if(isopen) pdownloadthread->suspend();
	}
	void SoraHttpFile::resume() { 
		if(isopen) pdownloadthread->resume();
	}
	
	SoraMemoryBuffer* SoraHttpFile::getMemoryBuffer() const {
		if(isopen) return pdownloadthread->getMemoryBuffer();
		return NULL;
	}
	

} // namespace sora