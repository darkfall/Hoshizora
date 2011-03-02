#ifndef SORA_STRING_H
#define SORA_STRING_H

#include <string>

#include "SoraPlatform.h"
#include "SoraException.h"

#include "SoraStringConv.h"

namespace sora {

	using std::string;
	using std::wstring;

	class SoraStringBase {
		enum SoraStringTpye {
			WSTRING = 1,
			STRING = 2,
		};

		enum SoraStringEncode {
			UTF8 = 1,
			SUNICODE = 2,
			ANSII = 3,
		};

	public:
		SoraStringBase() {
			iEncode = 0;
			iType = 0;
		}
		SoraStringBase(const char* pstr) {
			iEncode = ANSII;
			iType = STRING;

			sorastr = s2ws(pstr);
		}
		SoraStringBase(const wchar_t* pwstr, int encode=UTF8) {
			iEncode = encode;
			iType = WSTRING;

			sorastr = pwstr;
		}
		SoraStringBase(const string& str) {
			iEncode = ANSII;
			iType = STRING;

			sorastr = s2ws(str);
		}
		SoraStringBase(const wstring& wstr, int encode=UTF8) {
			iEncode = encode;
			iType = WSTRING;

			sorastr = wstr;
		}

//		const char* c_str() { return ws2s(sorastr).c_str(); }
//		const wchar_t* wc_str() { return sorastr.c_str(); }

		uint32 size() {
			switch(iEncode) {
			case ANSII:
				return sorastr.size();

			case UTF8:
			case SUNICODE:
				return sorastr.size();
			}
			return 0;
		}

	private:
		wstring sorastr;

		int32 iType;
		int32 iEncode;
	};

} // namespace sora

#endif // SORA_STRING_H