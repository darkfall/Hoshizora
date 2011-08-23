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

#ifndef SOUNDDATA_H_INCLUDED_C4199A75
#define SOUNDDATA_H_INCLUDED_C4199A75

#include "openalpp/export.h"
#include "openalpp/error.h"
#include "openalpp/audiobase.h"
#include "openalpp/referenced.h"

#ifdef WIN32
// Ignore the dll interface warning using std::vector members
#pragma warning(disable : 4251)
#endif


namespace openalpp {

/**
 * Base class for sound data.
 */
class OPENALPP_API SoundData : public AudioBase {
 protected:
  /**
   * Protected class to handle generation/deletion of OpenAL buffers correctly.
   */
   class SoundBuffer : public openalpp::Referenced {
    ALuint buffername_;
  protected:
   virtual ~SoundBuffer();

  public:
    SoundBuffer() throw (NameError);
    ALuint getName() {return buffername_;}
  };
 public:
  /**
   * Get the OpenAL name for the buffer.
   * @return the OpenAL name.
   */
  ALuint getAlBuffer() const;

  /**
   * Constructor.
   */
  SoundData() throw (NameError,InitError);

  /**
   * Copy constructor.
   */
  SoundData(const SoundData &sounddata);

  /**
   * Assignment operator.
   */
  SoundData &operator=(const SoundData &sounddata);

protected:

  /**
   * Destructor.
   */
  virtual ~SoundData();

  /**
   * See class SoundBuffer comment.
   */
  openalpp::ref_ptr<SoundBuffer> buffer_;

  /**
   * OpenAL name for the buffer.
   */
  ALuint buffername_;
};

}

#endif /* SOUNDDATA_H_INCLUDED_C4199A75 */
