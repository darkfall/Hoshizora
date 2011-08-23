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

#include "openalpp/source.h"

using namespace openalpp;

Source::Source(float x, float y, float z) : SourceBase(x,y,z) {
  streaming_=false;
  sounddata_=NULL;
}

Source::Source(const std::string& filename,float x,float y,float z)
  : SourceBase(x,y,z) {
  streaming_=false;
  sounddata_=new Sample(filename);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->getAlBuffer());
}

Source::Source(const Sample &buffer,float x,float y,float z)
  : SourceBase(x,y,z) {
  streaming_=false;
  sounddata_=new Sample(buffer);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->getAlBuffer());
}


  Source::Source(const Stream &stream,float x,float y,float z) 
  : SourceBase(x,y,z) {
  streaming_=true;
  sounddata_=new Stream(stream);
  // Associate the source with a buffer, even if update will change this later on.
  // otherwise OpenAL will crash.
  alSourcei(sourcename_,AL_BUFFER,sounddata_->getAlBuffer());
}

Source::Source(const Source &source) : SourceBase(source) {
  streaming_=source.streaming_;
  if(streaming_)
    sounddata_=new Stream(*(const Stream *)source.getSound());
  else
    sounddata_=new Sample(*(const Sample *)source.getSound());
  alSourcei(sourcename_,AL_BUFFER,sounddata_->getAlBuffer());
}

Source::~Source() {
  stop();
}

// TODO: Add flag for wether the sound should be loaded. This is useful for
// dummy sources that will be used only for groupsources.
void Source::setSound(const std::string& filename) {
  streaming_=false;
  sounddata_=new Sample(filename);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->getAlBuffer());
}

void Source::setSound(const Sample *buffer) {
  streaming_=false;

  openalpp::ref_ptr<SoundData> tmp = sounddata_;
  sounddata_=new Sample(*buffer);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->getAlBuffer());
}

void Source::setSound(const Stream *stream) {
  streaming_=true;

  sounddata_=new Stream(*stream);
  alSourcei(sourcename_,AL_BUFFER,sounddata_->getAlBuffer());
}

const SoundData *Source::getSound() const {
  return sounddata_.get();
}

void Source::play(const std::string& filename) {
  setSound(filename);
  SourceBase::play();
}

void Source::play(const Sample *buffer) {
  setSound(buffer);
  SourceBase::play();
}

void Source::play(const Stream *stream) {
  alSourcei(sourcename_,AL_LOOPING,AL_FALSE); //Streaming sources can't loop...
  setSound(stream);
  ((Stream *)sounddata_.get())->record(sourcename_);
  SourceBase::play();
}

void Source::play() {
  if(streaming_) {
    alSourcei(sourcename_,AL_LOOPING,AL_FALSE); //Streaming sources can't loop...
    ((Stream *)sounddata_.get())->record(sourcename_);
  }
  SourceBase::play();
}

void Source::stop() {
  if(streaming_)
    ((Stream *)sounddata_.get())->stop(sourcename_);
  SourceBase::stop();
}

void Source::pause() {
  SourceBase::pause();
}


bool Source::isStreaming() {
  return streaming_;
}

Source &Source::operator=(const Source &source) {
  if(this!=&source) {
    SourceBase::operator=(source);

    streaming_=source.streaming_;
    if(streaming_)
      sounddata_=new Stream(*(Stream *)source.sounddata_.get());
    else
      sounddata_=new Sample(*(Sample *)source.sounddata_.get());
  }
  return *this;
}

