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

#ifndef FILESTREAM_H_INCLUDED
#define FILESTREAM_H_INCLUDED

#include "openalpp/stream.h"
#include "openalpp/export.h"

namespace openalpp {

/**
 * Class for streaming audio data from a file. Presently it supports Ogg
 * Vorbis files (http://www.vorbis.com/).
 */
class OPENALPP_API  FileStream : public Stream {
 public:
  /**
   * Constructor. A FileError will be thrown if the file can't be opened or
   * isn't recognized.
   * @param filename is the name of the file to try to open.
   * @param buffersize is an optional parameter specifying how large the
   * buffer should be (in samples per second).
   */
   FileStream(const std::string& filename,const int buffersize=65536) 
    throw (NameError,InitError,FileError);

  /**
   * Copy constructor.
   */
  FileStream(const FileStream &stream);


  FileStream &operator=(const FileStream &stream);

  /**
   * Turn on/off looping.
   * @param loop is true if the stream should loop, false otherwise.
   */
  void setLooping(bool loop = true);
protected:
    /**
   * Destructor.
   */
  virtual ~FileStream();

};


}

#endif
