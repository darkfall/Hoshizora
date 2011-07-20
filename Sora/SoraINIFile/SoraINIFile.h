/*
 *  SoraINIFile.h
 *  OpenGLES Study 1
 *
 *  Created by griffin clare on 09��8��30��.
 *  Copyright 2009 Robert Bu. All rights reserved.
 *
 */
#ifndef _SoraINI_H_
#define _SoraINI_H_

#include <iostream>
#include <string>
#include <vector>

#include <cstdio>

#include "../SoraPlatform.h"
#include "../SoraException.h"
#include "../SoraFileBase.h"

namespace sora {

using std::vector;
using std::ifstream;
using std::ofstream;
using std::endl;

struct INISectorValue {
	SoraString key;
	SoraString value;
	
	INISectorValue(const char* _key, const char* _value): key(_key), value(_value) {} 
};

struct INISector {
	vector<INISectorValue> value;
	SoraString name;
};

class SORA_API SoraINIFile: public SoraFileBase {
public:
	typedef vector<INISector>::iterator itSector;

	SoraINIFile();
	~SoraINIFile();
	
	int32 readFile(const wchar_t* path);
	int32 readFileMem(void* ptr, ulong32 size);
	void closeFile();
	
	int32		getInt(const char* section, const char* name, int32 iDefault=0);
	SoraString  getString(const char* section, const char* name, const char* sDefault);
	float32		getFloat(const char* section, const char* name, float32 fDefault=0.f);
	bool		getBool(const char* section, const char* name, bool bDefault=false);
	
	void writeInt(const char* section, const char* name, int32 iValue);
	void writeString(const char* section, const char* name, const char* sValue);
	void writeFloat(const char* section, const char* name, float32 fValue);
	
	vector<INISectorValue> getSector(const char* section);
	
	void Commit();

private:	
	itSector GetSectorByName(const char* section);
	
	SoraWString sFile;
	bool bFileWrite;
	bool bMemoryFile;
	
	vector<INISector> sec;
	INISector publicSec;
};

} //namespace sora

#endif
