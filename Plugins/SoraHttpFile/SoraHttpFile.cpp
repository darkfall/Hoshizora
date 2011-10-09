#include "SoraHttpFile.h"
#include "Debug/SoraInternalLogger.h"

namespace sora {
	
	SoraHttpFileDownloadThread::SoraHttpFileDownloadThread() { 
		pFile = NULL;
		easy_handle = NULL;
        
        downloadTask.setAsMemberFunc(&SoraHttpFileDownloadThread::execute, this);
	}

	SoraHttpFileDownloadThread::~SoraHttpFileDownloadThread() {

	}
	
	int32 SoraHttpFileDownloadThread::getState() const { 
		return pFile->state;
	}
	
	ulong32 SoraHttpFileDownloadThread::getDownloadedSize() const { 
		return pFile->receivedSize;
	}
	
	double SoraHttpFileDownloadThread::getDownloadTime() const{ 
		return pFile->receiveTime; 
	}
		
	double SoraHttpFileDownloadThread::getDownloadSpeed() const {
		return pFile->downloadSpeed;
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
	void SoraHttpFileDownloadThread::stop() {
        downloadThread.exit();
    }
	
	size_t SoraHttpFileDownloadThread::write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
		SoraHttpDownloadFile* phead = static_cast<SoraHttpDownloadFile*>(userp);
		phead->buffer->push(buffer, size*nmemb);
		phead->receivedSize += size*nmemb;
			
		return size*nmemb;
	}
	
	std::string SoraHttpFileDownloadThread::getURL() const {
		return pHead->sURL;
	}
	
    void SoraHttpFileDownloadThread::startDownload(void* parg) {
        downloadTask.setArg(parg);
      
        setup();
        downloadThread.startWithTask(downloadTask);
    }
    
	void SoraHttpFileDownloadThread::setup() {
		if(!pFile)
			pFile = new SoraHttpDownloadFile;
		if(!easy_handle) {
			easy_handle = curl_easy_init();
			
            sora_assert(easy_handle);
		}
	}

	void SoraHttpFileDownloadThread::execute(void* parg) {        
		pHead = static_cast<SoraHttpFileHead*>(parg);
		
		pFile->buffer = new SoraMemoryBuffer;
		pFile->state = DOWNLOAD_DOWNLOADING;
		
		curl_easy_setopt(easy_handle, CURLOPT_URL, pHead->sURL.c_str());
		curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, pFile);
		CURLcode result = curl_easy_perform(easy_handle);
		if(result != CURLE_OK) {
			log_mssg(vamssg("Unable to perform CURL at url %s, error code %d", pHead->sURL.c_str(), result), 
						  LOG_LEVEL_ERROR);
		}
		
		curl_easy_getinfo(easy_handle, CURLINFO_SPEED_DOWNLOAD, &pFile->downloadSpeed);
		curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &pFile->receiveTime);

		pFile->state = DOWNLOAD_FINISHED;
		
		end();
		cleanup();
	}
	
	void SoraHttpFileDownloadThread::cleanup() {
		curl_easy_cleanup(easy_handle);
		if(pFile) {
			delete pFile->buffer;
			delete pFile;
		}
	}
	
	void SoraHttpFileDownloadThread::end() {
        SoraHttpFile* hfile = pHead->httpFile;

		if(hfile->getDelegate() != NULL)
			hfile->getDelegate()->notify(pHead->httpFile, *pHead->httpFile);
		
		if(hfile->getEventHandler() != NULL) {
			SoraHttpDownloadEvent event;
			event.setReceivedSize(pFile->receivedSize);
			event.setReceiveTime(pFile->receiveTime);
			event.setURL(pHead->sURL);
			event.setMemoryBuffer(pFile->buffer);
			event.setDownloadSpeed(pFile->downloadSpeed);
			
			hfile->getEventHandler()->handleEvent(&event);
		}
		
		if(hfile->isDownloadToFile()) {
			hfile->writeToFile(hfile->getDownloadToFile());
		}
		
		hfile->finishDownload();
	}
	
	SoraHttpFile::SoraHttpFile(): 
	mDownloadTo(false),
	isopen(false),
    mDelegate(NULL),
    mEventHandler(NULL) {
	}
	
	SoraHttpFile::SoraHttpFile(const SoraString& url, SoraEventHandler* handler) {
		mEventHandler = handler;
		downloadFile(url);
        mDelegate = NULL;
	}
	
	SoraHttpFile::SoraHttpFile(const SoraString& url, const SoraAbstractDelegate<SoraHttpFile>& del) {
		mDelegate = del.clone();
		downloadFile(url);
        mEventHandler = NULL;
	}

	SoraHttpFile::~SoraHttpFile() {
		if(mDelegate)
            delete mDelegate;
	}
	
	void SoraHttpFile::finishDownload() {
		isopen = false;
	}

	bool SoraHttpFile::writeToFile(const SoraWString& file) {
		if(getState() == DOWNLOAD_FINISHED) {
			FILE* fp = sora_fopenw(file.c_str(), "wb");
			if(fp) {
				fwrite(getMemoryBuffer()->get(), 1, getMemoryBuffer()->realsize(), fp);
				fclose(fp);
				return true;
			}
		}
		return false;
	}

	bool SoraHttpFile::downloadFile(const SoraString& url) {
		// to do
		if(isopen) return false;

		phead.sURL = url;
		phead.httpFile = this;
		pdownloadthread.startDownload(&phead);
		
		log_mssg(vamssg("Start download file from %s", phead.sURL.c_str()), 
					  LOG_LEVEL_NORMAL);

		isopen = true;
			
		return true;
	}
	
	bool SoraHttpFile::downloadFileTo(const SoraString& url, const SoraWString& file) {
		if(isopen) return false;
		
		phead.sURL = url;
		phead.httpFile = this;
		pdownloadthread.startDownload(&phead);
		
		log_mssg(vamssg("Start download file from %s, to %s", phead.sURL.c_str(), ws2s(file).c_str()), 
					  LOG_LEVEL_NORMAL);
		
		isopen = true;
		setDownloadToFile(file);
		return true;
	}
    
    SoraAbstractDelegate<SoraHttpFile>* SoraHttpFile::getDelegate() {
        return mDelegate;
    }
							   
	void SoraHttpFile::setDownloadToFile(const SoraWString& file) {
		mDownloadToFile = file;
		mDownloadTo = true;
	}						   
	
	void SoraHttpFile::setEventHandler(SoraEventHandler* handler) {
		mEventHandler = handler;
	}
	
	SoraEventHandler* SoraHttpFile::getEventHandler() const {
		return mEventHandler;
	}
	
	bool SoraHttpFile::isDownloadToFile() const {
		return mDownloadTo;
	}
	
	SoraWString SoraHttpFile::getDownloadToFile() const {
		return mDownloadToFile;
	}
	
	void SoraHttpFile::setDelegate(const SoraAbstractDelegate<SoraHttpFile>& del) { 
		mDelegate = del.clone();
	}

	ulong32 SoraHttpFile::getDownloadedSize() const { 
		if(isopen) return pdownloadthread.getDownloadedSize();
		return 0;
	} 
    
    bool SoraHttpFile::downloadFileWithDelegate(const SoraString& url, const SoraAbstractDelegate<SoraHttpFile>& del) {
        setDelegate(del);
        return downloadFile(url);
    }
	
	double SoraHttpFile::getRemoteFileSize(const SoraString& url) {
		CURL* handle;
		handle = curl_easy_init();
		curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
		curl_easy_setopt(handle, CURLOPT_HEADER, false);
		curl_easy_setopt(handle, CURLOPT_NOBODY, true);
		
		CURLcode result = curl_easy_perform(handle);
		if(result == CURLE_OK) {
			double size;
			result = curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &size);
			if(result == CURLE_OK) {
				return size;
			}
		}
		if(result != CURLE_OK) {
			log_mssg(vamssg("Unable to perform curl at url %s, error code: %d", url.c_str(), result));
		}
		return 0.0;
	}
	
	double SoraHttpFile::getDownloadTime() const { 
		if(isopen) return pdownloadthread.getDownloadTime();
		return 0.0;
	}
	
	double SoraHttpFile::getDownloadSpeed() const {
		if(isopen) return pdownloadthread.getDownloadSpeed();
		return 0.0;
	}
	
	int32 SoraHttpFile::getState() const { 
		if(isopen) return pdownloadthread.getState();
		return 0;
	}

	bool SoraHttpFile::isFinished() const {
		if(isopen)
			return pdownloadthread.getState() == DOWNLOAD_FINISHED;
		return false;
	}

	void SoraHttpFile::suspend() { 
		if(isopen) pdownloadthread.suspend();
	}
	void SoraHttpFile::resume() { 
		if(isopen) pdownloadthread.resume();
	}
	
	SoraMemoryBuffer* SoraHttpFile::getMemoryBuffer() const {
		if(isopen) return pdownloadthread.getMemoryBuffer();
		return NULL;
	}
	
	std::string SoraHttpFile::getURL() const {
		if(isopen) return pdownloadthread.getURL();
		return "";
	}

} // namespace sora