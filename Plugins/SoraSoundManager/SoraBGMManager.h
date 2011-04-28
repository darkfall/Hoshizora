/*
 *  SoraBGMManager.h
 *  Sora
 *
 *  Created by Griffin Bu on 4/27/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_BGM_MANAGER_H_
#define SORA_BGM_MANAGER_H_

#include "SoraPlatform.h"
#include "SoraSoundFile.h"
#include "SoraSingleton.h"
#include "SoraPlugin.h"
#include "stringId.h"

namespace sora {
	
	typedef stringId BGMId;
	
	/*
	 simple interface for bgm management
	 also with some sound management features
	 a bgm would play in loop mode
	 */
	
	class SoraBGMManager: public SoraSingleton<SoraBGMManager>, public SoraPlaybackEventHandler, public SoraPlugin {
		friend class SoraSingleton<SoraBGMManager>;
	
	protected:
		SoraBGMManager();
		~SoraBGMManager();
		
		/*
		 update the bgm manager
		 inheritated from SoraPlugin
		 */
		void update();
		const SoraString getName() const { return "SoraBGMManager"; }
		
		void onPlaybackEvent(const SoraPlaybackEvent* event);
		
	public:
		/*
		 @param bgmPath, the path of the bgm to play
		 @param addToQueue, is add the bgm to the bgm queue
		*/
		bool   play(const std::wstring& bgmPath, bool addToQueue);
		void   stop(bool bStopAtEnd);
		
		void	pause();
		void	resume();
		
		/*
		 free all BGMs in the bgm queue
		 */
		void   freeAllBGMs();
		
		/*
		 force skip to next/prev bgm file in the bgm queue
		 */
		void toNextBGM();
		void toPrevBGM();
		
		// get the size of current bgm queue
		uint32 getQueueSize() const;
		
		/*
		 set bgm playback properties
		 applies to all bgms in the bgm queue
		 */
		
		// 0 - 100.0
		void setVolume(float32 volume);
		float32 getVolume() const;
		
		// 0.5 - 2.0
		void setPitch(float32 pitch);
		float32 getPitch() const;
		
		// -1.0 - 1.0
		void setPan(float32 pan);
		float32 getPan() const;
		
		/*
		 if these times are not 0, 
		 then when a new bgm starts, it will fade in fadeInTime seconds, 
		 the old bgm would fade out in fadeOutTime seconds
		 default are zero
		 */
		void setFadeTime(float32 fadeInTime, float32 fadeOutTime);
		float32 getFadeInTime() const;
		float32 getFadeOutTime() const;
		
		/*
		 if this is enabled, them when a bgm ends, new bgm would be a random item in the bgm queue
		 default false
		 */
		void enableRandomBGMQueuePlay(bool flag);
		bool isRandomBGMQueuePlayEnabled() const;
		
	private:
		inline void _clearBGMQueue();
		inline void _playBGM(SoraMusicFile* musicFile, uint32 newBGMId);
		
		typedef std::vector<SoraMusicFile*> BGM_QUEUE;
		BGM_QUEUE mBGMQueue;
		
		int32 mPrevBGMId;
		int32 mCurrBGMId;
		
		float32 bgmVolume;
		float32 bgmPitch;
		float32 bgmPan;
		
		float32 mCurrFadeInTime;
		float32 mCurrFadeOutTime;
		float32 mFadeInTime;
		float32 mFadeOutTime;
		
		bool mStopAtEnd;
		bool mRandomBGMQueuePlay;
		bool mPaused;
	};
	
} // namespace sora

#endif // SORA_BGM_MANAGER_H_