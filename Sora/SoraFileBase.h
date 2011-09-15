#ifndef SORA_FILE_H
#define SORA_FILE_H

#include <fstream>

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraNamedObject.h"
//#include "SoraStringConv.h"

namespace sora {

	class SORA_API SoraFileBase {
    public:
		SoraFileBase(): isopen(false) {}
		virtual ~SoraFileBase() {}
		
		bool  is_open() const { return isopen; }
		void  set_open(bool flag) { isopen = flag; }

        void setFilePath(const SoraWString& path) { sFilePath = path; }
        const SoraWString& getFilePath() const { return sFilePath; }
        
	protected:
		SoraWString sFilePath;

		bool isopen;
	};

} // namespace sora

#endif // SORA_FILE_H