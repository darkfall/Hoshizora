/*
 *  SoraFileChangeEvent.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_FILE_CHANGE_EVENT_H_
#define SORA_FILE_CHANGE_EVENT_H_

#include "SoraEvent.h"

namespace sora {
	
	class SoraFileChangeEvent: public SoraEvent {
	public:
		SoraFileChangeEvent(const SoraWString& file): mChangedFile(file) {}
		
		SoraWString getChangedFile() const {
			return mChangedFile;
		}
		
#ifndef SORA_USE_RTTI
		SORA_EVENT_IDENTIFIER(18446744073686077189ULL);
#endif
	
	private:
		SoraWString mChangedFile;
	};
	
	
} // namespace sora


#endif // SORA_FILE_CHANGE_EVENT_H_