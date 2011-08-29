//
//  UnitTestException.h
//  Sora
//
//  Created by Robert Bu on 8/6/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_UnitTestException_h
#define Sora_UnitTestException_h

#include "SoraUnit.h"

#include <string>

namespace SoraUnit {
    
    class UnitTestException: public std::exception {
    public:
        UnitTestException(const std::string& message = "",
                      long lineNumber = 0,
                      const std::string& filename = std::string()):
        mMessage(message),
        mLineNumber(lineNumber),
        mFileName(filename) {
            
        }
        
        UnitTestException(const UnitTestException& rhs) {
            mMessage = rhs.mMessage;
            mLineNumber = rhs.mLineNumber;
            mFileName = rhs.mFileName;
        }
        
        virtual ~UnitTestException() throw() {}
        
        const char* what() const throw() {
            std::ostringstream msg;
            msg << mMessage << " [Line: "<<mLineNumber<<"] "<<"[File: "<<mFileName<<"]";
            return msg.str().c_str();
        }
        
    private:
        std::string mMessage;
        long mLineNumber;
        std::string mFileName;
    };
    
    
    
} // namespace SoraUnit

#endif
