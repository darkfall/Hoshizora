/*
 *  SoraINIFile.h
 *  OpenGLES Study 1
 *
 *  Created by Robert Bu on 09��8��30��.
 *  Copyright 2009 Robert Bu. All rights reserved.
 *
 */
#ifndef _SoraINI_H_
#define _SoraINI_H_

#include <iostream>
#include <string>
#include <vector>

#include <cstdio>

#include "SoraPlatform.h"
#include "SoraException.h"
#include "io/SoraFileBase.h"

namespace sora {

using std::vector;
using std::ifstream;
using std::ofstream;
using std::endl;

struct INISectorValue {
	util::String key;
	util::String value;
	
	INISectorValue(const util::String& _key, const util::String& _value): key(_key), value(_value) {} 
};

struct INISector {
	vector<INISectorValue> value;
	util::String name;
};

class SORA_API SoraINIFile: public SoraFileBase {
public:
	typedef vector<INISector>::iterator itSector;

	SoraINIFile();
	~SoraINIFile();
	
	int32 readFile(const util::String& path);
	int32 readFileMem(void* ptr, ulong32 size);
	void closeFile();
	
	int32		getInt(const util::String& section, const util::String& name, int32 iDefault=0);
	util::String  getString(const util::String& section, const util::String& name, const util::String& sDefault);
	float		getFloat(const util::String& section, const util::String& name, float fDefault=0.f);
	bool		getBool(const util::String& section, const util::String& name, bool bDefault=false);
	
	void writeInt(const util::String& section, const util::String& name, int32 iValue);
	void writeString(const util::String& section, const util::String& name, const util::String& sValue);
	void writeFloat(const util::String& section, const util::String& name, float fValue);
	
	vector<INISectorValue> getSector(const util::String& section);
	
	void Commit();

private:	
	itSector GetSectorByName(const util::String& section);
	
	util::String sFile;
	bool bFileWrite;
	bool bMemoryFile;
	
	vector<INISector> sec;
	INISector publicSec;
};

} //namespace sora

#endif
