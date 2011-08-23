/**
 * OpenAL++ - an object oriented toolkit for spatial sound
 * Copyright (C) 2002 VRlab, Umeå University
 *
 * OpenAL++ was created using the libraries:
 *                 OpenAL (http://www.openal.org), 
 *              PortAudio (http://www.portaudio.com/), and
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#ifndef AUDIOBASE_H_INCLUDED_C41996D8
#define AUDIOBASE_H_INCLUDED_C41996D8

#include "openalpp/export.h"
#include <cstdlib>
extern "C" {
#include <AL/al.h>
#include <AL/alut.h>
#include <AL/alc.h>
}
#include "openalpp/windowsstuff.h"
#include "openalpp/error.h"
#include "openalpp/referenced.h"
#include "openalpp/ref_ptr.h"
/**
 * \mainpage
 * OpenAL++ is an object oriented API for OpenAL (www.openal.org). It also uses
 * PortAudio (www.portaudio.com) for audio capture.
 * In addition to the functionality of OpenAL and ALUT (the OpenAL utility
 * library).
 *
 * OpenAL++ has been tested on Windows and Linux. 
 *
 * Every class in OpenAL++ that is available to the user is a descendant of the
 * virtual base class AudioBase. This class takes care of initialization and
 * shutdown, so there is no need to do it explicitly.
 */

/**
 * Namespace for OpenAL++.
 */

namespace openalpp {

/**
 * Format for sound data. Mono/Stereo, 8 or 16 bits.
 */
typedef enum SampleFormat {Mono8,Stereo8,Mono16,Stereo16};

/** 
 * Base class for enviroment, listener and source classes.
 * Takes care of initialisation/shutdown of anything necessary (e.g. ALut)
 */
class OPENALPP_API AudioBase : public Referenced{
  /**
   * Counter for #instances for enviroment, listener and source classes.
   * Used to determine when init and shutdown functions should be called
   */
  static int instances_;

  /**
   * Pointer to device.
   */
  static ALCdevice *device_;

  /**
   * Pointer to context.
   */
#ifndef WIN32
  static void *context_;
#else
  static struct ALCcontext_struct *context_;
#endif
 protected:
  /**
   * Constructor.
   * @param frequency is the output frequency, in Hz.
   * @param refresh is the refresh rate, in Hz.
   * @param is a flag for syncronous context. Values <0 indicates that the
   * default should be used.
   */
   AudioBase(int frequency=-1,int refresh=-1,int synchronous=-1)
    throw (InitError);

  /**
   * Destructor.
   */
  virtual  ~AudioBase();

  /**
   * Flag for whether reverb has been initiated.
   * Reverb can be initiated with AudioEnviroment::InitiateReverb()
   */
  static bool reverbinitiated_;

  /**
   * Set reverb scale.
   * This pointer will be set by AudioEnviroment::InitiateReverb()
   * @param sid is the OpenAL name for the source
   * @param param is the reverb scale. Range [0.0,1.0].
   */
  static void (*alReverbScale)(ALuint sid, ALfloat param);

  /**
   * Set reverb delay.
   * This pointer will be set by AudioEnviroment::InitiateReverb()
   * @param sid is the OpenAL name for the source
   * @param param is the reverb delay. Range [0.0,2.0].
   */
  static void (*alReverbDelay)(ALuint sid, ALfloat param);
};

}

#endif /* AUDIOBASE_H_INCLUDED_C41996D8 */
