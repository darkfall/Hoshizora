//
//  SoraFMODSoundFile.h
//  Sora
//
//  Created by Griffin Bu on 4/18/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#ifndef SORA_FMOD_SOUND_FILE_H_
#define SORA_FMOD_SOUND_FILE_H_

#include "SoraPlatform.h"
#include "SoraSoundFile.h"

#ifdef OS_IOS

#import "SoraOALController.h"

/**
  This header must be included by a object-c source file
  otherwise it won't compile since it contains object-c objects
 **/

namespace sora {
    
    class SoraiOSMusicFile: public SoraMusicFile {
    public:
        SoraiOSMusicFile(bool bStream);
        SoraiOSMusicFile(const StringType& sFilePath, bool bStream);
        ~SoraiOSMusicFile();
		
		int32 readFile(const StringType& sFilePath);
		int32 readFileMem(void* ptr, ulong32 size);
		void closeFile();
        
		void play();
		void stop();
		void pause();
		void resume();
        
		// 0 - 1
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
        bool bIsStream;
        
        OALAudioTrack* mMusicTrack;
    };
    
    class SoraiOSSoundEffectFile : public SoraSoundEffectFile {
    public:
        SoraiOSSoundEffectFile();
        SoraiOSSoundEffectFile(const StringType& path);
        ~SoraiOSSoundEffectFile();
		
		int32 readFile(const StringType& sFilePath);
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
        
        void setRepeat(bool flag);
		bool isRepeat() const;
		bool isPlaying() const;
        
    private:
        bool mRepeat;
        
        float32 mVolume;
		float32 mPitch;
		float32 mPan;
        
        ALBuffer* mBuffer;
        id<ALSoundSource> mSoundSource;
    };
    
} // namespace sora

#endif // OS_IOS

#endif