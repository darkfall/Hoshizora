#include "SoraJGEApp.h"
#include "SoraCore.h"

namespace sora {

	void SoraJGEApp::Create() {
	}
		
	void SoraJGEApp::Destroy() {
		SORA->shutDown();
	}
		
	void SoraJGEApp::Update() {
	//	if(mTimer->update()) {
			SORA->update();
	//	}
	}
		
	void SoraJGEApp::Render() {
	}
		
	void SoraJGEApp::Pause() {
		SORA->pause();
	}
		
	void SoraJGEApp::Resume() {
		SORA->resume();
	}
	
	void SoraJGEApp::SetTimer(SoraTimer* timer) {
		mTimer = timer;
	}

} // namespace sora