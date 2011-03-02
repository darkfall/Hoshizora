#ifndef SORA_TYPE_INFO
#define SORA_TYPE_INFO

#include "SoraPlatform.h"

#include <typeinfo>

namespace sora {

	class SoraTypeInfo {
	public:
		explicit SoraTypeInfo(const std::type_info& info) : _typeInfo(info) {};

		bool operator < (const SoraTypeInfo& rhs) const {
			return _typeInfo.before(rhs._typeInfo) != 0;
		}

	private:
		const std::type_info& _typeInfo;
	};

} // namespace sora

#endif