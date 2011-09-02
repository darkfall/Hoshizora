//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// Copyright (c) 2007 Sijiu Duan (a.k.a. Chi80) <sijiu49@gmail.com>
// 
//-------------------------------------------------------------------------------------

#include <stdio.h>

#include <JGE.h>
#include <JRenderer.h>
#include <JLBFont.h>
#include <JInputSystem.h>
#include <JGBKFont.h>

#include "GameApp.h"


//-------------------------------------------------------------------------------------
// Constructor. Variables can be initialized here.
//
//-------------------------------------------------------------------------------------
GameApp::GameApp()
{

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
	m_inputStatus=0;
	JInputSystem::GetInstance();
}


//-------------------------------------------------------------------------------------
// This is the clean up callback function. You should delete all your in-game 
// resources, for example texture and quads, here.
// 
//-------------------------------------------------------------------------------------
void GameApp::Destroy()
{
	JInputSystem::Destory();
}


//-------------------------------------------------------------------------------------
// This is the update callback function and is called at each update frame
// before rendering. You should update the game logic here.
//
//-------------------------------------------------------------------------------------
void GameApp::Update()
{

	JGE* engine = JGE::GetInstance();


	// if Application is in input mode.
	if(JInputSystem::GetInstance()->GetIsInputActive())
	{
		JInputSystem::GetInstance()->Update();
		return;
	}

	if (m_inputStatus == 1 && !JInputSystem::GetInstance()->GetIsInputActive())
	{
		m_inputStatus = 0;
		m_outstring=JInputSystem::GetInstance()->GetInputString();
	}

	// switch to input mode
	if (JGE::GetInstance()->GetButtonClick(PSP_CTRL_CIRCLE))
	{
		m_inputStatus = 1;
		JInputSystem::GetInstance()->EnableInputMode(JInputSystem::GetInstance()->GetInputString());
	}

	// do a screen shot when the PSP_CTRL_TRIANGLE button is pressed
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

	float dt = engine->GetDelta();		// Get time elapsed since last update.

	//
	// Your updating code here...
	//
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
	renderer->ClearScreen(ARGB(255,230,0,0));
	JRenderer::GetInstance()->EnableTextureFilter(false);

	if(JInputSystem::GetInstance()->GetIsInputActive())
	{

		JInputSystem::GetInstance()->Draw();
		JInputSystem::GetInstance()->GetFont12()->RenderString((BYTE*)"Analog Control(摇杆)＋×○△□",10,170);
		JInputSystem::GetInstance()->GetFont12()->RenderString((BYTE*)"L: Shift",10,10);
		JInputSystem::GetInstance()->GetFont12()->RenderString((BYTE*)"R: Eng/Chi/Num",395,10);
		JInputSystem::GetInstance()->GetFont12()->RenderString((BYTE*)"↓＋× Finish Input(结束输入)",210,100);

		return;
	}

	std::string outstr = "Inputs String (输入字符串):" + m_outstring;
	JInputSystem::GetInstance()->GetFont12()->RenderString((BYTE*)(outstr.c_str()),20,20);
	JInputSystem::GetInstance()->GetFont12()->RenderString((BYTE*)" 按   ○ 开始输入",210,110);
	JInputSystem::GetInstance()->GetFont12()->RenderString((BYTE*)"Press ○ to start",210,125);

	JInputSystem::GetInstance()->GetFont12()->RenderString((BYTE*)" 按   ×   退出",210,160);
	JInputSystem::GetInstance()->GetFont12()->RenderString((BYTE*)"Press × to exit",210,175);

	//
	// Your rendering code here...
	//

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
