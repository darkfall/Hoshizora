/*
 *  audioWrapper.cpp
 *  Sora
 *
 *  Created by griffin clare on 11/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "audioWrapper.h"

#include <QTKit/QTKit.h>

namespace sora {

class QTAudioFile {
public:
	QTAudioFile(): next(NULL), prev(NULL), audioFile(NULL) {}
	~QTAudioFile() {
		if(audioFile) {
			[audioFile release];
		}
	}
	
	ulong32 load(void* data, ulong32 length) {
		NSData* adata = [[NSData alloc] initWithBytes:(const char*)data length:(NSUInteger)length];
		if(!audioFile) {
			NSError* err = [NSError alloc];
			audioFile = [[QTMovie alloc] initWithData:adata error:&err];
			if(audioFile) {
				[adata release];
				
				return (ulong32)this;
			} else {
				printf("%s\n", [[err localizedDescription] UTF8String]);
			}
		}
		return 0;
	}
	
	ulong32 load(const SoraWString& file) {
		std::string sfile = ws2s(file);
		NSString* nsfile = [[NSString alloc] initWithUTF8String:sfile.c_str()];
		if(!audioFile) {
			NSError* err = [NSError alloc];
			audioFile = [[QTMovie alloc] initWithFile:nsfile error:&err];
			if(audioFile) {
				[nsfile release];
				
				return (ulong32)this;
			} else {
				printf("%s\n", [[err localizedDescription] UTF8String]);
			}
		}		
		return 0;
	}
	
	bool play() {
		[audioFile play];
		return true;
	}
	
	void pause() {
		[audioFile stop];
	}
	
	void stop() {
		[audioFile stop];
	}
		
	
	QTAudioFile* next;
	QTAudioFile* prev;
	
	QTMovie* audioFile;
};

	QTAudioFile* audioFiles = NULL;

	ulong32 openAudioFile(void* data, ulong32 length) {
		QTAudioFile* qtfile = new QTAudioFile;
		ulong32 result = qtfile->load(data, length);
	
		if(result) {
			if(!audioFiles) audioFiles = qtfile;
			else {
				audioFiles->next = qtfile;
				qtfile->prev = audioFiles;
				audioFiles = qtfile;
			}
			return result;
		} 
		return 0;
	}
	
	ulong32 openAudioFile(const SoraWString& file) {
		QTAudioFile* qtfile = new QTAudioFile;
		ulong32 result = qtfile->load(file);
		
		if(result) {
			if(!audioFiles) audioFiles = qtfile;
			else {
				audioFiles->next = qtfile;
				qtfile->prev = audioFiles;
				audioFiles = qtfile;
			}
			return result;
		} 
		return 0;
	}

	bool playAudioFile(ulong32 handle) {
		QTAudioFile* cfile = audioFiles;
		if(handle == (ulong32)cfile) {
			return cfile->play();
		}
		
		while(cfile->prev != 0) {
			if(handle == (ulong32)cfile) {
				return cfile->play();
			}
			cfile = cfile->prev;
		}
		return false;
	}
	
	bool pauseAudioFile(ulong32 handle) {
		QTAudioFile* cfile = audioFiles;
		if(handle == (ulong32)cfile) {
			cfile->pause();
			return true;
		}
		
		while(cfile->prev != 0) {
			if(handle == (ulong32)cfile) {
				cfile->pause();
				return true;
			}
			cfile = cfile->prev;
		}
		return false;
	}
	void stopAudioFile(ulong32 handle) {
		QTAudioFile* cfile = audioFiles;
		if(handle == (ulong32)cfile) {
			return cfile->stop();
		}
		
		while(cfile->prev != 0) {
			if(handle == (ulong32)cfile) {
				return cfile->pause();
			}
			cfile = cfile->prev;
		}
	}
	
} // namespace sora
