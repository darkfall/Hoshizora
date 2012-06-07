/*
 *  SoraZSorter.h
 *  Sora
 *
 *  Created by Robert Bu on 6/6/11.
 *  Copyright 2011 GameMaster Studio(Project Hoshizora). All rights reserved.
 *
 */

#ifndef SORA_ZSORTER_H_
#define SORA_ZSORTER_H_

#include "SoraPlatform.h"
#include "SoraVertex.h"
#include "SoraShader.h"

namespace sora {
	
	class SoraZSorter {
	public:
		static void renderQuad(SoraQuad& quad, SoraShaderContext* shader);
		static void renderWithVertices(SoraTextureHandle tex, int32 blendMode, SoraVertex* vertices, uint32 vsize, RenderMode mode, SoraShaderContext* shader);
		
		static void endSortAndRender();
	};
	
} // namespace sora


#endif // SORA_ZSORTER_H_