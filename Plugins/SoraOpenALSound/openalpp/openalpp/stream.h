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

#ifndef STREAM_H_INCLUDED_C41983DF
#define STREAM_H_INCLUDED_C41983DF

#include "openalpp/export.h"
#include "openalpp/sounddata.h"
#include "openalpp/ref_ptr.h"
#include "openalpp/streamupdater.h"

#ifdef WIN32
// Ignore the dll interface warning using std::vector members
#pragma warning(disable : 4251)
#endif


namespace openalpp {

//class StreamUpdater;

/**
 * Base class for NetStream and InputDevice.
 * Used for audio streams.
 */
class OPENALPP_API Stream : public SoundData {
 protected:
  /**
   * For double-buffering of sounds.
   */
  openalpp::ref_ptr<SoundData> buffer2_;

  openalpp::ref_ptr<StreamUpdater> updater_;
 public:
  /**
   * Default constructor.
   */
   Stream() throw (NameError);

  /**
   * Copy constructor.
   */
   Stream(const Stream &stream);

  /**
   * Assignment operator.
   */
   Stream &operator=(const Stream &stream);


  /**
   * Start recording.
   * I.e. start copying data to buffers.
   * @param sourcename is the (OpenAL) name of the source.
   */
   void record(ALuint sourcename);

  /**
   * Stop recording.
   * @param sourcename is the (OpenAL) name of the source.
   */
   void stop(ALuint sourcename);

protected:
   /**
   * Destructor.
   */
   virtual ~Stream();

};

}

#endif /* STREAM_H_INCLUDED_C41983DF */
