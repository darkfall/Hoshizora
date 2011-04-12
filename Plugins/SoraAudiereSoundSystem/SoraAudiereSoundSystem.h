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
		
		SoraMusicFile* createMusicFile(const SoraWString& path, bool bStream=false);
        SoraSoundEffectFile* createSoundEffectFile(const SoraWString& path);
        SoraMusicFile* createMusicFile(bool bStream);
        SoraSoundEffectFile* createSoundEffectFile();
	};

} // namespace sora 


#endif