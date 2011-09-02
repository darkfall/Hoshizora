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
#include "SoraKeyInfo.h"
#include "GL/glfw3.h"

#include <map>

namespace sora {
	void glfwKeyCallback(GLFWwindow window, int key, int action);

    void clearPoll();
    bool getEv(SoraKeyEvent& ev);

} // namespace sora
#endif

