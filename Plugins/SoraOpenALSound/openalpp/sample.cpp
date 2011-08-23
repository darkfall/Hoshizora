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

#include "openalpp/sample.h"
#include <sstream>

using namespace openalpp;

  Sample::Sample(const std::string& filename) throw (FileError)
  : SoundData(),filename_(filename) {
  ALsizei size,bits,freq;
  ALenum format,error;
  ALvoid *data;
  ALboolean success;

  success=alutLoadWAV(filename.c_str(),&data,&format,&size,&bits,&freq);
  if(success==AL_TRUE && data ) {
    alBufferData(buffername_,format,data,size,freq);
    if((error=alGetError())!=AL_FALSE)
      throw FileError("Error buffering sound");
    free(data);
  } else {
    std::ostringstream str;
    str << "Unable to load file: " << filename << std::ends;
    throw FileError(str.str().c_str());
  }
}

Sample::Sample(const Sample &sample)
  : SoundData(/*(const SoundData &)*/sample), filename_(sample.filename_) {
}

Sample::Sample(ALenum format,ALvoid* data,ALsizei size,ALsizei freq) throw (FileError)
   : SoundData() {
   ALenum error;
 
   alBufferData(buffername_,format,data,size,freq);
   if((error=alGetError())!=AL_FALSE)
     throw FileError("Error buffering sound");
 }

std::string Sample::getFileName() const {
  return filename_;
}


Sample::~Sample()
{

}

Sample &Sample::operator=(const Sample &sample) {
  if(this!=&sample) {
    SoundData::operator=(sample);
    filename_=sample.filename_;
  }
  return *this;
}

// A couple of utility functions. Might move them to their own file later...
unsigned int openalpp::sampleSize(SampleFormat format) {
  switch(format) {
    case(Mono8):
      return 1;
      break;
    case(Stereo8):
    case(Mono16):
      return 2;
      break;
    case(Stereo16):
      return 4;
      break;
    default:
      return 0;
  }
}

unsigned int openalpp::sampleSize(ALenum format) {
  switch(format) {
    case(AL_FORMAT_MONO8):
      return 1;
      break;
    case(AL_FORMAT_STEREO8):
    case(AL_FORMAT_MONO16):
      return 2;
      break;
    case(AL_FORMAT_STEREO16):
      return 4;
      break;
    default:
      return 0;
  }
}

