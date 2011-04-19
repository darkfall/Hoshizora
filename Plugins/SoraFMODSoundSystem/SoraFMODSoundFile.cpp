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
    
    SoraFMODMusicFile::SoraFMODMusicFile(bool bStream) {
        pSystem = SoraFMODSystem::Instance()->getSystem();
        if(!pSystem)
            INT_LOG_HANDLE->logf("Creating FMODMusicFile: No FMOD System available");
        
        pSound = NULL;
        pChannel = NULL;
        pSoundData = NULL;
                
        bIsStream = bStream;
    }
    
    SoraFMODMusicFile::SoraFMODMusicFile(const SoraWString& path, bool bStream) {
        pSystem = SoraFMODSystem::Instance()->getSystem();
        if(!pSystem)
            INT_LOG_HANDLE->logf("Creating %s: No FMOD System available", ws2s(path).c_str());
        bIsStream = bStream;
        
        pSound = NULL;
        pChannel = NULL;
        pSoundData = NULL;

        if(!SoraFileUtility::fileExists(path)) {            
            ulong32 size;
            void* data = SORA->getResourceFile(path, size);
            if(data) {
                readFileMem(data, size);
                SORA->freeResourceFile(data);
             } else 
                INT_LOG_HANDLE->logf("Creating %s: File not exists", ws2s(path).c_str());
        } else {
            readFile(path);
        }
    }
    
    SoraFMODMusicFile::~SoraFMODMusicFile() {
        if(bIsStream)
            if(pSoundData)
                SORA->freeResourceFile(pSoundData);
    }
    
    int32 SoraFMODMusicFile::readFile(const SoraWString& path) {
        if(!pSystem) return 0;
        if(pSound) closeFile();
        
        FMOD_RESULT result;
        if(!bIsStream)
            result = pSystem->createSound(ws2s(path).c_str(), FMOD_CREATESAMPLE | FMOD_SOFTWARE | FMOD_2D, 0, &pSound);
        else 
            result = pSystem->createStream(ws2s(path).c_str(), FMOD_CREATESAMPLE | FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &pSound);
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
            result = pSystem->createStream((const char*)data, FMOD_CREATESAMPLE | FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_OPENMEMORY, &exinfo, &pSound);
        return FMOD_ERROR_CHECK(result)==FMOD_OK?1:0;
    }
    
    void SoraFMODMusicFile::closeFile() {
        if(pSound) {
            if(pChannel)
                pChannel->stop();
            pSound->release();
        }
    }
    
    void SoraFMODMusicFile::play() {
        if(pSound) {
            pSystem->playSound(FMOD_CHANNEL_FREE, pSound, pChannel==NULL?false:true, &pChannel);
        }
    }
    
    void SoraFMODMusicFile::stop() {
        if(pChannel)
            pChannel->stop();
    }
    
    void SoraFMODMusicFile::pause() {
        if(pChannel)
            pChannel->setPaused(true);
    }
    
    void SoraFMODMusicFile::resume() {
        if(pChannel)
            pChannel->setPaused(false);
    }
    
    void SoraFMODMusicFile::setVolume(float32 vol) {
        if(pChannel)
            // scale to 0-1
            pChannel->setVolume(vol/100.f);
    }
    
    void SoraFMODMusicFile::setPan(float32 pan) {
        if(pChannel)
            pChannel->setPan(pan);
    }
    
    void SoraFMODMusicFile::setPitch(float32 pitch) {
      //  if(pChannel)
      //      pChannel->setPitch(
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
    
    SoraFMODSoundEffectFile::SoraFMODSoundEffectFile {
        pSystem = SoraFMODSystem::Instance()->getSystem();
        if(!pSystem)
            INT_LOG_HANDLE->logf("Creating %s: No FMOD System available", ws2s(path).c_str());
            
        pSound = NULL;
        pChannel = NULL;
    }
    
    SoraFMODSoundEffectFile::SoraFMODSoundEffectFile(const SoraWString& path) {
        pSystem = SoraFMODSystem::Instance()->getSystem();
        if(!pSystem)
            INT_LOG_HANDLE->logf("Creating %s: No FMOD System available", ws2s(path).c_str());
        
        pSound = NULL;
        pChannel = NULL;
        pSoundData = NULL;
        
        if(!SoraFileUtility::fileExists(path)) {            
            ulong32 size;
            void* data = SORA->getResourceFile(path, size);
            if(data) {
                readFileMem(data, size);
                SORA->freeResourceFile(data);
            } else 
                INT_LOG_HANDLE->logf("Creating %s: File not exists", ws2s(path).c_str());
        } else {
            readFile(path);
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
            pSystem->playSound(FMOD_CHANNEL_FREE, pSound, pChannel==NULL?false:true, &pChannel);
        }
    }
    
    void SoraFMODSoundEffectFile::stop() {
        if(pChannel)
            pChannel->stop();
    }
    
    void SoraFMODSoundEffectFile::setVolume(float32 vol) {
        if(pChannel)
            // scale to 0-1
            pChannel->setVolume(vol/100.f);
    }
    
    void SoraFMODSoundEffectFile::setPan(float32 pan) {
        if(pChannel)
            pChannel->setPan(pan);
    }
    
    void SoraFMODSoundEffectFile::setPitch(float32 pitch) {
        //  if(pChannel)
        //      pChannel->setPitch(
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