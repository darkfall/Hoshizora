#ifndef SORA_AUDIERE_SOUND_FILE_H
#define SORA_AUDIERE_SOUND_FILE_H

#include "audiere/audiere.h"

#include "SoraSoundFile.h"
#include "SoraStringConv.h"
#include "SoraSingleton.h"

namespace sora {

	using namespace audiere;

	class SoraAudiereMusicFile: public SoraMusicFile {
	public:
		SoraAudiereMusicFile(bool stream=true) { bIsStream = stream; pausePos = 0; }
		int32 readFile(const SoraWString& sFilePath);
		int32 readFileMem(void* ptr, ulong32 size);
		void closeFile();

		void play();
		void stop();
		void pause();
		void resume();

		// 0 - 100
		void setVolume(float32 vol);
		// -1.0 - 1.0
		void setPan(float32 pan);
		// 0.5 - 2.0
		void setPitch(float32 pitch);

		float32 getPan();
		float32 getPitch();
		float32 getVolume();

		void setPosition(float32 x, float32 y, float32 z);
        void getPosition(float32& x, float32& y, float32& z);
        
        void setMusicPosition(int32 pos);
        int32 getMusicPosition();

		int32 getLength();
		
		void setRepeat(bool flag);
		bool isRepeat();
		bool isPlaying();
		
		SoraWString getName() { return musicName; }

	private:
		void _parseName(const SoraWString& path);

		bool bIsStream;
		int pausePos;

		OutputStreamPtr sound;	

		SoraWString musicName;
	};

	class SoraAudiereSoundEffectFile: public SoraSoundEffectFile {
	public:
		SoraAudiereSoundEffectFile(int type = MULTIPLE) { iSoundEffectType = type; }
		int32 readFile(const SoraWString& sFilePath);
		int32 readFileMem(void* ptr, ulong32 size);
		void closeFile();

		void play();
		// if type==MULTIPLE, then stop all instances of the sound
		// if type==SINGLE, stop the sound only
		void stop();
        
        void setPosition(float32 x, float32 y, float32 z) {}
        void getPosition(float32& x, float32& y, float32& z) {}

		// 0 - 1
		void setVolume(float32 vol);
		// -1.0 - 1.0
		void setPan(float32 pan);
		// 0.5 - 2.0
		void setPitch(float32 pitch);

		float32 getPan();
		float32 getPitch();
		float32 getVolume();

	private:
		SoundEffectPtr se;	

		int iSoundEffectType;
	};


} // namespace sora

#endif