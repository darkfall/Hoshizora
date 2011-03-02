#ifndef SORA_AUDIERE_DEVICE_H
#define SORA_AUDIERE_DEVICE_H

#include "SoraSingleton.h"

#include "audiere/audiere.h"

#ifdef WIN32
#pragma comment(lib, "audiere.lib")
#endif

namespace sora {
	
	using namespace audiere;

	class SoraAudiereDevice: public SoraSingleton<SoraAudiereDevice> {
		friend class SoraSingleton<SoraAudiereDevice>;
	public:
		void Destroy() {
			if(device)
				device->unref();
		}
		bool setupDevice(const char* name, const char* param) {
			device = OpenDevice(name, param);
			if(device) return true;
			return false;
		}
		void update() {
			if(device) device->update();
		}

		AudioDevicePtr getDevice() { return device; }

	private:
		AudioDevicePtr device;
	};
} // namespace sora

#endif