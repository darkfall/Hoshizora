/*
 *  SoraOSXSoundSystem.h
 *  Sora
 *
 *  Created by griffin clare on 11/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef SORA_OSX_SOUND_SYSTEM_H_
#define SORA_OSX_SOUND_SYSTEM_H_

#include "SoraSoundSystem.h"

namespace sora {
	
	class SoraQTSoundSystem: public SoraSoundSystem {
	public:
		SoraQTSoundSystem() {}
		virtual ~SoraQTSoundSystem() {}
		
		int32 init() {}
		void shutdown() {}
		
		/* Music Interfaces */
		/*
		 open a music file to play
		 @param name, the name of the music to open
		 @param buffer, use buffer mode or not, if not, then try to load the music into memory for once
		 @retval, the handle of the music, if =0 then failed
		 */
		ulong32 openMusic(const SoraWString& name, bool buffer=true);
		/*
		 play a music
		 @param handle, the handle of the music, see openMusic
		 @param inqueue, use queue mode or not, if yes, then the music would be put into the end of bgm queue, otherwise would be played immediately
		 @retval, succeed or not
		 */
		bool playMusic(ulong32 handle, bool inqueue=false);
		/*
		 stop a music
		 @param handle, the handle of the music, see openMusic
		 */
		bool pauseMusic(ulong32 handle);
		/*
		 pause a music
		 @param handle, the handle of the music, see openMusc
		 */
		void stopMusic(ulong32 handle, bool tillend=false);
		/*
		 set volume
		 */
		void setMusicVolume(ulong32 handle, float32 vol);
		/*
		 get volume
		 */
		float32 getMusicVolume(ulong32 handle);
		/*
		 get the length of a music
		 @param handle, the handle of the music, see openMusic
		 @retval, the length of the music
		 */
		ulong32 getMusicLength(ulong32 handle);
		
		
		
		/*
		 open a soundeffect
		 @param name, the name of the soundeffect
		 @retval, then handle of the soundeffect, =0 failed
		 */
		ulong32 openSoundEffect(const SoraWString& name);
		/*
		 play a soundeffect
		 @param handle, then handle of the soundeffect, see openSoundEffect
		 @param loop, loop play the soundeffect or not
		 @retval, succeed or not
		 */
		bool playSoundEffect(ulong32 handle, bool loop=false);
		/*
		 stop a soundeffect(if it is looping)
		 @param handle, the handle of the soundeffect
		 */
		void stopSoundEffect(ulong32 handle);
		
		void setSoundEffectVolume(ulong32 handle, float32 vol);
		
		
		void update();
		
	};
} // namespace sora
	

#endif
