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
	
	class SoraBGMManager: public SoraSingleton<SoraBGMManager>, public SoraEventHandler, public SoraPlugin {
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
		 plays a bgm
		 @param bgmPath, the path of the bgm to play
		 @param addToQueue, is add the bgm to the bgm queue
		*/
		bool   play(const std::wstring& bgmPath, bool addToQueue);
		/*
		 stops the bgm
		 @param stopAtEnd, is stop the bgm queue at the end of it
				notice if random bgm playback is enabled, 
				the queue may never reach the end
		 */
		void   stop(bool stopAtEnd);
		
		/*
		 plays a looping background sound that mixes with the bgm
		 @param bgmsPath, the path of the sound
		 @param bgsid, the user defined id of the bgs
		 @param looptime, the loop time of the bgs, pass -1 for infinite loop
		 @param volume, the volume of the bg sound, this is a scale relative to the bgmVolume, 0.0 - 1.0
		 @param bgmVolume adjustment, this is a scale relative to the bgmVolume, use 0.0 - 1.0 to adjust bgmvolume to a certain percentage
		 @return the id represents the bg sound, it is the same with the user defined id if no error happened
				if the user defined id had already be token in the bgs queue, then the returned id would be the
				first available id after it
				return -1 means failed open the bg sound file
		*/
		int32	playBGS(const std::wstring& bgmsPath, uint32 bgsid, int32 looptimes, float32 volumeScale, float32 bgmVolumeScale);
		/*
		 adjust the volume of a certain bgs
		 @param volume, the new volume for the bgs
		 */
		void	adjustBGSVolume(uint32 bgsid, float32 volume);
		/*
		 stops a certain bgs
		 @param bgsid, the id of the bgs, see @playBGS
		*/
		void	stopBGS(uint32 bgsid);
		
		
		
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
		
		typedef struct tagBGSInfo {
			uint32 bgsid;
			SoraMusicFile* bgsFile;
			
			uint32 currLoopTimes;
			uint32 loopTimes;
			
			float32 volumeScale;
			float32 bgmVolumeScale;
			
			tagBGSInfo(uint32 id, uint32 lt, uint32 vol, uint32 bgmvol): 
				bgsid(id), loopTimes(lt), volumeScale(vol), bgmVolumeScale(bgmvol), currLoopTimes(0), bgsFile(NULL) {
			}
		} BGSInfo;
		typedef std::map<uint32, BGSInfo> BGS_MAP;
		BGS_MAP mBGSounds;
	};
	
} // namespace sora

#endif // SORA_BGM_MANAGER_H_