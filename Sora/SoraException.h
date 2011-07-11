/*
 *  SoraException.h
 *  Plugin Concept
 *
 *  Created by griffin clare on 8/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SORA_EXCEPTION_H
#define SORA_EXCEPTION_H

#include "SoraPlatform.h"

#define SORA_EXCEPTION(mess) sora::SoraException(mess,   \
								__FUNCTION__,          \
								__FILE__,              \
								__LINE__)

#ifdef SORA_USE_EXCEPTION

#define THROW_SORA_EXCEPTION(mess) throw sora::SoraException(mess, \
									__FUNCTION__, \
									__FILE__, \
									__LINE__)

#else

#define THROW_SORA_EXCEPTION(mssg) \
	DebugPtr->log(vamssg("^s, function: %s, file: %s, line: %s", mssg, __FUNCTION__, __FILE__, __LINE__)); 

#endif

#include "SoraPlatform.h"
#include "SoraStringConv.h"

namespace sora {
	class SORA_API SoraException {
	public:
		SoraException(const SoraString& mssg, const SoraString& function, const SoraString& file, const SoraString& line):
				mMssg(mssg), mFunction(function), mFile(file), mLine(line) {}
		SoraException(const SoraWString& mssg, const SoraString& function, const SoraString& file, const SoraString& line):
				mMssg(ws2s(mssg)), mFunction(function), mFile(file), mLine(line) {}
		SoraException(const SoraString& mssg, const SoraString& function, const SoraString& file, int line):
				mMssg(mssg), mFunction(function), mFile(file), mLine(int_to_str(line)) {}
		SoraException(const SoraString& mssg, const SoraString& function, const SoraString& file):
				mMssg(mssg), mFunction(function), mFile(file), mLine("?") {}
		SoraException(const SoraString& mssg, const SoraString& function):
				mMssg(mssg), mFunction(function) {}
		SoraException(const SoraString& mssg):
				mMssg(mssg), mFunction("?"), mFile("?"), mLine("?") {}
		
		// get a formatted exception mssg
		SoraString what() const {
			SoraString fmssg("SoraException:"+mMssg);
			if(mFunction[0] != '?') fmssg += "\nFunction:"+mFunction;
			if(mFile[0] != '?') fmssg += "\nFile:"+mFile;
			if(mLine[0] != '?') fmssg += "\nLine:"+mLine;
			
			return fmssg;
		}
		const SoraString& mssg() const		{ return mMssg; }
		const SoraString& function() const	{ return mFunction; }
		const SoraString& file() const		{ return mFile; }
		const SoraString& line() const		{ return mLine; }
		uint32 nline() const { return atoi(mLine.c_str()); }
        
        /*
         may not work for every platform
         */
        static SoraString stackTrace();
		
	private:
		SoraString mMssg, mFunction, mFile, mLine;
	};
} // namespace sora

#endif