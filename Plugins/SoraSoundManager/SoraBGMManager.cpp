/*
 *  SoraBGMManager.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 4/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraBGMManager.h"
#include "SoraCore.h"

namespace sora {

	SoraBGMManager::SoraBGMManager(): bgmVolume(100.f), bgmPitch(-2.f), bgmPan(-2.f), 
										mCurrFadeInTime(-1.f), mCurrFadeOutTime(-1.f), mFadeInTime(0.f), mFadeOutTime(0.f), 
										mStopAtEnd(false), mRandomBGMQueuePlay(false), mPaused(false),
										mPrevBGMId(-1), mCurrBGMId(-1) {
		SORA->registerPlugin(this);
	}
	
	void SoraBGMManager::_clearBGMQueue() {
		if(!mBGMQueue.empty()) {
			for(uint32 i=0; i<mBGMQueue.size(); ++i) {
				if(mBGMQueue[i] != NULL) {
					mBGMQueue[i]->stop();
					delete mBGMQueue[i];
					mBGMQueue[i] = NULL;
				}
			}
			mBGMQueue.clear();
		}
		
		mCurrFadeInTime = -1.f;
		mCurrFadeOutTime = -1.f;
		mPrevBGMId = -1;
		mCurrBGMId = -1;
		mStopAtEnd = false;
		mPaused = false;
	}
	
	SoraBGMManager::~SoraBGMManager() {
		_clearBGMQueue();
	}
	
	bool SoraBGMManager::play(const std::wstring& bgmPath, bool addToQueue) {
		if(!addToQueue)
			_clearBGMQueue();
	
		SoraMusicFile* pmfile = sora::SORA->createMusicFile(bgmPath);
		if(pmfile != NULL) {
			mBGMQueue.push_back(pmfile);
			if(!addToQueue || mBGMQueue.size() == 1) {
				_playBGM(pmfile, 0);
			}
			return true;
		}
		return false;
	}

	void SoraBGMManager::_playBGM(SoraMusicFile* musicFile, uint32 newBGMId) {
		if(mFadeOutTime != 0.f) {
			mPrevBGMId = mCurrBGMId;
			mCurrFadeOutTime = 0.f;
		}
		else {
			if(mCurrBGMId >= 0 && mCurrBGMId < mBGMQueue.size()) {
				mBGMQueue[mCurrBGMId]->stop();
			}
		}
		if(mFadeInTime != 0.f) 
			mCurrFadeInTime = 0.f;
		
		musicFile->enableEventPublish(true);
		musicFile->registerEventHandler(this);
		
		musicFile->setVolume(bgmVolume);
		if(bgmPitch != -2.f)
			musicFile->setPitch(bgmPitch);
		if(bgmPan != -2.f)
			musicFile->setPan(bgmPan);
		musicFile->play();
		
		mCurrBGMId = newBGMId;
		mPaused = false;
	}
	
	void SoraBGMManager::pause() {
		if(mPaused)
			return;
		
		mPaused = true;
		if(mCurrBGMId >= 0 && mCurrBGMId < mBGMQueue.size())
			mBGMQueue[mCurrBGMId]->pause();
		else
			mPaused = false;
	}
	
	void SoraBGMManager::resume() {
		if(mPaused) {
			if(mCurrBGMId >= 0 && mCurrBGMId < mBGMQueue.size())
				mBGMQueue[mCurrBGMId]->resume();
			mPaused = false;
		}
	}
	
	void SoraBGMManager::stop(bool bStopAtEnd) {
		if(!bStopAtEnd) {
			_clearBGMQueue();
		} else
			mStopAtEnd = bStopAtEnd;
	}
	
	void SoraBGMManager::freeAllBGMs() {
		_clearBGMQueue();
	}
	
	void SoraBGMManager::toNextBGM() {
		if(mCurrBGMId < mBGMQueue.size()-1) {
			_playBGM(mBGMQueue[mCurrBGMId+1], mCurrBGMId+1);
		}
	}
	
	void SoraBGMManager::toPrevBGM() {
		if(mCurrBGMId > 0) {
			_playBGM(mBGMQueue[mCurrBGMId-1], mCurrBGMId-1);
		}
	}
	
	uint32 SoraBGMManager::getQueueSize() const {
		return mBGMQueue.size();
	}
	
	void SoraBGMManager::setVolume(float32 volume) { 
		bgmVolume = volume;
	}
	
	float32 SoraBGMManager::getVolume() const {
		return bgmVolume;
	}
	
	void SoraBGMManager::setPitch(float32 pitch) {
		bgmPitch = pitch;
	}
	
	float32 SoraBGMManager::getPitch() const {
		return bgmPitch;
	}
	
	void SoraBGMManager::setPan(float32 pan) {
		bgmPan = pan;
	}
	
	float32 SoraBGMManager::getPan() const {
		return bgmPan;
	}
	
	void SoraBGMManager::setFadeTime(float32 fadeInTime, float32 fadeOutTime) {
		mFadeInTime	 = fadeInTime;
		mFadeOutTime = fadeOutTime;
	}
	
	float32 SoraBGMManager::getFadeInTime() const {
		return mFadeInTime;
	}
	
	float32 SoraBGMManager::getFadeOutTime() const {
		return mFadeOutTime;
	}
	
	void SoraBGMManager::enableRandomBGMQueuePlay(bool flag) {
		mRandomBGMQueuePlay = flag;
	}
	
	bool SoraBGMManager::isRandomBGMQueuePlayEnabled() const {
		return mRandomBGMQueuePlay;
	}
	
	void SoraBGMManager::update() {
		if(mPaused)
			return; 
		
		float32 delta = SORA->getDelta();
		
		if(mCurrFadeOutTime != -1.f && mPrevBGMId != -1) {
			if(mCurrFadeOutTime < mFadeOutTime) {
				mCurrFadeOutTime += delta;
				if(mCurrFadeOutTime >= mFadeOutTime) {
					mBGMQueue[mPrevBGMId]->stop();
					
					mCurrFadeOutTime = -1.f;
					mPrevBGMId = -1;
				} else {
					mBGMQueue[mPrevBGMId]->setVolume(bgmVolume * (1.f - (mCurrFadeOutTime / mFadeOutTime)));
				}
			}
		}
		if(mCurrFadeInTime != -1.f && mCurrBGMId != -1) {
			if(mCurrFadeInTime < mFadeInTime) {
				mCurrFadeInTime += delta;
				if(mCurrFadeInTime >= mFadeInTime) {
					mCurrFadeInTime = -1.f;
					mBGMQueue[mCurrBGMId]->setVolume(bgmVolume);
				} else {
					mBGMQueue[mCurrBGMId]->setVolume(bgmVolume * (mCurrFadeInTime / mFadeInTime));
				}
			}
		}
		
		printf("%f, %f, %f, %f\n", mCurrFadeOutTime, mFadeOutTime, mCurrFadeInTime, mFadeInTime);
	}
	
	void SoraBGMManager::onPlaybackEvent(const SoraPlaybackEvent* event) {
		if(event->getEventType() == SORAPB_EV_PLAY_ENDED) {
			if(mCurrBGMId < mBGMQueue.size()) {
				if(!mRandomBGMQueuePlay)
					_playBGM(mBGMQueue[mCurrBGMId+1], mCurrBGMId+1);
				else {
					uint32 newId = SORA->randomInt(0, mBGMQueue.size());
					_playBGM(mBGMQueue[newId], newId);
				}
			} else {
				if(mStopAtEnd)
					_clearBGMQueue();
				else
					mCurrBGMId = 0;
			}
		}
	}

} // namespace sora