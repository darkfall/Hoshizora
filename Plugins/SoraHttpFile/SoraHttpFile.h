#ifndef SORA_HTTP_FILE_H
#define SORA_HTTP_FILE_H

#include "SoraPlatform.h"
#include "SoraPThread.h"
#include "SoraFileBase.h"
#include "SoraStringConv.h"

#include "curl/curl.h"

#include "MyFileReader.h"


#include <sys/timeb.h>

namespace sora {

	enum SoraHttpDownloadState {
		DOWNLOAD_DOWNLOADING = 1,
		DOWNLOAD_NOTSTART = 2,
		DOWNLOAD_ERROR = 3,
		DOWNLOAD_FINISHED = 4,
	};

	typedef struct tagSoraHttpDownloadFile {
		ulong32 receivedSize;
		float32 receiveTime;
		int32 state;

		FILE* file;

		timeb starttime, endtime;

		tagSoraHttpDownloadFile(): receivedSize(0), state(DOWNLOAD_NOTSTART) {}
	} SoraHttpDownloadFile;
	
	typedef void (*SoraHttpCallback)(SoraHttpDownloadFile* pFile);

	typedef struct tagSoraHttpFileHead {
		SoraString sURL;
		SoraWString dest;

		SoraHttpCallback finish;
	} SoraHttpFileHead;

	class SoraHttpFileDownloadThread: public SoraThread {
	public:
		SoraHttpFileDownloadThread() { pFile = new SoraHttpDownloadFile; }
		~SoraHttpFileDownloadThread();

		void setup() { }
		void execute(void* parg);

		void getTime();

		int32 getState() { return pFile->state; }
		ulong32 getDownloadedSize() { return pFile->receivedSize; }
		float32 getDownloadTime() { getTime(); return pFile->receiveTime; }
		void suspend() { curl_easy_pause(easy_handle, CURLPAUSE_ALL); }
		void resume() { curl_easy_pause(easy_handle, CURLPAUSE_CONT); }
		// to do
		void stop() { }

		static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
			SoraHttpDownloadFile* phead = static_cast<SoraHttpDownloadFile*>(userp);
			ulong32 writesize = fwrite(buffer, size, nmemb, phead->file);
			phead->receivedSize += writesize;
			ftime(&phead->endtime);

			if(writesize != size*nmemb) phead->state = DOWNLOAD_ERROR;
			return writesize;
		}

	private:
		SoraHttpDownloadFile* pFile;
		SoraHttpFileHead* pHead;

		CURL *easy_handle;
	};

	typedef void (*SoraHttpDownloadCallback)(SoraHttpDownloadFile* pFile);

	class SoraHttpFile: public SoraFileBase {
		enum {
			DOWNLOAD_FILE_SET = 1,
			DOWNLOAD_FILE_NO = 0
		};
	public:
		SoraHttpFile(): state(DOWNLOAD_FILE_NO), finish(0) {}
		~SoraHttpFile();

		void setFile(const SoraWString& file);
		void setFinishCallback(SoraHttpDownloadCallback callback) { finish = callback; }

		int32 readFile(const SoraWString& url);
		int32 readFileMem(void* pstr, ulong32 filesize) { return 0; }
		void closeFile() { }

		ulong32 getDownloadedSize();
		float32 getDownloadTime();
		int32 getState();

		void suspend();
		void resume();

	private:
		SoraHttpFileDownloadThread* pdownloadthread;
		
		SoraWString downloadfile;
		SoraHttpDownloadCallback finish;

		int32 state;
	};

} // namespace sora

#endif