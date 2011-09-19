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
#include "SoraUncopyable.h"
#include "SoraString.h"

namespace sora {
	
	/**
	 *	a helper class that can release the resource file when it deallocs
	 *	for situation that the user may forget to free the resource data and cause memory leaks
	 *	this class can also request for a resource file directly
	 **/
	
	class SORA_API SoraResourceFileAuto: SoraUncopyable {
	public:
        SoraResourceFileAuto();
		explicit SoraResourceFileAuto(const StringType& file, bool retain=false);
		SoraResourceFileAuto(void* data, ulong32 size, bool retain=false);
        SoraResourceFileAuto(const StringType& file, ulong32 size, bool retain=false);
		~SoraResourceFileAuto();
        
        SoraResourceFileAuto& operator=(const StringType& file);
		
		void* getData() const;
		ulong32 getSize() const;
        
        void* data() const;
        ulong32 size() const;
		
		operator void*();
		operator char*();
		operator int*();
		operator long*();
		
		bool isValid() const;
		
	private:
		void* mData;
		ulong32 mSize;
		bool mRetain;
        
        SoraWString mName;
	};
	
} // namespace sora

#endif // SORA_RESOURCE_FILE_H_