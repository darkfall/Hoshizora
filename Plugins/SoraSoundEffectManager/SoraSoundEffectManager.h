/*
 *  SoraSoundEffectManager.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_SOUND_EFFECT_MANAGER_H_
#define SORA_SOUND_EFFECT_MANAGER_H_

#include "SoraPlatform.h"
#include "SoraSoundFile.h"
#include "SoraSingleton.h"
#include "stringId.h"

namespace sora {
		
	typedef stringId SoundEffectId;
	/*
	 a global sound effect manager for fast and convinient sound effect playback
	 especially in scripts
	 */
	class SoraSoundEffectManager: public SoraSingleton<SoraSoundEffectManager> {
		friend class SoraSingleton<SoraSoundEffectManager>;
		
	protected:
		SoraSoundEffectManager();
		~SoraSoundEffectManager();
		
	public:
		
		/*
		 load a sound effect into the manager
		 if cannot load the effect, the @retval would be 0
		 */
		SoundEffectId load(const std::wstring& effectName);
		void unload(SoundEffectId);
				
		SoraSoundEffectFile* get(SoundEffectId) const;
		
		bool play(SoundEffectId effId);
		void stop(SoundEffectId effId);
		
		/* 
			play a sound effect directly by its name
			this sound effect would also be cached by the manager
		 */
		bool playDirect(const std::wstring& effectName);
		
		/* applies to global effects */
		/* 0 - 100 */
		void setVolume(float32 vol);
		float32 getVolume() const;
		
	private:
		typedef sora::hash_map<SoundEffectId, SoraSoundEffectFile*> SOUND_EFFECT_MAP;
		SOUND_EFFECT_MAP mSoundEffects;
		
		float32 effectVolume;
	};
	
	
} // namespace sora


#endif // SORA_SOUND_EFFECT_MANAGER_H_