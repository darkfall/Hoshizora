/**
 * OpenAL++ - an object oriented toolkit for spatial sound
 * Copyright (C) 2002 VRlab, Ume� University
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

#include "openalpp/stream.h"
#include "openalpp/netstream.h"

namespace openalpp {

Stream::Stream() throw (NameError) : SoundData() {
  throw InitError("No streaming support compiled in.");
}

Stream::Stream(const Stream &stream) : SoundData((const SoundData &)stream) {
}

Stream &Stream::operator=(const Stream &) {
  return *this;
}

Stream::~Stream() {
}

void Stream::record(ALuint) {
}

void Stream::stop(ALuint) {
}

/*NetStream::NetStream(ost::UDPSocket *socket,ost::TCPStream *controlsocket) 
  : Stream() {
}

NetStream::NetStream(ost::UDPSocket *socket,SampleFormat format,
		     unsigned int frequency,unsigned int packetsize) 
  : Stream() {
}*/


NetStream::NetStream(const NetStream &stream) : Stream((const Stream &)stream){
}

NetStream::~NetStream() {
}

NetStream &NetStream::operator=(const NetStream&) {
  return *this;
}

}
