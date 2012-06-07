/*
 *  SoraOSXFileUtility.h
 *  Sora
 *
 *  Created by Robert Bu on 1/12/11.
 *  Copyright 2011 GameMaster Studio(Project Hoshizora). All rights reserved.
 *
 */

#ifndef SORA_OSX_FILE_UTILITY_H_
#define SORA_OSX_FILE_UTILITY_H_

#include "SoraPlatform.h"
#include "SoraString.h"

namespace sora {
    StringType osxApplicationPath();
	bool osxFileExists(const StringType& path);
} // namespace sora

#endif