#ifndef SORA_AUDIERE_SOUNDSYSTEM_H
#define SORA_AUDIERE_SOUNDSYSTEM_H

#include "SoraSoundSystem.h"

#include <list>
#include <map>

namespace sora {

	class SoraAudiereSoundSystem: public SoraSoundSystem {
	public:
		SoraAudiereSoundSystem();
		virtual ~SoraAudiereSoundSystem();

		int32 init();
		void shutdown();

		void update();
		
		SoraMusicFile* createMusicFile(void* data, ulong32 size, bool bStream=false);
		SoraSoundEffectFile* createSoundEffectFile(void* data, ulong32 size);
	};

} // namespace sora 


#endif