#ifndef SORA_HTTP_FILE_H
#define SORA_HTTP_FILE_H

#include "SoraPlatform.h"
#include "SoraFileBase.h"
#include "SoraStringConv.h"
#include "SoraEvent.h"

#include "SoraMemoryBuffer.h"

#include "curl/curl.h"
#include "SoraPThread/SoraPThread.h"

#ifdef OS_WIN32
#pragma comment(lib, "libcurl.lib")
#endif

#include <sys/timeb.h>

namespace sora {

	enum SoraHttpDownloadState {
		DOWNLOAD_DOWNLOADING	= 1,
		DOWNLOAD_NOTSTART		= 2,
		DOWNLOAD_ERROR			= 3,
		DOWNLOAD_FINISHED		= 4,
	};
	
	class SoraHttpDownloadEvent: public SoraEvent {
	public:
		SoraMemoryBuffer* getMemoryBuffer() const {
			return mFileBuffer;
		}
		
		void setMemoryBuffer(SoraMemoryBuffer* buffer) {
			mFileBuffer = buffer;
		}
		
		ulong32 getReceivedSize() const {
			return mReceivedSize;
		}
		
		void setReceivedSize(ulong32 size) {
			mReceivedSize = size;
		}
		
		double getReceiveTime() const {
			return mReceiveTime;
		}
		
		void setReceiveTime(double time) {
			mReceiveTime = time;
		}
		
		SoraString getURL() const {
			return mURL;
		}
		
		void setURL(const SoraString& url) {
			mURL = url;
		}
		
		void setDownloadSpeed(double speed) {
			mDownloadSpeed = speed;
		}
		
		double getDownloadSpeed() const {
			return mDownloadSpeed;
		}
		
#ifndef SORA_USE_RTTI
		SORA_EVENT_IDENTIFIER(18446744070430258672ULL);
#endif
		
	private:
		SoraMemoryBuffer* mFileBuffer;
		
		ulong32 mReceivedSize;
		double mReceiveTime;
		double mDownloadSpeed;
		
		SoraString mURL;
	};

	typedef struct tagSoraHttpDownloadFile {
		ulong32 receivedSize;
		double receiveTime;
		double downloadSpeed;
		int32 state;

		SoraMemoryBuffer* buffer;
		
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

		void setup();
		void execute(void* parg);
		
		void end();
		void cleanup();

		int32 getState() const ;
		ulong32 getDownloadedSize() const ;
		double getDownloadTime() const;
		double getDownloadSpeed() const;
		
		void suspend();
		void resume();
		// to do
		void stop();
		
		SoraString getURL() const;
		
		SoraMemoryBuffer* getMemoryBuffer() const;

		static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

	private:
		SoraHttpDownloadFile* pFile;
		SoraHttpFileHead* pHead;

		CURL *easy_handle;
	};

	class SoraHttpFile: public SoraEventHandler {
		friend class SoraHttpFileDownloadThread;
		
	public:
		SoraHttpFile();
		SoraHttpFile(const SoraString& url, SoraEventHandler* handler);
		SoraHttpFile(const SoraString& url, SoraHttpCallback callback);
		~SoraHttpFile();

		bool downloadFile(const SoraString& url);
		bool downloadFileTo(const SoraString& url, const SoraWString& to);
		
		void setDownloadToFile(const SoraWString& file);
		
		void setEventHandler(SoraEventHandler* handler);
		SoraEventHandler* getEventHandler() const;
		
		bool writeToFile(const SoraWString& file);
		void setFinishCallback(SoraHttpCallback callback);
		
		static double getRemoteFileSize(const SoraString& url);
		
		SoraMemoryBuffer* getMemoryBuffer() const;
		
		ulong32		getDownloadedSize() const;
		double		getDownloadTime() const;
		double		getDownloadSpeed() const;
		int32		getState() const;
		SoraString	getURL() const;

		void suspend();
		void resume();
		
		bool isFinished() const;
		
		bool isDownloadToFile() const;
		SoraWString getDownloadToFile() const;

	private:
		void finishDownload();
		
		bool isopen;
		bool mDownloadTo;
		SoraWString mDownloadToFile;
		
		SoraHttpFileDownloadThread pdownloadthread;
		SoraHttpCallback finishCallback;
		SoraHttpFileHead phead;
		
		SoraEventHandler* mEventHandler;
	};

} // namespace sora

#endif