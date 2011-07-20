/*
 *  soraiOSFileUtility.h
 *  Sora-iPhone
 *
 *  Created by GriffinBu on 1/28/11.
 *  Copyright 2011 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_IOS_FILE_UTILITY_H_
#define SORA_IOS_FILE_UTILITY_H_

#include "SoraPlatform.h"

namespace sora {
	SoraWString appResourcePath();
	SoraWString appApplicationPath();
	SoraWString appDocumentPath();
	
	bool appFileExists(const wchar_t* path);
} // namespace sora

#endif