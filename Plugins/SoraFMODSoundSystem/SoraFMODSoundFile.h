//
//  SoraFMODSoundFile.h
//  Sora
//
//  Created by Griffin Bu on 4/18/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef SORA_FMOD_SOUND_FILE_H_
#define SORA_FMOD_SOUND_FILE_H_

#include "SoraSoundFile.h"
#include "SoraFMODSystem.h"

namespace sora {
    
    class SoraFMODMusicFile: public SoraMusicFile {
    public:
        SoraFMODMusicFile(bool bStream);
        SoraFMODMusicFile(const SoraWString& sFilePath, bool bStream);
        ~SoraFMODMusicFile();
		
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
        
		float32 getPan() const;
		float32 getPitch() const;
		float32 getVolume() const;
        
		void setPosition(float32 x, float32 y, float32 z);
		void getPosition(float32& x, float32& y, float32& z);
        
        void setMusicPosition(int32 pos);
        int32 getMusicPosition() const;
		
		int32 getLength() const;
		
		void setRepeat(int32 loopCount);
		int32 isRepeat() const;
		bool isPlaying() const;
        
    private:
        FMOD::System* pSystem;
        FMOD::Sound* pSound;
        FMOD::Channel* pChannel;
        
        bool bIsStream;
        void *pSoundData;
		
		float32 volume;
		float32 pitch;
		float32 pan;
    };
    
    class SoraFMODSoundEffectFile : public SoraSoundEffectFile {
    public:
        SoraFMODSoundEffectFile();
        SoraFMODSoundEffectFile(const SoraWString& path);
        ~SoraFMODSoundEffectFile();
		
		int32 readFile(const SoraWString& sFilePath);
		int32 readFileMem(void* ptr, ulong32 size);
		void closeFile();
        
		void play();
		void stop();
        
        void setPosition(float32 x, float32 y, float32 z);
        void getPosition(float32& x, float32& y, float32& z);
        
		// 0 - 1
		void setVolume(float32 vol);
		// -1.0 - 1.0
		void setPan(float32 pan);
		// 0.5 - 2.0
		void setPitch(float32 pitch);
        
		float32 getPan() const;
		float32 getPitch() const;
		float32 getVolume() const;
        
        void setRepeat(int32 loopCount);
		int32 isRepeat() const;
		bool isPlaying() const;
        
    private:
        FMOD::System* pSystem;
        
        FMOD::Sound* pSound;
        FMOD::Channel* pChannel;
		
		float32 volume;
		float32 pitch;
		float32 pan;
        
        void *pSoundData;
    };
    
} // namespace sora

#endif