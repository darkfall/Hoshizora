//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------
//
// Adapted from HGE's tutorial03. To use HGE's helper classes, you need to include
// libhgetools in the PSP makefile.
//
//-------------------------------------------------------------------------------------

#include <stdio.h>

#include <JAssert.h>


#include <JGE.h>
#include <JRenderer.h>
#include <JSoundSystem.h>
#include <JLBFont.h>

#include <hge\hgeparticle.h>
#include <hge\hgefont.h>

#include "GameApp.h"




//-------------------------------------------------------------------------------------
// Constructor. Variables can be initialized here.
//
//-------------------------------------------------------------------------------------
GameApp::GameApp()
{
	mAngle = 0.0f;
	mTimer = 0.0f;

	for (int i=0;i<16;i++)
		mParticles[i] = NULL;

	mMovingParticleSys = NULL;
	mParticleSys = NULL;
	mTexture = NULL;
	mCircle = NULL;

	x = 100.0f;
	y = 100.0f;
	dx = 0.0f;
	dy = 0.0f;

	mSpeed = 90;
	mFriction = 0.98f;
}


//-------------------------------------------------------------------------------------
// Destructor.
//
//-------------------------------------------------------------------------------------
GameApp::~GameApp()
{

}


//-------------------------------------------------------------------------------------
// This is the init callback function. You should load and create your in-game 
// resources here.
// 
//-------------------------------------------------------------------------------------
void GameApp::Create()
{

	mTexture = JRenderer::GetInstance()->LoadTexture("particles.png", TEX_TYPE_USE_VRAM);
	
	// load all the particle images
	int n = 0;
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{
			mParticles[n] = new JQuad(mTexture, j*32, i*32, 32, 32);
			mParticles[n]->SetHotSpot(16,16);
			n++;
		}
	}

	mCircle =new JQuad(mTexture, 96, 64, 32, 32);
	mCircle->SetColor(ARGB(0xFF,0xFF,0xA0,0x00));
	mCircle->SetHotSpot(16,16);

	// the particle system that move with the circle
	mMovingParticleSys = new hgeParticleSystem("trail.psi", mParticles[5]);
	mMovingParticleSys->Fire();

	mCurrParticleSys = 0;
	mCurrParticle = 0;

	// Load a font
	mFont = new hgeFont("font2.fnt");

	LoadNewParticleSys();

	JSoundSystem* sound = JSoundSystem::GetInstance();

	if (sound)
 		mMusic = sound->LoadMusic("12thWarrior.mp3");	// Load a background music.

 	if (mMusic)
 		JSoundSystem::GetInstance()->PlayMusic(mMusic, true);
}


void GameApp::LoadNewParticleSys()
{
	if (mParticleSys)
	{
		delete mParticleSys;
		mParticleSys = NULL;
	}

	char s[80];
	sprintf(s, "particle%d.psi", (mCurrParticleSys+1));
	mParticleSys = new hgeParticleSystem(s, mParticles[mCurrParticle]);
	mParticleSys->MoveTo(SCREEN_WIDTH_F/2, SCREEN_HEIGHT_F/2);
	mParticleSys->Fire();
}


//-------------------------------------------------------------------------------------
// This is the clean up callback function. You should delete all your in-game 
// resources, for example texture and quads, here.
// 
//-------------------------------------------------------------------------------------
void GameApp::Destroy()
{

	SAFE_DELETE(mMovingParticleSys);
	
	SAFE_DELETE(mCircle);

	for (int i=0;i<16;i++)
		SAFE_DELETE(mParticles[i]);

	SAFE_DELETE(mParticleSys);

	SAFE_DELETE(mTexture);

	SAFE_DELETE(mFont);

	SAFE_DELETE(mMusic);
}


//-------------------------------------------------------------------------------------
// This is the update callback function and is called at each update frame
// before rendering. You should update the game logic here.
//
//-------------------------------------------------------------------------------------
void GameApp::Update()
{

	JGE* engine = JGE::GetInstance();

	// do a screen shot when the TRIANGLE button is pressed
	if (engine->GetButtonClick(PSP_CTRL_TRIANGLE))		
	{
		char s[80];

		// save screen shot to root of Memory Stick 
		sprintf(s, "ms0:/screenshot.png");				
		JRenderer::GetInstance()->ScreenShot(s);
	}

	// exit when the CROSS button is pressed
	if (engine->GetButtonClick(PSP_CTRL_CROSS))	
	{
		engine->End();
		return;
	}

	int quadIdx[] =
	{
		7, 15, 0, 5, 7, 0, 1, 15, 4
	};

	// change particle image if PSP_CTRL_LEFT or PSP_CTRL_RIGHT is down
	if (engine->GetButtonClick(PSP_CTRL_LEFT))
	{
		mCurrParticle--;
		if (mCurrParticle < 0)
			mCurrParticle = 15;

		mParticleSys->info.sprite = mParticles[mCurrParticle];
		mMovingParticleSys->info.sprite = mParticles[15-mCurrParticle];
	}
	if (engine->GetButtonClick(PSP_CTRL_RIGHT))
	{
		mCurrParticle++;
		if (mCurrParticle > 15)
			mCurrParticle = 0;

		mParticleSys->info.sprite = mParticles[mCurrParticle];
		mMovingParticleSys->info.sprite = mParticles[15-mCurrParticle];
	}

	// change particle system if PSP_CTRL_DOWN or PSP_CTRL_UP is down
	if (engine->GetButtonClick(PSP_CTRL_DOWN))
	{
		mCurrParticleSys--;
		if (mCurrParticleSys < 0)
			mCurrParticleSys = 8;

		mCurrParticle = quadIdx[mCurrParticleSys];
		LoadNewParticleSys();
	}
	if (engine->GetButtonClick(PSP_CTRL_UP))
	{
		mCurrParticleSys++;
		if (mCurrParticleSys > 8)
			mCurrParticleSys = 0;

		mCurrParticle = quadIdx[mCurrParticleSys];
		LoadNewParticleSys();
	}

	float dt = engine->GetDelta();		// Get time elapsed since last update.

	mTimer += dt;
	

	mAngle += 2.0f*dt;
	if (mAngle >= M_PI*2)
		mAngle = 0;

	if (mTimer > 0.01f)		// update the moving circle and particle at about 100fps
	{
		if (engine->GetAnalogX()<64) 
			dx-=mSpeed*mTimer;
		if (engine->GetAnalogX()>192) 
			dx+=mSpeed*mTimer;
		if (engine->GetAnalogY()<64) 
			dy-=mSpeed*mTimer;
		if (engine->GetAnalogY()>192) 
			dy+=mSpeed*mTimer;

		// Do some movement calculations and collision detection	
		dx *= mFriction;
		dy *= mFriction;
		x += dx; 
		y += dy;

		if (x>470) 
		{
			x=470-(x-470);
			dx=-dx;
		}
		if (x<16) 
		{
			x=16+16-x;
			dx=-dx;
		}
		if (y>260)
		{
			y=260-(y-260);
			dy=-dy;
		}
		if (y<16)
		{
			y=16+16-y;
			dy=-dy;
		}

		// Update particle system
		mMovingParticleSys->info.nEmission=(int) (dx*dx+dy*dy)*2;
		mMovingParticleSys->MoveTo(x,y);

		mTimer = 0.0f;
	
	}

	mMovingParticleSys->Update(dt);
	mParticleSys->Update(dt);
	
}


//-------------------------------------------------------------------------------------
// All rendering operations should be done in Render() only.
// 
//-------------------------------------------------------------------------------------
void GameApp::Render()
{

	// get JRenderer instance
	JRenderer* renderer = JRenderer::GetInstance();		

	// clear screen to black
	renderer->ClearScreen(ARGB(0,0,0,0));

	// render circle
	renderer->RenderQuad(mCircle, x, y);

	// set additive blending
	renderer->SetTexBlend(BLEND_SRC_ALPHA, BLEND_ONE);

	mParticleSys->Render();
	mMovingParticleSys->Render();

	// set normal blending
	renderer->SetTexBlend(BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA);

	// draw the current image used for the particle system shown in the middle
	renderer->FillRect(SCREEN_WIDTH_F-37, 3, 34, 34, ARGB(255,0,0,128));
	mParticles[mCurrParticle]->SetColor(ARGB(255,255,255,255));
	renderer->RenderQuad(mParticles[mCurrParticle], SCREEN_WIDTH_F-20, 20);

	// turn off bilinear filtering to render sharp text
	renderer->EnableTextureFilter(false);

	mFont->printf(5, 5, HGETEXT_LEFT, "[Left/Right] Change Particle Image");
	char s[80];
	mFont->printf(5, 20, HGETEXT_LEFT, "[Up/Down] Change Effect -> particle%d.psi", (mCurrParticleSys+1));
	mFont->printf(5, 35, HGETEXT_LEFT, "[Analog] Move Circle");

	renderer->EnableTextureFilter(true);
}


//-------------------------------------------------------------------------------------
// This function is called when the system wants to pause the game. You can set a flag
// here to stop the update loop and audio playback.
//
//-------------------------------------------------------------------------------------
void GameApp::Pause()
{
	
}


//-------------------------------------------------------------------------------------
// This function is called when the game returns from the pause state.
//
//-------------------------------------------------------------------------------------
void GameApp::Resume()
{
	
}
