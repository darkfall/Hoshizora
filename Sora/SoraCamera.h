/*
 *  SoraCamera.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/20/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_CAMERA_H_
#define SORA_CAMERA_H_

#include "SoraPlatform.h"
#include "SoraObject.h"

#include "SoraImageEffect.h"

namespace sora {
	
	class SORA_API SoraCamera: public SoraObject {
	public:
		SoraCamera(float32 x, float32 y) {}
		~SoraCamera() {}
		
		void addTransform(SoraImageEffect* effect) {}
		
		void render() {}
		uint32 update(float32 dt) { return 0; }
		
	private:
		typedef std::list<SoraImageEffect*> TRANSFORM_LIST;
		TRANSFORM_LIST mTransforms;
	};
	
} // namespace sora

#endif // SORA_CAMERA_H_