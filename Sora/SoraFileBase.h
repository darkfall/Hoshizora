#ifndef SORA_FILE_H
#define SORA_FILE_H

#include <fstream>

#include "SoraPlatform.h"

namespace sora {

	class SORA_API SoraFileBase {
    public:
		SoraFileBase(): 
        mIsOpen(false) {
        }
		
        virtual ~SoraFileBase() {
        }
		
		bool is_open() const { 
            return mIsOpen;
        }
		void  set_open(bool flag) { 
            mIsOpen = flag;
        }
        
        SoraWString getFilePath() const {
            return mFilePath;
        }
        
        void setFilePath(const SoraWString& path) {
            mFilePath = path;
        }

	private:
		SoraWString mFilePath;

		bool mIsOpen;
	};

} // namespace sora

#endif // SORA_FILE_H