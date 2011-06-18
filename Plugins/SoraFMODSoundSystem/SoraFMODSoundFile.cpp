//
//  SoraFMODSoundFile.cpp
//  Sora
//
//  Created by Griffin Bu on 4/18/11.
//  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
//

#include "SoraFMODSoundFile.h"
#include "SoraFileUtility.h"
#include "SoraCore.h"

namespace sora {
    
#ifndef OS_WIN32
    static FMOD_RESULT SORACALL myChannelCallback(FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2) {
#else
	static FMOD_RESULT _stdcall myChannelCallback(FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2) {
#endif
		SoraPlaybackEventHandler* pMusicFile = NULL;
        FMOD::Channel* pChannel = (FMOD::Channel*)channel;
        if(pChannel) {
            void* pmyData;
            pChannel->getUserData(&pmyData);
            if(pmyData)
                pMusicFile = (SoraFMODMusicFile*)pmyData;
        }
        
        if(!pMusicFile) {
            DebugPtr->log("error converting event data object to playbackeventHandler", 
						  LOG_LEVEL_ERROR);
            return FMOD_ERR_EVENT_FAILED;
        }
        
        if(type == FMOD_CHANNEL_CALLBACKTYPE_END) {
			if(pMusicFile->isEventPublishEnabled())
				pMusicFile->publishEvent(SORAPB_EV_PLAY_ENDED);
        }
        return FMOD_OK;
    }
    
    SoraFMODMusicFile::SoraFMODMusicFile(bool bStream) {
        pSystem = SoraFMODSystem::Instance()->getSystem();
        if(!pSystem)
            DebugPtr->log("Creating FMODMusicFile: No FMOD System available", LOG_LEVEL_ERROR);
        
        pSound = NULL;
        pChannel = NULL;
        pSoundData = NULL;
		
		volume = 100.f;
		pan = 0.f;
		pitch = 0.f;
                
        bIsStream = bStream;
    }
    
    SoraFMODMusicFile::SoraFMODMusicFile(const SoraWString& path, bool bStream) {
        pSystem = SoraFMODSystem::Instance()->getSystem();
        if(!pSystem)
            DebugPtr->log(vamssg("Creating %s: No FMOD System available", ws2s(path).c_str()),
						  LOG_LEVEL_ERROR);
        bIsStream = bStream;
        
        pSound = NULL;
        pChannel = NULL;
        pSoundData = NULL;
		
		volume = 100.f;
		pan = 0.f;
		pitch = 0.f;

        if(!SoraFileUtility::fileExists(path)) {            
            ulong32 size;
            void* data = SORA->getResourceFile(path, size);
            if(data) {
                readFileMem(data, size);
                SORA->freeResourceFile(data);
             } else 
                DebugPtr->log(vamssg("Creating %s: File not exists", ws2s(path).c_str()),
							  LOG_LEVEL_ERROR);
        } else {
            readFile(path);
        }
    }
    
    SoraFMODMusicFile::~SoraFMODMusicFile() {
		closeFile();
    }
    
    int32 SoraFMODMusicFile::readFile(const SoraWString& path) {
        if(!pSystem) return 0;
        if(pSound) closeFile();
        
        FMOD_RESULT result;
        if(!bIsStream)
            result = pSystem->createSound(ws2s(path).c_str(), FMOD_CREATESAMPLE | FMOD_SOFTWARE | FMOD_2D, 0, &pSound);
        else 
            result = pSystem->createStream(ws2s(path).c_str(), FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &pSound);
        return FMOD_ERROR_CHECK(result)==FMOD_OK?1:0;
    }
    
    int32 SoraFMODMusicFile::readFileMem(void* data, ulong32 length) {
        if(!pSystem) return 0;
        if(pSound) closeFile();

        FMOD_RESULT result;
        FMOD_CREATESOUNDEXINFO exinfo;
        exinfo.length = length;
        if(!bIsStream)
            result = pSystem->createSound((const char*)data, FMOD_CREATESAMPLE | FMOD_SOFTWARE | FMOD_2D | FMOD_OPENMEMORY, &exinfo, &pSound);
        else
            result = pSystem->createStream((const char*)data, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_OPENMEMORY, &exinfo, &pSound);
        return FMOD_ERROR_CHECK(result)==FMOD_OK?1:0;
    }
    
    void SoraFMODMusicFile::closeFile() {
        if(pSound) {
            if(pChannel) {
				pChannel->setUserData(0);
                pChannel->stop();
			}
            pSound->release();
        }
    }
    
    void SoraFMODMusicFile::play() {
        if(pSound) {
            if(pChannel)
                pChannel->stop();
            FMOD_RESULT result = pSystem->playSound(FMOD_CHANNEL_FREE, pSound, false, &pChannel);
			if(pChannel) {
				pChannel->setVolume(volume/100.f);
				if(pan != 0.f) pChannel->setPan(pan);
			}
			
            if(FMOD_ERROR_CHECK(result) && mEventPublish) {
                publishEvent(SORAPB_EV_PLAY_STARTED);
                
                // register FMOD Channel Callback to get SOUND_ENDED event
                pChannel->setUserData((void*)this);
                pChannel->setCallback(myChannelCallback);
                pChannel->setPaused(false);
            }
        }
    }
    
    void SoraFMODMusicFile::stop() {
        if(pChannel) {
            FMOD_RESULT result = pChannel->stop();
            if(FMOD_ERROR_CHECK(result) && mEventPublish)
				publishEvent(SORAPB_EV_PLAY_STOPPED);
        }
    }
    
    void SoraFMODMusicFile::pause() {
        if(pChannel) {
            FMOD_RESULT result = pChannel->setPaused(true);
            if(FMOD_ERROR_CHECK(result) && mEventPublish) {
                publishEvent(SORAPB_EV_PLAY_PAUSED);
            }
        }
    }
    
    void SoraFMODMusicFile::resume() {
        if(pChannel) {
            FMOD_RESULT result = pChannel->setPaused(false);
            if(FMOD_ERROR_CHECK(result) && mEventPublish) {
                publishEvent(SORAPB_EV_PLAY_RESUMED);
            }
        }
    }
    
    void SoraFMODMusicFile::setVolume(float32 vol) {
        if(pChannel)
            // scale to 0-1
            pChannel->setVolume(vol/100.f);
		volume = vol;
    }
    
    void SoraFMODMusicFile::setPan(float32 pan) {
        if(pChannel)
            pChannel->setPan(pan);
		this->pan = pan;
    }
    
    void SoraFMODMusicFile::setPitch(float32 pitch) {
      //  if(pChannel)
      //      pChannel->setPitch(
		this->pitch = pitch;
    }
    
    float32 SoraFMODMusicFile::getVolume() const {
        if(pChannel) {
            float32 vol;
            FMOD_RESULT result = pChannel->getVolume(&vol);
            FMOD_ERROR_CHECK(result);
            // scale to 0-100
            vol *= 100.f;
            return vol;
        }
        return 0.f;
    }
    
    float32 SoraFMODMusicFile::getPitch() const {
        return 0.f;
    }
    
    float32 SoraFMODMusicFile::getPan() const {
        if(pChannel) {
            float32 pan;
            FMOD_RESULT result = pChannel->getPan(&pan);
            FMOD_ERROR_CHECK(result);
            return pan;
        }
        return 0.f;
    }
    
    void SoraFMODMusicFile::setPosition(float32 x, float32 y, float32 z) {
        
    }
    
    void SoraFMODMusicFile::getPosition(float32 &x, float32 &y, float32 &z) {
    
    }
    
    void SoraFMODMusicFile::setMusicPosition(int32 pos) {
        if(pChannel)
            pChannel->setPosition(pos, FMOD_TIMEUNIT_MS);
    }
    
    int32 SoraFMODMusicFile::getMusicPosition() const {
        if(pChannel) {
            unsigned int pos;
            FMOD_RESULT result = pChannel->getPosition(&pos, FMOD_TIMEUNIT_MS);
            FMOD_ERROR_CHECK(result);
            return pos;
        }
        return 0;
    }
    
    int32 SoraFMODMusicFile::getLength() const {
        if(pSound) {
            unsigned int length;
            FMOD_RESULT result = pSound->getLength(&length, FMOD_TIMEUNIT_MS);
            FMOD_ERROR_CHECK(result);
            return length;
        }
        return 0;
    }
    
    void SoraFMODMusicFile::setRepeat(int32 loopCount) {
        if(pChannel)
            pChannel->setLoopCount(loopCount);
    }
    
    int32 SoraFMODMusicFile::isRepeat() const {
        if(pChannel) {
            int lc = 0;
            pChannel->getLoopCount(&lc);
            return lc;
        }
        return 0;
    }
    
    bool SoraFMODMusicFile::isPlaying() const {
        if(pChannel) {
            bool bp;
            pChannel->isPlaying(&bp);
            return bp;
        }
        return false;
    }
    
    SoraFMODSoundEffectFile::SoraFMODSoundEffectFile() {
        pSystem = SoraFMODSystem::Instance()->getSystem();
        if(!pSystem)
            DebugPtr->log("Creating FMODSoundEffectFile: No FMOD System available");
            
        pSound = NULL;
        pChannel = NULL;
		
		volume = 100.f;
		pan = 0.f;
		pitch = 0.f;
    }
    
    SoraFMODSoundEffectFile::SoraFMODSoundEffectFile(const SoraWString& path) {
        pSystem = SoraFMODSystem::Instance()->getSystem();
        if(!pSystem)
            DebugPtr->log(vamssg("Creating %s: No FMOD System available", ws2s(path).c_str()),
						  LOG_LEVEL_ERROR);
        
        pSound = NULL;
        pChannel = NULL;
        pSoundData = NULL;
		
		volume = 100.f;
		pan = 0.f;
		pitch = 0.f;
        
        if(!SoraFileUtility::fileExists(path)) {            
            ulong32 size;
            void* data = SORA->getResourceFile(path, size);
            if(data) {
                readFileMem(data, size);
                SORA->freeResourceFile(data);
            } else 
                DebugPtr->log(vamssg("Creating %s: File not exists", ws2s(path).c_str()),
							  LOG_LEVEL_ERROR);
        } else {
            readFile(path);
        }
    }

	
	SoraFMODSoundEffectFile::~SoraFMODSoundEffectFile() {
		closeFile();
    }

	 void SoraFMODSoundEffectFile::closeFile() {
        if(pSound) {
            if(pChannel)
                pChannel->stop();
            pSound->release();
        }
    }
    
    int32 SoraFMODSoundEffectFile::readFile(const SoraWString& path) {
        if(!pSystem) return 0;
        if(pSound) closeFile();
        
        FMOD_RESULT result;
        result = pSystem->createSound(ws2s(path).c_str(), FMOD_CREATESAMPLE | FMOD_SOFTWARE | FMOD_2D, 0, &pSound);
    
        return FMOD_ERROR_CHECK(result)==FMOD_OK?1:0;
    }
    
    int32 SoraFMODSoundEffectFile::readFileMem(void* data, ulong32 length) {
        if(!pSystem) return 0;
        if(pSound) closeFile();
        
        FMOD_RESULT result;
        FMOD_CREATESOUNDEXINFO exinfo;
        exinfo.length = length;
        result = pSystem->createSound((const char*)data, FMOD_CREATESAMPLE | FMOD_SOFTWARE | FMOD_2D | FMOD_OPENMEMORY, &exinfo, &pSound);
        return FMOD_ERROR_CHECK(result)==FMOD_OK?1:0;
    }
    
    void SoraFMODSoundEffectFile::play() {
        if(pSound) {
			if(pChannel)
				pChannel->stop();
            FMOD_RESULT result = pSystem->playSound(FMOD_CHANNEL_FREE, pSound, false, &pChannel);
			if(pChannel) {
				pChannel->setVolume(volume/100.f);
				if(pan != 0.f) pChannel->setPan(pan);
			}
			if(FMOD_ERROR_CHECK(result) && mEventPublish) {
				
				
                publishEvent(SORAPB_EV_PLAY_STARTED);
                pChannel->setUserData((void*)this);
                pChannel->setCallback(myChannelCallback);
            }
        }
    }
    
    void SoraFMODSoundEffectFile::stop() {
        if(pChannel) {
            FMOD_RESULT result = pChannel->stop();
            if(FMOD_ERROR_CHECK(result)&& mEventPublish) {
                publishEvent(SORAPB_EV_PLAY_STOPPED);
            }
        }
    }
    
    void SoraFMODSoundEffectFile::setVolume(float32 vol) {
        if(pChannel)
            // scale to 0-1
            pChannel->setVolume(vol/100.f);
		volume = vol;
    }
    
    void SoraFMODSoundEffectFile::setPan(float32 pan) {
        if(pChannel)
            pChannel->setPan(pan);
		this->pan = pan;
    }
    
    void SoraFMODSoundEffectFile::setPitch(float32 pitch) {
        //  if(pChannel)
        //      pChannel->setPitch(
		this->pitch = pitch;
    }
    
    float32 SoraFMODSoundEffectFile::getVolume() const {
        if(pChannel) {
            float32 vol;
            FMOD_RESULT result = pChannel->getVolume(&vol);
            FMOD_ERROR_CHECK(result);
            // scale to 0-100
            vol *= 100.f;
            return vol;
        }
        return 0.f;
    }
    
    float32 SoraFMODSoundEffectFile::getPitch() const {
        return 0.f;
    }
    
    float32 SoraFMODSoundEffectFile::getPan() const {
        if(pChannel) {
            float32 pan;
            FMOD_RESULT result = pChannel->getPan(&pan);
            FMOD_ERROR_CHECK(result);
            return pan;
        }
        return 0.f;
    }
    
    void SoraFMODSoundEffectFile::setPosition(float32 x, float32 y, float32 z) {
        
    }
    
    void SoraFMODSoundEffectFile::getPosition(float32 &x, float32 &y, float32 &z) {
        
    }
    
    void SoraFMODSoundEffectFile::setRepeat(int32 loopCount) {
        if(pChannel)
            pChannel->setLoopCount(loopCount);
    }
    
    int32 SoraFMODSoundEffectFile::isRepeat() const {
        if(pChannel) {
            int lc = 0;
            pChannel->getLoopCount(&lc);
            return lc;
        }
        return 0;
    }
    
    bool SoraFMODSoundEffectFile::isPlaying() const {
        if(pChannel) {
            bool bp;
            pChannel->isPlaying(&bp);
            return bp;
        }
        return false;
    }
} // namespace sora