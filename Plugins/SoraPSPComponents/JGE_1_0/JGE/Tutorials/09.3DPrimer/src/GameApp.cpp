//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#include <stdio.h>


#include <JGE.h>
#include <JRenderer.h>
#include <JLBFont.h>
#include <JSprite.h>
#include <JOBJModel.h>

#include "GameApp.h"

#include <JMD2Model.h>


GameApp::GameApp()
{
	mTexture = NULL;
	mBgTex = NULL;
	mBg = NULL;

	mPlayer = NULL;
	mGun = NULL;
}


GameApp::~GameApp()
{
}


void GameApp::Create()
{
	mAngle = 0.0f;
	mState = STATE_IDLE;

	JRenderer* renderer = JRenderer::GetInstance();
	mTexture = renderer->LoadTexture("box.png");
	mBgTex = renderer->LoadTexture("bg.png");
	mBg = new JQuad(mBgTex, 0, 0, 480, 272);

	mPlayer = new JMD2Model();
	mPlayer->Load("tris.md2", "abarlith.png");		// load a MD2 character

	mGun = new JMD2Model();
	mGun->Load("weapon.md2", "weapon.png");			// load the character's weapon

	if (mPlayer != NULL && mGun != NULL)
	{
		mPlayer->SetState(mState);
		mGun->SetState(mState);
	}

	renderer->SetFOV(60.0f);

}


void GameApp::Destroy()
{

	SAFE_DELETE(mTexture);
	SAFE_DELETE(mBgTex);
	SAFE_DELETE(mBg);
	SAFE_DELETE(mPlayer);
	SAFE_DELETE(mGun);

}


void GameApp::Update()
{

	JGE* engine = JGE::GetInstance();

	if (engine->GetButtonClick(PSP_CTRL_TRIANGLE))		// do a screen shot when the TRIANGLE button is pressed
	{
		char s[80];
		sprintf(s, "ms0:/screenshot.png");
		JRenderer::GetInstance()->ScreenShot(s);
	}

	if (engine->GetButtonClick(PSP_CTRL_CROSS))			// exit when the CROSS button is pressed
	{
		engine->End();
		return;
	}


	int mNewState = mState;
	if (engine->GetButtonClick(PSP_CTRL_UP) || engine->GetButtonClick(PSP_CTRL_LEFT))
	{
		mNewState--;
		if (mNewState < STATE_IDLE)
			mNewState = STATE_FALING_FORWARD_DYING;

	}
	else if (engine->GetButtonClick(PSP_CTRL_DOWN) || engine->GetButtonClick(PSP_CTRL_RIGHT))
	{
		mNewState++;
		if (mNewState > STATE_FALING_FORWARD_DYING)
			mNewState = STATE_IDLE;

	} 

	if (mNewState != mState && mPlayer != NULL && mGun != NULL)
	{
		mState = mNewState;
		mPlayer->SetState(mState);
		mGun->SetState(mState);
	}

	float dt = engine->GetDelta();	// get number of milliseconds passed since last frame

	mAngle += 1.0f*dt;				// the frame rate is not fixed by default, so we use dt to do adjustments
	if (mAngle > M_PI*2.0f)			// angles are in radian, so 2 PI is one full circle
		mAngle = 0.0f;

	mPlayer->Update(dt);			// update model animation
	mGun->Update(dt);

}


void GameApp::Render()
{

	JRenderer* renderer = JRenderer::GetInstance();

	renderer->Enable2D();
	renderer->RenderQuad(mBg, 0, 0);
	
	renderer->Enable3D();

	renderer->PushMatrix();
	renderer->Translate(30.0f, 0.0f, -60.0f);
	renderer->RotateX(-M_PI_2);						// adjust the model to the right orientation
	renderer->RotateZ(mAngle);

	mPlayer->Render();								// render the model
	mGun->Render();

 	renderer->PopMatrix();

	
	Vertex3D tris[] =								// define a 3D cube
	{
		// top
		{ 0.0f, 0.0f, -5.0f, 5.0f, 5.0f },		
		{ 1.0f, 0.0f, 5.0f, 5.0f, 5.0f },
		{ 1.0f, 1.0f, 5.0f, 5.0f, -5.0f },

		{ 1.0f, 1.0f, 5.0f, 5.0f, -5.0f },
		{ 0.0f, 1.0f, -5.0f, 5.0f, -5.0f },
		{ 0.0f, 0.0f, -5.0f, 5.0f, 5.0f },

		// front
		{ 0.0f, 0.0f, 5.0f, -5.0f, 5.0f },		
		{ 1.0f, 0.0f, 5.0f, 5.0f, 5.0f },
		{ 1.0f, 1.0f, -5.0f, 5.0f, 5.0f },

		{ 1.0f, 1.0f, -5.0f, 5.0f, 5.0f },
		{ 0.0f, 1.0f, -5.0f, -5.0f, 5.0f },
		{ 0.0f, 0.0f, 5.0f, -5.0f, 5.0f },

		// right
		{ 0.0f, 0.0f, 5.0f, 5.0f, -5.0f },
		{ 1.0f, 0.0f, 5.0f, 5.0f, 5.0f },
		{ 1.0f, 1.0f, 5.0f, -5.0f, 5.0f },
		
		{ 1.0f, 1.0f, 5.0f, -5.0f, 5.0f },
		{ 0.0f, 1.0f, 5.0f, -5.0f, -5.0f },
		{ 0.0f, 0.0f, 5.0f, 5.0f, -5.0f },

		// left
		{ 0.0f, 0.0f, -5.0f, -5.0f, 5.0f },
		{ 1.0f, 0.0f, -5.0f, 5.0f, 5.0f },
		{ 1.0f, 1.0f, -5.0f, 5.0f, -5.0f },

		{ 1.0f, 1.0f, -5.0f, 5.0f, -5.0f },
		{ 0.0f, 1.0f, -5.0f, -5.0f, -5.0f },
		{ 0.0f, 0.0f, -5.0f, -5.0f, 5.0f },

		// bottom
		{ 0.0f, 0.0f, 5.0f, -5.0f, 5.0f },
		{ 1.0f, 0.0f, -5.0f, -5.0f, 5.0f },
		{ 1.0f, 1.0f, -5.0f, -5.0f, -5.0f },

		{ 1.0f, 1.0f, -5.0f, -5.0f, -5.0f },
		{ 0.0f, 1.0f, 5.0f, -5.0f, -5.0f },
		{ 0.0f, 0.0f, 5.0f, -5.0f, 5.0f },
		
		// back
		{ 0.0f, 0.0f, 5.0f, 5.0f, -5.0f },
		{ 1.0f, 0.0f, 5.0f, -5.0f, -5.0f },
		{ 1.0f, 1.0f, -5.0f, -5.0f, -5.0f },

		{ 1.0f, 1.0f, -5.0f, -5.0f, -5.0f },
		{ 0.0f, 1.0f, -5.0f, 5.0f, -5.0f },
		{ 0.0f, 0.0f, 5.0f, 5.0f, -5.0f }
	};

	renderer->PushMatrix();
	renderer->Translate(-15.0f, 0.0f, -30.0f);
	renderer->RotateX(mAngle);
	renderer->RotateY(-mAngle);
	renderer->RotateZ(mAngle);
	renderer->RenderTriangles(mTexture, tris, 0, 12);		// render the 3D cube
	renderer->PopMatrix();
	
}


void GameApp::Pause()
{

}


void GameApp::Resume()
{

}
