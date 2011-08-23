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

#include "openalpp/streamupdater.h"
#include "openalpp/stream.h"

using namespace openalpp;

Stream::Stream() throw (NameError) : SoundData() {
  buffer2_=new SoundData();
  updater_=NULL;
}

Stream::Stream(const Stream &stream) : SoundData((const SoundData &)stream) {
  buffer2_=stream.buffer2_;//->reference();
  updater_=stream.updater_;//->reference();
}

Stream &Stream::operator=(const Stream &stream) {
  if(this!=&stream) {
    SoundData::operator=((const SoundData &)stream);
    //buffer2_->deReference();
    buffer2_=stream.buffer2_;//->reference();
//    updater_;//->deReference();
    updater_=stream.updater_;//->reference();
  }
  return *this;
}

Stream::~Stream() {
  std::cerr << "~Stream: " << buffername_ << std::endl;

  //if(buffer2_)
  //  buffer2_->deReference();
  //if(updater_)
  //  updater_->deReference();
}

void Stream::record(ALuint sourcename) {
  if(!updater_)
    throw FatalError("No updater thread for stream!");
  updater_->addSource(sourcename);
}

void Stream::stop(ALuint sourcename) {
  if(!updater_)
    throw FatalError("No updater thread for stream!");
  updater_->removeSource(sourcename);
}
