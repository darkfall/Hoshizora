#ifndef SORA_AUDIERE_DEVICE_H
#define SORA_AUDIERE_DEVICE_H

#include "SoraSingleton.h"

#include "audiere/audiere.h"

#ifdef WIN32
#pragma comment(lib, "audiere.lib")
#endif

namespace sora {

	static std::map<audiere::OutputStreamPtr, SoraMusicFile*> G_MUSIC_FILE_MAP;
	
	class myAudiereMusicCallback: public audiere::StopCallback {
	public:
		void __stdcall ref() {}
		void __stdcall unref() {}
		void __stdcall streamStopped(audiere::StopEvent* ev) {
			std::map<audiere::OutputStreamPtr, SoraMusicFile*>::iterator itMusicFile = G_MUSIC_FILE_MAP.find(ev->getOutputStream());
			if(itMusicFile != G_MUSIC_FILE_MAP.end()) {
				assert(itMusicFile->second != NULL);
				itMusicFile->second->publishEvent(ev->getReason()==audiere::StopEvent::STOP_CALLED?SORAPB_EV_PLAY_STOPPED:SORAPB_EV_PLAY_ENDED);
			}
		}
	};

	class SoraAudiereDevice: public SoraSingleton<SoraAudiereDevice> {
		friend class SoraSingleton<SoraAudiereDevice>;
	public:
		void Destroy() {

		}
		bool setupDevice(const char* name, const char* param) {
			device = audiere::OpenDevice(name, param);
			if(device) {
				device->registerCallback(&myCallback);
				return true;
			}
			return false;
		}
		void update() {
			if(device) device->update();
		}

		audiere::AudioDevicePtr getDevice() { return device; }

	private:
		audiere::AudioDevicePtr device;
		myAudiereMusicCallback myCallback;
	};
} // namespace sora

#endif