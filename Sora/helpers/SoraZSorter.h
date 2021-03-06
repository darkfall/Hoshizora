/*
 *  SoraZSorter.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/6/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_ZSORTER_H_
#define SORA_ZSORTER_H_

#include "../SoraPlatform.h"
#include "../SoraSprite.h"

namespace sora {
	
	class SoraZSorter {
	public:
		static void renderQuad(SoraQuad& quad, SoraShaderContext* shader);
		static void renderTriple(SoraTriple& trip, SoraShaderContext* shader);
		static void renderWithVertices(HSORATEXTURE tex, int32 blendMode, SoraVertex* vertices, uint32 vsize, int32 mode, SoraShaderContext* shader);
		
		static void endSortAndRender();
	};
	
} // namespace sora


#endif // SORA_ZSORTER_H_