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

#include "openalpp/netupdater.h"
#include "openalpp/netstream.h"
#include "openalpp/sample.h"

namespace openalpp {

NetStream::NetStream(ost::UDPSocket *socket,ost::TCPStream *controlsocket) 
  : Stream() {
  unsigned int frequency=11025,buffersize=2048;
  ALenum format=AL_FORMAT_MONO8;
  if(controlsocket) {
    *controlsocket >> format;
    *controlsocket >> frequency;
    *controlsocket >> buffersize;
  }
  updater_=new NetUpdater(socket,controlsocket,
			  buffername_,buffer2_->GetName(),
			  format,frequency,buffersize*SampleSize(format));
}

NetStream::NetStream(ost::UDPSocket *socket,SampleFormat format,
		     unsigned int frequency,unsigned int buffersize) 
  : Stream() {
  ALenum alformat=0;
  switch(format) {
    case(Mono8):
      alformat=AL_FORMAT_MONO8;
      break;
    case(Mono16):
      alformat=AL_FORMAT_MONO16;
      break;
    case(Stereo8):
      alformat=AL_FORMAT_STEREO8;
      break;
    case(Stereo16):
      alformat=AL_FORMAT_STEREO16;
      break;
  }
  updater_=new NetUpdater(socket,NULL,buffername_,buffer2_->GetName(),
			  alformat,frequency,buffersize*SampleSize(format));
}


NetStream::NetStream(const NetStream &stream) : Stream((const Stream &)stream){
}

NetStream::~NetStream() {
}

NetStream &NetStream::operator=(const NetStream &stream) {
  if(&stream!=this) {
    Stream::operator=((const Stream &)stream);
  }
  return *this;
}

}

