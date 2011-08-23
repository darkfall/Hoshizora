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

#ifndef SOURCE_H_INCLUDED_C419B739
#define SOURCE_H_INCLUDED_C419B739

#include "openalpp/export.h"
#include "openalpp/stream.h"
#include "openalpp/sourcebase.h"
#include "openalpp/sample.h"

namespace openalpp {

/**
 * Class for "normal" sources.
 * This is used for standard OpenAL sources, whether streaming or not.
 */
class OPENALPP_API Source : public SourceBase {
 public:
  /**
   * Constructor.
   * Creates the source with the specified position.
   * @param x x coordinate.
   * @param y y coordinate.
   * @param z z coordinate.
   */
  Source(float x = 0.0, float y = 0.0, float z = 0.0);
  
  /**
   * Constructor.
   * Creates the source and a buffer with the specified file.
   * @param filename is the name of the file.
   */
  Source(const std::string& filename,float x=0.0,float y=0.0,float z=0.0);

  /**
   * Constructor.
   * Creates the source with the specified buffer.
   * @param buffer is the buffer to use.
   */
  Source(const Sample &buffer,float x=0.0,float y=0.0,float z=0.0);

  /**
   * Constructor.
   * Creates the source with the specified stream.
   * @param stream is the stream to use.
   */
  Source(const Stream &stream,float x=0.0,float y=0.0,float z=0.0);

  /**
   * Copy constructor.
   */
  Source(const Source &source);
    

  /**
   * Create a buffer for the source and load a file into it.
   * The source should _not_ be playing when doing this.
   * @param filename is the name of the file.
   */
  void setSound(const std::string& filename);

  /**
   * Sets a new buffer for the source.
   * The source should _not_ be playing when doing this.
   * @param buffer is the new buffer.
   */
  void setSound(const Sample *buffer);

  /**
   * Sets a new (streamed) buffer for the source.
   * The source should _not_ be playing when doing this.
   * @param stream is the new buffer.
   */
  void setSound(const Stream *stream);

  /**
   * Gets the buffer associated with the source.
   * @return the buffer.
   */
  const SoundData *getSound() const;

  /**
   * Play a file on the source.
   * This will change the source's buffer.
   * @param filename is the name of the file to play.
   */
  void play(const std::string& filename);

  /**
   * Play a buffer on the source.
   * This will change the source's buffer.
   * @param buffer is the buffer to play.
   */
  void play(const Sample *buffer);
    
  /**
   * Play a stream on the source.
   * This will change the source's buffer.
   * @param stream is the stream to play.
   */
  void play(const Stream *stream);
    
  /**
   * Play this source.
   * This is only here, because the above Play(...) hides SourceBase::Play()
   */
  void play();

  /**
   * Stop this source.
   * This is needed here for streaming sources...
   */
  void stop();

  /**
   * Pause this source.
   * This is needed here for streaming sources...
   */
  void pause();


  /**
   * Check if the source is streaming.
   * @return true if the source is streaming, false otherwise.
   */
  bool isStreaming();
    
  /**
   * Assignment operator.
   */
  Source &operator=(const Source &source);

  protected:
  /**
   * Destructor.
   */
  virtual ~Source();

 private:
  /**
   * Pointer to the SoundData (buffer) associated with this source.
   */
   openalpp::ref_ptr<SoundData> sounddata_;
};

}

#endif /* SOURCE_H_INCLUDED_C419B739 */
