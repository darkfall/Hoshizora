//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
// Graphics copyright (c) 2007 Chen Chao <chenchao868@21cn.com>
//
//-------------------------------------------------------------------------------------

#include <stdio.h>

#include <JGE.h>
#include <JRenderer.h>
#include <JResourceManager.h>
#include <JAnimator.h>

#include "GameApp.h"


GameApp::GameApp()
{
	mResourceMgr = NULL;
	mLeft = NULL;
	mRight = NULL;

	mCurr = NULL;
}


GameApp::~GameApp()
{
}


void GameApp::Create()
{
	// JAnimator needs to get its resource from a JResourceManager
	mResourceMgr = new JResourceManager();
	mResourceMgr->LoadResource("animation.res");

	// create the first animator
	mLeft = new JAnimator(mResourceMgr);
	mLeft->Load("left.anm");

	// create the second animator
	mRight = new JAnimator(mResourceMgr);
	mRight->Load("right.anm");
	
	mLeft->SetHotSpot(79,126);
	mLeft->SetPosition(240,220);

	mRight->SetHotSpot(79,126);
	mRight->SetPosition(240,220);

	// start the animation
	mCurr = mLeft;
	mCurr->Start();
}


void GameApp::Destroy()
{

	if (mRight)
		delete mRight;

	if (mLeft)
		delete mLeft;

	if (mResourceMgr)
		delete mResourceMgr;


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

	float dt = engine->GetDelta();
	mCurr->Update(dt);

	// change to the other animation when the current one is done
	if (!mCurr->IsAnimating())
	{
		if (mCurr == mLeft)
			mCurr = mRight;
		else
			mCurr = mLeft;

		mCurr->Start();
	}
}


void GameApp::Render()
{

	JRenderer* renderer = JRenderer::GetInstance();
	
	// turn off bilinear filtering to have sharp image
	renderer->EnableTextureFilter(false);

	// render background
	renderer->RenderQuad(mResourceMgr->GetQuad("bg"), 480>>1, 272>>1);
	
	// render animation
	mCurr->Render();

}


void GameApp::Pause()
{

}


void GameApp::Resume()
{

}
