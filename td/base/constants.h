//
//  Header.h
//  Hoshizora
//
//  Created by Robert Bu on 9/2/11.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
#ifndef HTL_CONSTANTS_h
#define HTL_CONSTANTS_h

#include <string>

namespace atd {
    
    static const char* SID_BASIC_BULLET = "basic";
    
    static const char* SID_EVENT_BULLET_SHOOT = "onshoot";
    
    static const char* SID_EVENT_DAMAGE = "ondamage";
    static const char* SID_EVENT_DEAD = "ondead";
    
    static const char* SID_EVENT_ENEMY_ENTER = "enemyenter";
    static const char* SID_EVENT_ENEMY_LEAVE = "enemyleave";
    static const char* SID_EVENT_ENEMY_DEATH = "enemydead";
    
    static const char* SID_EVENT_MOVE_END = "moveend";
    
    static const char* ANIM_MOVE_LEFT = "anim_ml";
    static const char* ANIM_MOVE_RIGHT = "anim_mr";
    static const char* ANIM_MOVE_UP = "anim_mu";
    static const char* ANIM_MOVE_DOWN = "anim_md";
}



#endif
