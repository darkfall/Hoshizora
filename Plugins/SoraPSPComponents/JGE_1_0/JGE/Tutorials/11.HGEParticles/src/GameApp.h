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

class hgeParticleSystem;
class JTexture;
class JQuad;
class hgeFont;
class JMusic;

class GameApp:	public JApp
{
	
public:
	GameApp();
	virtual ~GameApp();
	virtual void Create();
	virtual void Destroy();
	virtual void Update();
	virtual void Render();
	virtual void Pause();
	virtual void Resume();

	void LoadNewParticleSys();

private:
	float mAngle;
	float mTimer;
	
	hgeFont* mFont;

	hgeParticleSystem*	mMovingParticleSys;
	hgeParticleSystem*	mParticleSys;
	JTexture *mTexture;
	JQuad *mCircle;

	JQuad *mParticles[16];

	int mCurrParticleSys;
	int mCurrParticle;

	float x;
	float y;
	float dx;
	float dy;

	float mSpeed;
	float mFriction;


	JMusic* mMusic;
};

#endif
