/*
 *  SoraRenderTarget.h
 *  SoraPureCore
 *
 *  Created by griffin clare on 11/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef SORA_RENDER_TARGET_H_
#define SORA_RENDER_TARGET_H_

#include "SoraPlatform.h"

namespace sora {

class SoraRenderTarget {
public:
	SoraRenderTarget(ulong32 _w, ulong32 _h, bool _zbuffer):
	w(_w), h(_h), zbuffer(_zbuffer) {}
	
	virtual ~SoraRenderTarget() {}
	
	virtual void attachToRender() = 0;
	virtual void detachFromRender() = 0;
	
	int32 w, h;
	ulong32 tex;
	ulong32 stex;
	bool zbuffer;
	int32 err;
};

} // namespace sora

#endif