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

namespace sora {
    
    class SoraFMODMusicFile: public SoraMusicFile {
    public:
        ~SoraMusicFile() {}
		
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
        
    };
    
    class SoraFMODSoundEffectFile : public SoraSoundEffectFile {
    public:
        ~SoraSoundEffectFile() {}
		
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
        
		float32 getPan();
		float32 getPitch();
		float32 getVolume();
    };
    
} // namespace sora

#endif