/*
 *  SoraEnvHelper.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_ENV_HELPER_H_
#define SORA_ENV_HELPER_H_

#include "SoraPlatform.h"

namespace sora {
	
	class SoraEnvHelper {
	public:
		/*
		 @param subkey and @param mKey are for windows
		 they would not work under linux, but pass them the same as under windows is fine
		 */
		static bool writeEnv(const char* key, const char* val, const char* subkey, ulong32 mKey);
		
		static char* getEnv(const char* key, const char* subkey, ulong32 mKey);
		
		static bool delEnv(const char* key, ulong32 mKey);
	};
	
} // namespace sora

#endif // SORA_ENV_HELPER_H_