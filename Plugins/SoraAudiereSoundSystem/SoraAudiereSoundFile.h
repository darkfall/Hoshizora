#ifndef SORA_AUDIERE_SOUND_FILE_H
#define SORA_AUDIERE_SOUND_FILE_H

#include "audiere/audiere.h"

#include "SoraSoundFile.h"
#include "SoraStringConv.h"
#include "SoraSingleton.h"
#include "SoraString.h"

namespace sora {

	class SoraAudiereMusicFile: public SoraMusicFile {
	public:
		SoraAudiereMusicFile(bool stream=true);
		~SoraAudiereMusicFile();

		int32 readFile(const StringType& sFilePath);
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

		float32 getPan() const;
		float32 getPitch() const;
		float32 getVolume() const;

		void setPosition(float32 x, float32 y, float32 z);
        void getPosition(float32& x, float32& y, float32& z);
        
        void setMusicPosition(int32 pos);
        int32 getMusicPosition() const;

		int32 getLength() const;
		
		void setRepeat(bool flag);
		bool isRepeat() const;
		bool isPlaying() const;

	private:
		inline void _parseName(const StringType& path);
		void* data;

		bool bIsStream;
		int pausePos;

		audiere::OutputStreamPtr sound;	
	};

	class SoraAudiereSoundEffectFile: public SoraSoundEffectFile {
	public:
		SoraAudiereSoundEffectFile(int type = audiere::MULTIPLE) { iSoundEffectType = type; }
		~SoraAudiereSoundEffectFile();

		int32 readFile(const StringType& sFilePath);
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

		float32 getPan() const;
		float32 getPitch() const;
		float32 getVolume() const;

		// todo 
		void setRepeat(bool flag) {}
		bool isRepeat() const { return 0; }
		bool isPlaying() const { return false; }

	private:
        inline void _parseName(const StringType& path);
		void* data;

		audiere::SoundEffectPtr se;	

		int iSoundEffectType;
	};


} // namespace sora

#endif