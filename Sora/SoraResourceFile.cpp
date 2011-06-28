/*
 *  SoraResourceFile.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/28/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraResourceFile.h"
#include "SoraCore.h"

namespace sora {

	SoraResourceFileAuto::SoraResourceFileAuto(void* data, ulong32 size, bool retain):
	mData(data),
	mSize(size),
	mRetain(retain) {
	}
	
	SoraResourceFileAuto::SoraResourceFileAuto(const SoraWString& file, bool retain):
	mRetain(retain) {
		mData = SORA->getResourceFile(file, mSize);
		if(!mData)
			THROW_SORA_EXCEPTION(vamssg("Error loading resource file: %s", ws2s(file).c_str()));
	}
	
	SoraResourceFileAuto::~SoraResourceFileAuto() {
		if(mData && !mRetain) {
			SORA->freeResourceFile(mData);
		}
	}
	
	void* SoraResourceFileAuto::getData() const {
		return mData;
	}
	
	ulong32 SoraResourceFileAuto::getSize() const {
		return mSize;
	}
	
	bool SoraResourceFileAuto::isValid() const {
		return mData!=NULL;
	}
	
	SoraResourceFileAuto::operator void*() {
		return mData;
	}
	
	SoraResourceFileAuto::operator unsigned long*() {
		return static_cast<unsigned long*>(mData);
	}
	
	SoraResourceFileAuto::operator unsigned int*() {
		return static_cast<unsigned int*>(mData);
	}
	
	SoraResourceFileAuto::operator unsigned char*() {
		return static_cast<unsigned char*>(mData);
	}
	
	SoraResourceFileAuto::operator char*() {
		return static_cast<char*>(mData);
	}
	
	SoraResourceFileAuto::operator int*() {
		return static_cast<int*>(mData);
	}
	
	SoraResourceFileAuto::operator long*() {
		return static_cast<long*>(mData);
	}
	
} // namespace sora