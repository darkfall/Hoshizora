/*
 *  rfMirror.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/22/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef RF_MIRROR_REV_H_
#define RF_MIRROR_REV_H_

#include "rfCommon.h"
#include "rfLightSource.h"

namespace reflection {
	
	/*
	 base class for mirror
	 can turn a certain degree depends on shape
	 and can reflect light
	 */
	class rfMirror: public rfLightSource {
	public:
		rfMirror();
		virtual ~rfMirror();
		
		virtual rfLight* reflect(rfLight* inLight);
		virtual rfLight* turn(float32 delta);
	
		void setVisited(bool flag);
		bool isVisited() const;
		
	protected:		
		bool bVisited;
		
		rfLight* pInLight;
		rfLight* pOutLight;
	};
	
} // namespace reflection

#endif // RF_MIRROR_REV_H_