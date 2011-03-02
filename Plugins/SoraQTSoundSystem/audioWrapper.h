/*
 *  audioWrapper.h
 *  Sora
 *
 *  Created by griffin clare on 11/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AUDIO_WRAPPER_H_
#define AUDIO_WRAPPER_H_

#include "SoraPlatform.h"
#include "SoraStringConv.h"

namespace sora {

	ulong32 openAudioFile(void* data, ulong32 length);
	ulong32 openAudioFile(const SoraWString& file);
	bool	playAudioFile(ulong32 handle);
	bool	pauseAudioFile(ulong32 handle);
	void	stopAudioFile(ulong32 handle);
	
} // namespace sora

#endif