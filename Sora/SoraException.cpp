#include "SoraException.h"

#ifdef __GNUC__
#include <execinfo.h>
#elif defined(OS_WIN32)
#include "WindowsStackTrace.h"
#endif

namespace sora {

	 SoraString SoraException::stackTrace() {
		 SoraString mStackTrace;
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
} // namespace sora