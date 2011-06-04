#ifndef SORA_HTTP_FILE_H
#define SORA_HTTP_FILE_H

#include "SoraPlatform.h"
#include "SoraFileBase.h"
#include "SoraStringConv.h"

#include "SoraMemoryBuffer.h"

#include "curl/curl.h"
#include "SoraPThread/SoraPThread.h"

#include <sys/timeb.h>

namespace sora {

	enum SoraHttpDownloadState {
		DOWNLOAD_DOWNLOADING	= 1,
		DOWNLOAD_NOTSTART		= 2,
		DOWNLOAD_ERROR			= 3,
		DOWNLOAD_FINISHED		= 4,
	};

	typedef struct tagSoraHttpDownloadFile {
		ulong32 receivedSize;
		float32 receiveTime;
		int32 state;

		SoraMemoryBuffer* buffer;

		timeb starttime, endtime;

		tagSoraHttpDownloadFile(): buffer(NULL), receivedSize(0), state(DOWNLOAD_NOTSTART) {}
	} SoraHttpDownloadFile;
	
	class SoraHttpFile;
	
	typedef void (*SoraHttpCallback)(SoraHttpFile* pFile);

	typedef struct tagSoraHttpFileHead {
		SoraString sURL;
		SoraHttpFile* httpFile;

		SoraHttpCallback finish;
	} SoraHttpFileHead;

	class SoraHttpFileDownloadThread: public SoraThread {
	public:
		SoraHttpFileDownloadThread();
		~SoraHttpFileDownloadThread();

		void setup() { }
		void execute(void* parg);

		void getTime();

		int32 getState() const ;
		ulong32 getDownloadedSize() const ;
		float32 getDownloadTime() ;
		void suspend();
		void resume();
		// to do
		void stop();
		
		SoraMemoryBuffer* getMemoryBuffer() const;

		static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

	private:
		SoraHttpDownloadFile* pFile;
		SoraHttpFileHead* pHead;

		CURL *easy_handle;
	};

	class SoraHttpFile: public SoraFileBase {
		enum {
			DOWNLOAD_FILE_SET = 1,
			DOWNLOAD_FILE_NO = 0
		};
	public:
		SoraHttpFile(): state(DOWNLOAD_FILE_NO), finish(0) {}
		~SoraHttpFile();

		bool writeToFile(const SoraWString& file);
		void setFinishCallback(SoraHttpCallback callback) { finish = callback; }

		int32 readFile(const SoraWString& url);
		int32 readFileMem(void* pstr, ulong32 filesize) { return 0; }
		void closeFile() { }
		
		SoraMemoryBuffer* getMemoryBuffer() const;
		
		ulong32 getDownloadedSize() const;
		float32 getDownloadTime() const;
		int32 getState() const;

		void suspend();
		void resume();
		
		bool isFinished() const;

	private:
		SoraHttpFileDownloadThread* pdownloadthread;
		SoraHttpCallback finish;

		int32 state;
	};

} // namespace sora

#endif