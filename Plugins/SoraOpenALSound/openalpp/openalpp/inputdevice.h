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

#ifndef INPUTDEVICE_H_INCLUDED_C41987DC
#define INPUTDEVICE_H_INCLUDED_C41987DC

#include "openalpp/export.h"
#include "openalpp/stream.h"

namespace openalpp {

/**
 * Class for handling input devices, like microphones.
 */
class OPENALPP_API InputDevice : public Stream {
  static int nobjects_;

  /**
   * Initialize input. Called by constructor(s).
   */
  void init();
 public:
  /**
   * Constructor.
   */
  InputDevice();

  /**
   * Constructor.
   * @param device is the device to open. -1 for default input.
   * @param samplerate is the desired sample rate.
   * @param buffersize is the desired buffer size _in samples_.
   * @param format is the desired sample format.
   */
  InputDevice(int device,unsigned int samplerate,unsigned int buffersize=1024, 
	      SampleFormat format=Mono16);

  /**
   * Copy constructor.
   */
  InputDevice(const InputDevice &input);

  /**
   * Assignment operator.
   */
  InputDevice &operator=(const InputDevice &input);

protected:
  /**
   * Destructor.
   */
  virtual ~InputDevice();
};

}

#endif /* INPUTDEVICE_H_INCLUDED_C41987DC */
