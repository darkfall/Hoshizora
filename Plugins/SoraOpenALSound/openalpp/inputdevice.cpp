/**
 * OpenAL++ - an object oriented toolkit for spatial sound
 * Copyright (C) 2002 VRlab, Umeå University
 *
 * OpenAL++ was created using the libraries:
 *                 OpenAL (http://www.openal.org), 
 *              PortAudio (http://www.portaudio.com/), and
 *              CommonC++ (http://cplusplus.sourceforge.net/)
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

#include "openalpp/deviceupdater.h"
#include "openalpp/inputdevice.h"
#include "openalpp/sample.h"

#ifdef ALPP_USE_PORTAUDIO

#include <portaudio.h>

namespace openalpp {

// Static
int InputDevice::nobjects_=0;

void InputDevice::Init() {
  if(!nobjects_) {
    PaError err=Pa_Initialize();
    if(err!=paNoError)
      throw InitError("Error initializing input device");
  }
  nobjects_++;
}

InputDevice::InputDevice() {
  Init();
  updater_=new DeviceUpdater(-1,22050,10000,Mono16,buffername_,buffer2_->GetName());
}

InputDevice::InputDevice(int device,unsigned int samplerate,unsigned int buffersize,
			 SampleFormat format) {
  Init();

  updater_=new DeviceUpdater(device,samplerate,buffersize*SampleFormat(format),format,buffername_,buffer2_->GetName());
}

InputDevice::InputDevice(const InputDevice &input)
  : Stream((const Stream &)input) {
}

InputDevice &InputDevice::operator=(const InputDevice &input) {
  if(this!=&input) {
    Stream::operator=((const Stream &)input);
  }
  return *this;
}

InputDevice::~InputDevice() {
  nobjects_--;
  if(!nobjects_)
    Pa_Terminate();
}

}
#endif