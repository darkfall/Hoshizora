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

#ifndef WINDOWSSTUFF_H_INCLUDED
#define WINDOWSSTUFF_H_INCLUDED
#ifndef WIN32
  
#include <unistd.h>

#else // WIN32

//C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#pragma warning( disable : 4290 )
#include "openalpp/export.h"

#include <windows.h>
#define usleep(x) Sleep((x)/1000)

#include <AL/alut.h>

#define alGetSourceiv alGetSourcei
inline OPENALPP_API ALboolean alutLoadWAV(const char *fname,ALvoid **buffer,ALint *format,
			     ALsizei *size,ALsizei *bits,ALsizei *freq) {
  ALboolean loop;
  alutLoadWAVFile(const_cast<char *>(fname),format,buffer,size,freq,&loop);
  if(*format==AL_FORMAT_STEREO16 || *format==AL_FORMAT_MONO16)
    *bits=16;
  else
    *bits=8;
  return AL_TRUE;
}

// This should take care of VC++ strange way of handling loop variables...
//#define for if(0); else for

#endif
#else
#endif
