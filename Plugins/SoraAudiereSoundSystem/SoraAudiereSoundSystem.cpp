#include "SoraAudiereSoundSystem.h"

#include "SoraAudiereDevice.h"

#include "SoraCore.h"

#include "hash.h"
#include "SoraAudiereSoundFile.h"


namespace sora {

	SoraAudiereSoundSystem::SoraAudiereSoundSystem() {
		init();
	}

	SoraAudiereSoundSystem::~SoraAudiereSoundSystem() {
		//shutdown();
	}

	int32 SoraAudiereSoundSystem::init() {
		return SoraAudiereDevice::Instance()->setupDevice(0, 0);
	}

	void SoraAudiereSoundSystem::shutdown() {
		SoraAudiereDevice::Instance()->Destroy();
	}

	void SoraAudiereSoundSystem::update() {
		SoraAudiereDevice::Instance()->update();
	}

	SoraMusicFile* createMusicFile(void* data, ulong32 size, bool bStream=false) {
		SoraAudiereMusicFile* bgm = new SoraAudiereMusicFile;
		
		int32 result = bgm->readFile(name);
		if(result == SORASOUND_SUCCESS) {
			return bgm;
		}
		delete bgm;
		return 0;
	}
		
	SoraSoundEffectFile* createSoundEffectFile(void* data, ulong32 size) {
		SoraAudiereSoundEffectFile* seFile = new SoraAudiereSoundEffectFile;
		int32 result = seFile->readFile(name);
		if(result == SORASOUND_SUCCESS) {
			return seFile;
		}
		delete seFile;
		return 0;
	}

} // namespace sora