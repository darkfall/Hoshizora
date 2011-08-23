#include "SoraException.h"

#ifdef __GNUC__
#include <execinfo.h>
#elif defined(OS_WIN32)
#include "WindowsStackTrace.h"
#endif

namespace sora {
    
    std::string SoraException::stackTrace() {
        std::string mStackTrace;
#ifdef __GNUC__
        const int len = 200;
        void* buffer[len];
        int nptrs = backtrace(buffer, len);
        char** strings = backtrace_symbols(buffer, nptrs);
        if (strings) {
            for (int i = 0; i < nptrs; ++i) {
                mStackTrace.append(strings[i]);
                mStackTrace.push_back('\n');
            }
            free(strings);
        }
#elif defined(OS_WIN32)
        mStackTrace = getStackTrace();
#endif
        return mStackTrace;
    }
    
    const char* SoraException::what() const throw() {
        std::ostringstream mssg;
        mssg << name() << ": " << mMssg;
        if(mFunction[0] != '?') mssg << "\nFunction:" << mFunction;
        if(mFile[0] != '?') mssg << "\nFile:" << mFile;
        if(mLine != -1) mssg << "\nLine:" << mLine;
        
        return mssg.str().c_str();
    }
    
    const char* SoraException::name() const throw() {
        return "SoraException";
    }
    
    const char* SoraException::className() const throw() {
        return typeid(*this).name();
    }
    
    const std::string& SoraException::mssg() const { 
        return mMssg;
    }
    
    const std::string& SoraException::function() const { 
        return mFunction;
    }
    
    const std::string& SoraException::file() const {
        return mFile;
    }
    
    int32 SoraException::line() const { 
        return mLine;
    }
    
    void SoraException::rethrow() {
        throw *this;
    }
    
    SORA_EXCEPTION_IMPL(LogicException,                 SoraException,      "Logic exception")
    SORA_EXCEPTION_IMPL(AssertionViolationException,    LogicException,     "Assertion violation")
    SORA_EXCEPTION_IMPL(NullPointerException,           LogicException,     "Null pointer")
    SORA_EXCEPTION_IMPL(NullValueException,             LogicException,     "Null value")
    SORA_EXCEPTION_IMPL(BugcheckException,              LogicException,     "Bugcheck")
    SORA_EXCEPTION_IMPL(InvalidArgumentException,       LogicException,     "Invalid argument")
    SORA_EXCEPTION_IMPL(NotImplementedException,        LogicException,     "Not implemented")
    SORA_EXCEPTION_IMPL(RangeException,                 LogicException,     "Out of range")
    SORA_EXCEPTION_IMPL(IllegalStateException,          LogicException,     "Illegal state")
    SORA_EXCEPTION_IMPL(InvalidAccessException,         LogicException,     "Invalid access")
    SORA_EXCEPTION_IMPL(SignalException,                LogicException,     "Signal received")
    SORA_EXCEPTION_IMPL(UnhandledException,             LogicException,     "Unhandled exception")
    
    SORA_EXCEPTION_IMPL(RuntimeException,               SoraException,      "Runtime exception")
    SORA_EXCEPTION_IMPL(NotFoundException,              RuntimeException,   "Item Not found")
    SORA_EXCEPTION_IMPL(ExistsException,                RuntimeException,   "Item Exists")
    SORA_EXCEPTION_IMPL(TimeoutException,               RuntimeException,   "Timeout")
    SORA_EXCEPTION_IMPL(SystemException,                RuntimeException,   "System exception")
    SORA_EXCEPTION_IMPL(RegularExpressionException,     RuntimeException,   "Error in regular expression")
    SORA_EXCEPTION_IMPL(LibraryLoadException,           RuntimeException,   "Cannot load library")
    SORA_EXCEPTION_IMPL(LibraryAlreadyLoadedException,  RuntimeException,   "Library already loaded")
    SORA_EXCEPTION_IMPL(NoThreadAvailableException,     RuntimeException,   "No thread available")
    SORA_EXCEPTION_IMPL(PropertyNotSupportedException,  RuntimeException,   "Property not supported")
    SORA_EXCEPTION_IMPL(PoolOverflowException,          RuntimeException,   "Pool overflow")
    SORA_EXCEPTION_IMPL(NoPermissionException,          RuntimeException,   "No permission")
    SORA_EXCEPTION_IMPL(OutOfMemoryException,           RuntimeException,   "Out of memory")
    SORA_EXCEPTION_IMPL(DataException,                  RuntimeException,   "Data error")
    
    SORA_EXCEPTION_IMPL(DataFormatException,        DataException,      "Bad data format")
    SORA_EXCEPTION_IMPL(SyntaxException,            DataException,      "Syntax error")
    SORA_EXCEPTION_IMPL(CircularReferenceException, DataException,      "Circular reference")
    SORA_EXCEPTION_IMPL(PathSyntaxException,        SyntaxException,    "Bad path syntax")
    SORA_EXCEPTION_IMPL(IOException,                RuntimeException,   "I/O error")
    SORA_EXCEPTION_IMPL(ProtocolException,          IOException,        "Protocol error")
    SORA_EXCEPTION_IMPL(FileException,              IOException,        "File access error")
    SORA_EXCEPTION_IMPL(FileExistsException,        FileException,      "File exists")
    SORA_EXCEPTION_IMPL(FileNotFoundException,      FileException,      "File not found")
    SORA_EXCEPTION_IMPL(PathNotFoundException,      FileException,      "Path not found")
    SORA_EXCEPTION_IMPL(FileReadOnlyException,      FileException,      "File is read-only")
    SORA_EXCEPTION_IMPL(FileAccessDeniedException,  FileException,      "Access to file denied")
    SORA_EXCEPTION_IMPL(CreateFileException,        FileException,      "Cannot create file")
    SORA_EXCEPTION_IMPL(OpenFileException,          FileException,      "Cannot open file")
    SORA_EXCEPTION_IMPL(WriteFileException,         FileException,      "Cannot write file")
    SORA_EXCEPTION_IMPL(ReadFileException,          FileException,      "Cannot read file")
    SORA_EXCEPTION_IMPL(UnknownURISchemeException,  RuntimeException,   "Unknown URI scheme")
    
    
    SORA_EXCEPTION_IMPL(ApplicationException,   SoraException,      "Application exception")
    SORA_EXCEPTION_IMPL(BadCastException,       RuntimeException,   "Bad cast exception")
    SORA_EXCEPTION_IMPL(RttiException,          RuntimeException,   "Rtti exception")

} // namespace sora