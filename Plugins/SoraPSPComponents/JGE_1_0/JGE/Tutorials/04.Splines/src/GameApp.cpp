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
#include <JSpline.h>
#include <JGameObject.h>

#include "GameApp.h"


GameApp::GameApp()
{
	mFont = NULL;
	mSpline = NULL;
	mTexBg = NULL;
	mTexPlane = NULL;
	mBg = NULL;
	mPlane = NULL;

}


GameApp::~GameApp()
{
}


void GameApp::Create()
{

	mFont = new JLBFont("f3", 16, true);

	mSpline = new JSpline();						// Load spline and scale it down a little bit because the 
	mSpline->Load("sample.spl", 0.60f, 0.56f);		// spline was originally created for 800x600 size screen.
	mSpline->GeneratePixels();						// Generate all the points on the spline.

	JRenderer* renderer = JRenderer::GetInstance();
	
	mTexBg = renderer->LoadTexture("bg.jpg");
	mBg = new JQuad(mTexBg, 0, 0, SCREEN_WIDTH_F, SCREEN_HEIGHT_F);

	mTexPlane = renderer->LoadTexture("sprites.png");		// Load the plane and set up the animation frames.
	mPlane = new JGameObject(mTexPlane, 1, 1, 63, 63);
	mPlane->AddFrame(1,65,63,63);
	mPlane->AddFrame(1,129,63,63);
	mPlane->SetHotSpot(32, 32);

	mPlane->SetRenderFlags(RENDER_FLAG_ANGLE);				// Make the plane facing the direction it is moving to.


	mIndex = 0;
	mCounter = 0.0;

	Point pt;
	mSpline->GetPixel(pt, mIndex);
	mPlane->SetPosition(pt.x, pt.y);

	mSpline->GetPixel(pt, mIndex+1);
	mPlane->SetDirection(pt.x, pt.y);

	mShowSpline = false;

}


void GameApp::Destroy()
{

	SAFE_DELETE(mFont);
	SAFE_DELETE(mSpline);
	SAFE_DELETE(mTexBg);
	SAFE_DELETE(mTexPlane);
	SAFE_DELETE(mBg);
	SAFE_DELETE(mPlane);

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

	if (engine->GetButtonClick(PSP_CTRL_CIRCLE))
	{
		mShowSpline = !mShowSpline;
	}

	float dt = engine->GetDelta();

	mPlane->Update(dt);

	mCounter += 100*dt;
	int curr = (int) mCounter;
	if (mIndex != curr)									// Update position of the plane
	{
		if (curr >= mSpline->GetPixelCount())
		{
			mCounter = 0.0f;
			curr = 0;
		}

		mIndex = curr;

		Point pt;
		mSpline->GetPixel(pt, mIndex);
		mPlane->SetPosition(pt.x, pt.y);

		mSpline->GetPixel(pt, mIndex+1);
		mPlane->SetDirection(pt.x, pt.y);
	}

}


void GameApp::Render()
{

	JRenderer* renderer = JRenderer::GetInstance();

	renderer->RenderQuad(mBg, 0, 0);
	
 	mFont->SetColor(ARGB(255,255,255,255));
 	mFont->DrawString("Press O to show spline", 5.0f, 130.0f, JGETEXT_LEFT); 

	if (mShowSpline)
		mSpline->Render(0, 0);

	mPlane->Render();

}


void GameApp::Pause()
{

}


void GameApp::Resume()
{

}
