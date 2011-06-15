#ifndef SORA_JAPP_H_
#define SORA_JAPP_H_

#include "JGE/include/jge.h"
#include "JGE/include/jApp.h"

#include "SoraTimer.h"

namespace sora {

	class SoraJGEApp: public JApp {
	public:
		void Create();
		void Destroy();
		
		void Update();
		void Render();
		
		void Pause();
		void Resume();
		
		void SetTimer(SoraTimer* timer);
		
	private:
		SoraTimer* mTimer;	
	};


} // namespace sora


#endif // SORA_JAPP_H_