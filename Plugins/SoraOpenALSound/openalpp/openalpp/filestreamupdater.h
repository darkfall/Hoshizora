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

#ifndef FILESTREAMUPDATER_H_INCLUDED
#define FILESTREAMUPDATER_H_INCLUDED
#include "openalpp/streamupdater.h"
#include "stdlib.h"
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include "openalpp/export.h"



namespace openalpp {

/**
 * Updater for streams from files.
 */
class OPENALPP_API FileStreamUpdater : public StreamUpdater {
  OggVorbis_File *oggfile_; // The file structure
  unsigned int buffersize_; // Size of the buffer in bytes
  bool looping_;            // Are we looping or not?
 public:
  /**
   * Constructor.
   * @param oggfile is the structure created by ov_open
   * @param buffer1 and...
   * @param buffer2 are the sound buffers to use.
   * @param format is the (OpenAL) format of the sound data.
   * @param frequency is the frequency of the sound data.
   * @param buffersize is the size of the buffer (in bytes)
   */
   FileStreamUpdater(const OggVorbis_File &oggfile,
		    const ALuint buffer1,ALuint buffer2,
		    ALenum format,unsigned int frequency,
		    unsigned int buffersize);


  /**
   * Inherited from Thread.
   * This will be called when the updater is Start():ed..
   */
   void run();

  /**
   * Turn on/off looping.
   * @param loop is true if the stream should loop, false otherwise.
   */
  void setLooping(bool loop = true);

  protected:
    /**
   * Destructor.
   */
   virtual ~FileStreamUpdater();
};

}

#endif
