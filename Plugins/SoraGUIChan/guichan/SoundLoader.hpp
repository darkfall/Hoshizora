/*
 *  SoundLoader.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef GCN_SOUND_LOADER_H_
#define GCN_SOUND_LOADER_H_

#include <string>

namespace gcn {
	
	class Sound;
    
    /* 
	 * Abstract sound loader that loads Sound
     * @see Sound
     *
     * @author Robert Bu(darkfall)
     * @since Guichan for Hoshizora
	 */
	
	class SoundLoader {
	public:
		virtual Sound* load(const std::string& soundName) = 0;
        virtual void play(const std::string& soundName) = 0;
    };
	
} // namespace gcn

#endif // GCN_SOUND_LOADER_H_