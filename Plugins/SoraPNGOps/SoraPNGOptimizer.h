/*
 *  SoraPNGOptimizer.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_PNG_OPTIMIZER_H_
#define SORA_PNG_OPTIMIZER_H_

#include "SoraCore.h"

namespace sora {
	
	class SoraPNGOptimizer {
	public:
		static bool optimizePNGTex(SoraTextureHandle tex);
		static void optimizePNGData(uint32* data, int32 width, int32 height, int32 pitch);
		
		static bool optimizePNGFromAndWriteToFile(const SoraWString& file, const wchar_t* output);
		
		static bool optimizeTexAndWriteToFile(SoraTextureHandle tex, const SoraWString& output);
		static bool writeTexToFile(SoraTextureHandle tex, const SoraWString& output);
	};
	
}


#endif // SORA_PNG_OPTIMIZER_H_