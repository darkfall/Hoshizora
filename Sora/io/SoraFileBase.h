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
		
		bool isOpen() const { 
            return mIsOpen;
        }
		void  setOpen(bool flag) { 
            mIsOpen = flag;
        }
        
        util::String getFilePath() const {
            return mFilePath;
        }
        
        void setFilePath(const util::String& path) {
            mFilePath = path;
        }

	private:
		util::String mFilePath;

		bool mIsOpen;
	};

} // namespace sora

#endif // SORA_FILE_H