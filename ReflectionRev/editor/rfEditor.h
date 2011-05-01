/*
 *  rfEditor.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/29/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RF_EDITOR_H_
#define RF_EDITOR_H_

#include "../rfMap.h"
#include "../rfShape.h"
#include "SoraFont.h"

namespace reflection {
	
	class rfEditor {
	public:
		rfEditor();
		~rfEditor();
		
		void init();
		
		rfMap* getMap() const { return map; }
		
		void renderInfo();
		
	private:
		rfMap* map;
		
		sora::SoraFont* pFont;
	};
	
} // namespace reflection


#endif // RF_EDITOR_H_