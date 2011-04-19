#include "SoraAudiereSoundFile.h"

#include "SoraCore.h"
#include "SoraAudiereDevice.h"

namespace sora {

		int32 SoraAudiereMusicFile::readFile(const SoraWString& sFilePath) {
			if(is_open()) closeFile();
            
            _parseName(sFilePath);

			AudioDevicePtr device = SoraAudiereDevice::Instance()->getDevice();
			if(!device) { return SORASOUND_FAILED_OPEN_DEVICE; }
			ulong32 size;
			void* ptr = SoraCore::Instance()->getResourceFile(sFilePath, size);
			if(ptr) {
				File* pSoundFile = CreateMemoryFile(ptr, (int)size);
				if(!pSoundFile) { return SORASOUND_FAILED_OPEN_MEMORY_FILE; }
				sound = OpenSound(device, pSoundFile, bIsStream);
				if(!sound) { return SORASOUND_FAILED_OPEN_SOUND; }
				set_open(true);
				SORA->freeResourceFile(ptr);
				return SORASOUND_SUCCESS;
			}

			return SORASOUND_FAILED_OPEN_SOUND;
		}
    
		int32 SoraAudiereMusicFile::readFileMem(void* ptr, ulong32 size) {
			if(is_open()) closeFile();

			AudioDevicePtr device = SoraAudiereDevice::Instance()->getDevice();
			if(!device) { return SORASOUND_FAILED_OPEN_DEVICE; }
			File* pSoundFile = CreateMemoryFile(ptr, (int)size);
			if(!pSoundFile) { return SORASOUND_FAILED_OPEN_MEMORY_FILE; }
			sound = OpenSound(device, pSoundFile, bIsStream);
			if(!sound) { return SORASOUND_FAILED_OPEN_SOUND; }
			set_open(true);
			return SORASOUND_SUCCESS;
		}
    
		void SoraAudiereMusicFile::closeFile() {
			if(is_open()) {
				//sound->unref();
				sound = NULL;
				set_open(false);
			}
		}

		void SoraAudiereMusicFile::play() { 
			if(is_open()) sound->play();
		}
		void SoraAudiereMusicFile::stop() { 
			if(is_open()) sound->stop();
		}
		void SoraAudiereMusicFile::pause() {
			if(is_open()) {
				pausePos = getMusicPosition();
				stop();
			}
		}
		void SoraAudiereMusicFile::resume() {
			if(is_open()) {
				setMusicPosition(pausePos);
				play();
			}
		}

		void SoraAudiereMusicFile::setVolume(float32 vol) { 
			if(is_open()) sound->setVolume(vol);
		}

		void SoraAudiereMusicFile::setPan(float32 pan) { 
			if(is_open()) sound->setPan(pan);
		}

		void SoraAudiereMusicFile::setPitch(float32 pitch) { 
			if(is_open()) sound->setPitchShift(pitch);
		}

		float32 SoraAudiereMusicFile::getPan() const { 
			if(is_open()) return sound->getPan();
			return 0.f;
		}
		float32 SoraAudiereMusicFile::getPitch() const { 
			if(is_open()) return sound->getPitchShift();
			return 0.f;
		}

		float32 SoraAudiereMusicFile::getVolume() const {
			if(is_open()) return sound->getVolume();
			return 0.f;
		}

		void SoraAudiereMusicFile::setPosition(float32 x, float32 y, float32 z) { 
//			if(is_open()) sound->setPosition(pos);
		}
		void SoraAudiereMusicFile::getPosition(float32& x, float32& y, float32& z) { 
			x = y = z = 0.f;
	//		if(is_open()) return sound->getPosition();
		//	return 0;
		}
    
    void SoraAudiereMusicFile::setMusicPosition(int32 pos) {
        if(is_open()) sound->setPosition(pos);
    }
    
    int32 SoraAudiereMusicFile::getMusicPosition() const {
        if(is_open()) return sound->getPosition();
        return 0;
    }

		int32 SoraAudiereMusicFile::getLength() const { 
			if(is_open()) return sound->getLength();
			return 0;
		}
		
		void SoraAudiereMusicFile::setRepeat(int32 loopCount) { 
			if(is_open()) sound->setRepeat(loopCount);
		}

		int32 SoraAudiereMusicFile::isRepeat() const { 
			if(is_open()) return sound->getRepeat();
			return false;
		}
		bool SoraAudiereMusicFile::isPlaying() const { 
			if(is_open()) return sound->isPlaying();
			return false;
		}

		void SoraAudiereMusicFile::_parseName(const SoraWString& path) {
            SoraWString musicName;
			SoraWString s = path;
			for(size_t i=0; i<s.size(); ++i) {
				if(s[i] == L'\\') s[i] = L'/';
			}
			if(s.find('/') != std::string::npos)
				musicName = s.substr(s.rfind('/'), s.size());
			if(musicName.find('.') != std::string::npos)
				musicName.erase(musicName.rfind('.'), musicName.size());
            setName(str2id(musicName));
		}

		int32 SoraAudiereSoundEffectFile::readFile(const SoraWString& sFilePath) {
			if(is_open()) closeFile();
            
            _parseName(sFilePath);

			AudioDevicePtr device = SoraAudiereDevice::Instance()->getDevice();
			if(!device) { return SORASOUND_FAILED_OPEN_DEVICE; }
			ulong32 size;
			void* ptr = SoraCore::Instance()->getResourceFile(sFilePath, size);
			if(ptr) {
				File* file = CreateMemoryFile(ptr, (int)size);
				if(!file) return SORASOUND_FAILED_OPEN_MEMORY_FILE;
				se = OpenSoundEffect(device, file, (SoundEffectType)iSoundEffectType);
				if(!se) return SORASOUND_FAILED_OPEN_SOUND;
			}

			set_open(true);
			return SORASOUND_SUCCESS;
		}

		int32 SoraAudiereSoundEffectFile::readFileMem(void* ptr, ulong32 size) {
			if(is_open()) closeFile();

			AudioDevicePtr device = SoraAudiereDevice::Instance()->getDevice();
			if(!device) { return SORASOUND_FAILED_OPEN_DEVICE; }
			File* file = CreateMemoryFile(ptr, (int)size);
			if(!file) return SORASOUND_FAILED_OPEN_MEMORY_FILE;
			se = OpenSoundEffect(device, file, (SoundEffectType)iSoundEffectType);
			if(!se) return SORASOUND_FAILED_OPEN_SOUND;

			set_open(true);
			return SORASOUND_SUCCESS;
		}

		void SoraAudiereSoundEffectFile::closeFile() {
			if(is_open()) {
				//se->unref();
				set_open(false);
			}
		}

		void SoraAudiereSoundEffectFile::play() {
			if(se) se->play();
		}

		void SoraAudiereSoundEffectFile::stop() {
			if(se) se->stop();
		}

		void SoraAudiereSoundEffectFile::setVolume(float32 vol) {
			if(se) se->setVolume(vol);
		}

		void SoraAudiereSoundEffectFile::setPan(float32 pan) {
			if(se) se->setPan(pan);
		}
		
		void SoraAudiereSoundEffectFile::setPitch(float32 pitch) {
			if(se) se->setPitchShift(pitch);
		}

		float32 SoraAudiereSoundEffectFile::getPan() const {
			if(se) return se->getPan();
			return 0.f;
		}

		float32 SoraAudiereSoundEffectFile::getPitch() const {
			if(se) return se->getPitchShift();
			return 0.f;
		}

		float32 SoraAudiereSoundEffectFile::getVolume() const {
			if(se) return se->getVolume();
			return 0.f;
		}

    void SoraAudiereSoundEffectFile::_parseName(const SoraWString& path) {
        SoraWString seName;
        SoraWString s = path;
        for(size_t i=0; i<s.size(); ++i) {
            if(s[i] == L'\\') s[i] = L'/';
        }
        if(s.find('/') != std::string::npos)
            seName = s.substr(s.rfind('/'), s.size());
        if(s.find('.') != std::string::npos)
            seName.erase(seName.rfind('.'), seName.size());
        setName(str2id(seName));
    }
} // namespace sora