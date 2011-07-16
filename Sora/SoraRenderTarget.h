/*
 *  SoraRenderTarget.h
 *  SoraPureCore
 *
 *  Created by griffin clare on 11/19/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */
#ifndef SORA_RENDER_TARGET_H_
#define SORA_RENDER_TARGET_H_

#include "SoraPlatform.h"

namespace sora {

class SORA_API SoraRenderTarget {
public:
	SoraRenderTarget(ulong32 _w, ulong32 _h, bool _zbuffer):
	w(_w), h(_h), zbuffer(_zbuffer) {}
	
	virtual ~SoraRenderTarget() {}
	
	virtual void attachToRender() = 0;
	virtual void detachFromRender() = 0;
	
    int32 getWidth() const  { return w; }
    int32 getHeight() const { return h; }
    int32 getError() const  { return err; }
    bool  isZBuffer() const { return zbuffer; }
    
private:
	int32 w, h;
	ulong32 tex;
	bool zbuffer;
	int32 err;
};

} // namespace sora

#endif