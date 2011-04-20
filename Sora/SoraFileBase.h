#ifndef SORA_FILE_H
#define SORA_FILE_H

#include <fstream>

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraNamedObject.h"
//#include "SoraStringConv.h"

namespace sora {

	class SoraFileBase {
    public:
		SoraFileBase(): isopen(false) {}
		virtual ~SoraFileBase() {}

		virtual int32 readFile(const SoraWString& sFile) = 0;
		virtual int32 readFileMem(void* pstr, ulong32 filesize) = 0;
		virtual void  closeFile() = 0;
		
		bool  is_open() const { return isopen; }
		void  set_open(bool flag) { isopen = flag; }

	private:
		SoraWString sFilePath;

		bool isopen;
	};

} // namespace sora

#endif // SORA_FILE_H