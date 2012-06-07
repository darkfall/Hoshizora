/*
 *  SoraFrameListener.h
 *  Sora
 *
 *  Created by Robert Bu on 1/23/11.
 *  Copyright 2011 GameMaster Studio(Project Hoshizora). All rights reserved.
 *
 */

#ifndef SORA_FRAME_LISTENER_H_
#define SORA_FRAME_LISTENER_H_

namespace sora {
	
	class SoraFrameListener {
	public:
		virtual void onFrameStart() = 0;
		virtual void onFrameEnd() = 0;
	};
	
} // namespace sora

#endif