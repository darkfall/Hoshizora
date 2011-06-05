/*
 *  SoraOGLKeyPoll.h
 *  Sora
 *
 *  Created by GriffinBu on 12/12/10.
 *  Copyright 2010 GameMaster. All rights reserved.
 *
 */

#ifndef SORA_OGL_KEYPOLL_H_
#define SORA_OGL_KEYPOLL_H_

#include "SoraPlatform.h"
#include <map>
#include "SoraKeyInfo.h"

namespace sora {
	void glfwKeyCallback(int key, int action);

	int sorakey_to_glfwkey(int key);
	int glfwkey_to_sorakey(int key);

    void clearPoll();
    bool getEv(SoraKeyEvent& ev);

} // namespace sora
#endif

