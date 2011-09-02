//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#ifndef _GAMEAPP_H_
#define _GAMEAPP_H_

#include <JApp.h>

class JQuad;
class JSprite;
class JTexture;
class JMD2Model;

class GameApp:	public JApp
{
private:
	JTexture* mTexture;
	JTexture* mBgTex;
	JQuad* mBg;

	JMD2Model *mPlayer;
	JMD2Model *mGun;

	int mState;
	float mAngle;
	
public:
	GameApp();
	virtual ~GameApp();
	virtual void Create();
	virtual void Destroy();
	virtual void Update();
	virtual void Render();
	virtual void Pause();
	virtual void Resume();

};



#endif
