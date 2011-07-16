#ifndef SORA_SOUND_FILE_H
#define SORA_SOUND_FILE_H

#include "SoraPlatform.h"
#include "SoraFileBase.h"
#include "SoraEvent.h"

#include "SoraConstantStrings.h"
#include "SoraPlaybackEvent.h"

namespace sora {
	
	/*
	 virtual sound file base classes
	 */

	enum SORA_SOUND_FILE_OPEN_CODE {
		SORASOUND_SUCCESS = 1,
		SORASOUND_FAILED_OPEN_DEVICE = -1,
		SORASOUND_FAILED_OPEN_SOUND = -2,
		SORASOUND_FAILED_OPEN_DISK_FILE = -4,
		SORASOUND_FAILED_OPEN_MEMORY_FILE = -3,
	};
    
	class SORA_API SoraMusicFile: public SoraFileBase, public SoraPlaybackEventHandler {
	public:
		virtual ~SoraMusicFile() {}
		
		virtual int32 readFile(const SoraWString& sFilePath) = 0;
		virtual int32 readFileMem(void* ptr, ulong32 size) = 0;
		virtual void closeFile() = 0;

		virtual void play() = 0;
		virtual void stop() = 0;
		virtual void pause() = 0;
		virtual void resume() = 0;

		// 0 - 100
		virtual void setVolume(float32 vol) = 0;
		// -1.0 - 1.0
		virtual void setPan(float32 pan) = 0;
		// 0.5 - 2.0
		virtual void setPitch(float32 pitch) = 0;

		virtual float32 getPan() const = 0;
		virtual float32 getPitch() const = 0;
		virtual float32 getVolume() const = 0;

		virtual void setPosition(float32 x, float32 y, float32 z) = 0;
		virtual void getPosition(float32& x, float32& y, float32& z) = 0;
        
        virtual void setMusicPosition(int32 pos) = 0;
        virtual int32 getMusicPosition() const = 0;
		
		virtual int32 getLength() const = 0;
		
		virtual void setRepeat(int32 flag) = 0;
		virtual int32 isRepeat() const = 0;
		virtual bool isPlaying() const = 0;
	};

	class SORA_API SoraSoundEffectFile: public SoraFileBase, public SoraPlaybackEventHandler {
	public:
		virtual ~SoraSoundEffectFile() {}
		
		virtual int32 readFile(const SoraWString& sFilePath) = 0;
		virtual int32 readFileMem(void* ptr, ulong32 size) = 0;
		virtual void closeFile() = 0;

		virtual void play() = 0;
		virtual void stop() = 0;
        
        virtual void setPosition(float32 x, float32 y, float32 z) = 0;
        virtual void getPosition(float32& x, float32& y, float32& z) = 0;

		// 0 - 1
		virtual void setVolume(float32 vol) = 0;
		// -1.0 - 1.0
		virtual void setPan(float32 pan) = 0;
		// 0.5 - 2.0
		virtual void setPitch(float32 pitch) = 0;

		virtual float32 getPan() const = 0;
		virtual float32 getPitch() const = 0;
		virtual float32 getVolume() const = 0;
        
        virtual void setRepeat(int32 flag) = 0;
		virtual int32 isRepeat() const = 0;
		virtual bool isPlaying() const = 0;
	};


} // namespace sora

#endif