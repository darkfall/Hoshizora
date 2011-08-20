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

    SoraMusicFile* SoraAudiereSoundSystem::createMusicFile(const SoraWString& name, bool bStream) {
        SoraAudiereMusicFile* bgm = new SoraAudiereMusicFile(bStream);
		
		int32 result = bgm->readFile(name);
		if(result == SORASOUND_SUCCESS) {
			return bgm;
		}
		delete bgm;
		return 0;
    }
    
    SoraSoundEffectFile* SoraAudiereSoundSystem::createSoundEffectFile(const SoraWString& name) {
        SoraAudiereSoundEffectFile* seFile = new SoraAudiereSoundEffectFile;
		int32 result = seFile->readFile(name);
		if(result == SORASOUND_SUCCESS) {
			return seFile;
		}
		delete seFile;
		return 0;
    }
    
    SoraMusicFile* SoraAudiereSoundSystem::createMusicFile(bool bStream) {
        return new SoraAudiereMusicFile(bStream);
    }
    
    SoraSoundEffectFile* SoraAudiereSoundSystem::createSoundEffectFile() {
        return new SoraAudiereSoundEffectFile;
    }

#ifdef SORA_AUTOMATIC_REGISTER
	SORA_STATIC_RUN_CODE(SoraCore::Instance()->registerSoundSystem(new SoraAudiereSoundSystem));
#endif

} // namespace sora