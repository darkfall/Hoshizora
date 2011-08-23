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

#ifndef STREAMUPDATER_H_INCLUDED_C419FA12
#define STREAMUPDATER_H_INCLUDED_C419FA12


//#include <cc++/thread.h>
#include <vector>

#ifdef WIN32
// Ignore the dll interface warning using std::vector members
#pragma warning(disable : 4251)
#endif

extern "C" {
#include <AL/al.h>
}

#include "openalpp/export.h"


#include "openalpp/audiobase.h"
#include "openalpp/error.h"
#include "openalpp/windowsstuff.h"
#include "OpenThreads/Thread"
#include "OpenThreads/Mutex"
#include "openalpp/referenced.h"

namespace openalpp {

/**
 * Base class for (threaded) updating of stream buffers.
 */
  class OPENALPP_API StreamUpdater : public OpenThreads::Thread, public OpenThreads::Mutex, public openalpp::Referenced { //ost::Mutex {
  /**
   * #references to this instance.
   * If zero => delete this.
   */
  //int nrefs_;
 public:
  /**
   * Constructor.
   * @param buffer1 and...
   * @param buffer2 are the buffers used for double-buffered streaming.
   * @param format is the (OpenAL) format of the sound.
   * @param frequency is the frequency of the sound.
   */
  StreamUpdater(ALuint buffer1,ALuint buffer2,
		ALenum format,unsigned int frequency);


  /**
   * Add a source to the stream.
   * @param sourcename is the OpenAL name of the source.
   */
  void addSource(ALuint sourcename);

  /**
   * Remove a source from the stream.
   * @param sourcename is the OpenAL name of the source.
   */
  void removeSource(ALuint sourcename);

  /**
   * Update the stream.
   * I.e. add new data to play.
   * @param buffer is a pointer to sound data.
   * @param length is the length of the sound data (in bytes).
   * @return done flag. I.e. stoprunning_.
   */
  bool update(void *buffer,unsigned int length); 

  /**
   * Inherited from Thread.
   * Is called after run() finishes, and deletes this.
   */
  void cancelCleanup();

  /**
   * Reference this updater.
   * @return this.
   */
  //StreamUpdater *reference();

  /**
   * Dereference this updater.
   */
//   void deReference() throw (FatalError);
 protected:

   /**
   * Destructor.
   */
  virtual ~StreamUpdater();

   
   /**
   * Names of the buffers to update.
   */
  ALuint buffers_[2];

  /**
   * OpenAL format of the sound data.
   */
  ALenum format_;

  /**
   * Frequency of the sound data.
   */
  unsigned int frequency_;

  /**
   * Source to update.
   */

   std::vector<ALuint> sources_,newsources_,removesources_;
  /**
   * Flag for when Run should stop running..
   */ 
  bool stoprunning_;

  /**
   * Mutex for stoprunning_.
   */
  OpenThreads::Mutex runmutex_;

};

}

#endif /* STREAMUPDATER_H_INCLUDED_C419FA12 */
