//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#ifndef _JSOUNDSYSTEM_H_
#define _JSOUNDSYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "JTypes.h"


#ifdef WIN32

	#include <windows.h>

#else

	#include <pspgu.h>
	#include <pspkernel.h>
	#include <pspdisplay.h>
	#include <pspdebug.h> 
	#include <pspctrl.h>
	#include <time.h>
	#include <string.h>
	#include <pspaudiolib.h>
	#include <psprtc.h>

	#include "JAudio.h"
	#include "JCooleyesMP3.h"

#endif


//------------------------------------------------------------------------------------------------
class JMusic
{
public:
	JMusic();
	~JMusic();

#ifdef WIN32
	FSOUND_SAMPLE *mTrack;		// MP3 needed to be of "sample" type for FMOD, FMUSIC_MODULE is for MODs
#else
	JCooleyesMP3* mTrack;
#endif
	
};


//------------------------------------------------------------------------------------------------
class JSample
{
public:
	JSample();
	~JSample();

	int mVoice;
	
#ifdef WIN32
	FSOUND_SAMPLE *mSample;
#else
	WAVDATA *mSample;
#endif
};


//////////////////////////////////////////////////////////////////////////
/// Sound engine for playing sound effects (WAV) and background 
/// music (MP3).
///
//////////////////////////////////////////////////////////////////////////
class JSoundSystem
{

public:

	//////////////////////////////////////////////////////////////////////////
	/// Get the singleton instance
	///
	//////////////////////////////////////////////////////////////////////////
	static JSoundSystem* GetInstance();

	static void Destroy();



	//////////////////////////////////////////////////////////////////////////
	/// Load music.
	/// 
	/// @note MP3 is the only supported format for the moment.
	/// 
	/// @param filename - Name of the music file.
	/// 
	//////////////////////////////////////////////////////////////////////////
	JMusic *LoadMusic(const char *fileName);

	//////////////////////////////////////////////////////////////////////////
	/// Delete music from memory.
	/// 
	/// @param music - Music to be deleted.
	/// 
	//////////////////////////////////////////////////////////////////////////
	//void FreeMusic(JMusic *music);

	//////////////////////////////////////////////////////////////////////////
	/// Play music.
	/// 
	/// @param music - Music to be played.
	/// @param looping - Play the music in a loop.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void PlayMusic(JMusic *music, bool looping = false);

	//////////////////////////////////////////////////////////////////////////
	/// Stop playing.
	/// 
	/// @param music - Music to be stopped.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void StopMusic(JMusic *music);


	//////////////////////////////////////////////////////////////////////////
	/// Resume playing.
	/// 
	/// @param music - Music to be resumed.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void ResumeMusic(JMusic *music);

	//////////////////////////////////////////////////////////////////////////
	/// Load sound effect.
	/// 
	/// @note WAV sound effect only.
	/// 
	/// @param fileName - Sound effect for loading.
	/// 
	//////////////////////////////////////////////////////////////////////////
	JSample *LoadSample(const char *fileName);

	//////////////////////////////////////////////////////////////////////////
	/// Delete sound effect from memory.
	/// 
	/// @param sample - Sound to be deleted.
	/// 
	//////////////////////////////////////////////////////////////////////////
	//void FreeSample(JSample *sample);

	//////////////////////////////////////////////////////////////////////////
	/// Play sound effect.
	/// 
	/// @param sample - Sound for playing.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void PlaySample(JSample *sample);
	
	//////////////////////////////////////////////////////////////////////////
	/// Set volume for audio playback.
	/// 
	/// @param volume - New volume.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void SetVolume(int volume);

protected:
	JSoundSystem();
	~JSoundSystem();
	
	void InitSoundSystem();
	void DestroySoundSystem();

private:
	
#ifdef WIN32

	JMusic *mCurrentMusic;

#endif

	int mVolume;
	int mChannel;

	static JSoundSystem* mInstance;

};

#endif
