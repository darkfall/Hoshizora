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

#include "openalpp/filestreamupdater.h"

using namespace openalpp;

FileStreamUpdater::FileStreamUpdater(
             const OggVorbis_File &oggfile,
				     const ALuint buffer1,ALuint buffer2,
				     ALenum format,unsigned int frequency,
				     unsigned int buffersize)
  : StreamUpdater(buffer1,buffer2,format,frequency),buffersize_(buffersize),
    looping_(false) {
  oggfile_=new OggVorbis_File(oggfile);
}

FileStreamUpdater::~FileStreamUpdater() {
  if(oggfile_)
    ov_clear(oggfile_);
  delete oggfile_;
}

void FileStreamUpdater::run() {
  ALshort *buffer=new ALshort[buffersize_/sizeof(ALshort)];

  runmutex_.lock();
  while(!stoprunning_) {
    runmutex_.unlock();

    unsigned int count=0;
    int stream;
    while(count<buffersize_) {
      unsigned int amt;
      do {
	      amt=ov_read(oggfile_,&((char *)buffer)[count],
		    buffersize_-count,
		    0,2,1,&stream);
	      if(looping_ && amt==0) {
	        if(!ov_seekable(oggfile_))
	          break;
	        if(!ov_time_seek(oggfile_,0.0))
	          break;
	      }
      } while(looping_ && amt==0);
      // We must break if:
      // * An error occurred
      // * We hit EOF and the file was not looping 
      // * We hit EOF and the file was looping, but we couldn't loop...
      if(amt<=0)
	      break;
      count+=amt;
    }

    if(count)
      update(buffer,count);

    runmutex_.lock();
  }

  runmutex_.unlock();
  delete []buffer;
}

void FileStreamUpdater::setLooping(bool loop) {
  looping_=loop;
}
