/*
 *  Sound.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef GCN_SOUND_H_
#define GCN_SOUND_H_

#include <string>

namespace gcn {
	
	class SoundLoader;
	/* 
	 * an abstract class represents a sound to use to GUIChan
	 */
	class Sound {
	public:
		Sound();
		virtual ~Sound();
		
		static void setSoundLoader(SoundLoader* sl);
		static SoundLoader* getSoundLoader();
		
		static Sound* load(const std::string& soundName);
		static Sound* load(const std::wstring& soundName);
		
		virtual void play() = 0;
		virtual void stop() = 0;
		
	protected:
		static SoundLoader* mSoundLoader;
	};

} // namespace gcn


#endif // GCN_SOUND_H_