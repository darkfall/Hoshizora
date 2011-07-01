/*
 *  SoraResourceFile.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/28/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_RESOURCE_FILE_H_
#define SORA_RESOURCE_FILE_H_

#include "SoraPlatform.h"

namespace sora {
	
	/**
	 *	a helper class that can release the resource file when it deallocs
	 *	for situation that the user may forget to free the resource data and cause memory leaks
	 *	this class can also request for a resource file directly
	 **/
	
	class SoraResourceFileAuto {
	public:
		SoraResourceFileAuto(void* data, ulong32 size, bool retain=false);
		SoraResourceFileAuto(const SoraWString& file, bool retain=false);
        SoraResourceFileAuto(const SoraWString& file, ulong32 size, bool retain=false);
		~SoraResourceFileAuto();
		
		void* getData() const;
		ulong32 getSize() const;
		
		operator void*();
		operator char*();
		operator unsigned int*();
		operator unsigned long*();
		operator unsigned char*();
		operator int*();
		operator long*();
		
		bool isValid() const;
		
	private:
		void* mData;
		ulong32 mSize;
		bool mRetain;
	};
	
} // namespace sora

#endif // SORA_RESOURCE_FILE_H_