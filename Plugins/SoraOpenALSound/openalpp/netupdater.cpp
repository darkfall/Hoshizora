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

namespace openalpp {

NetUpdater::NetUpdater(ost::UDPSocket *socket,ost::TCPStream *controlsocket,
		       const ALuint buffer1,ALuint buffer2,
		       ALenum format,unsigned int frequency,
		       unsigned int buffersize) 
  : StreamUpdater(buffer1,buffer2,format,frequency), socket_(socket)
  , controlsocket_(controlsocket), buffersize_(buffersize) {
}

void NetUpdater::run() {
  char *buffer=new char[buffersize_];
  unsigned int len=0,received=0;

  runmutex_.enterMutex();
  while(!stoprunning_) {
    runmutex_.leaveMutex();
    // TODO: Make the timeout dependant on how long it would take to play
    // one buffer. If it takes too long, just fill up with zeros and Update.
    if(socket_->isPending(ost::Socket::pendingInput,100)) {
      len=socket_->receive(&(buffer[received]),buffersize_-received);
      received+=len;
      if(received>=buffersize_) {
	Update(buffer,received);
	received=0;
      }
    } else {
      // Timeout; fill the buffer with zeros (silence) and Update
      memset(&(buffer[received]),0,buffersize_-received);
      Update(buffer,buffersize_);
      received=0;

      // Take care of messages on the control socket (if it exists)
      if(controlsocket_ && 
	 controlsocket_->isPending(ost::Socket::pendingInput,100)) {
	char instr[100];
	*controlsocket_ >> instr;
	runmutex_.enterMutex();
	break;
      }
    }
    runmutex_.enterMutex();
  }
  runmutex_.leaveMutex();
  
  delete []buffer;
}

}
