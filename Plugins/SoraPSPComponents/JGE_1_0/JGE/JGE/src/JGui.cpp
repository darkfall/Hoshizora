//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#include "../include/JGE.h"
#include "../include/JGui.h"

JGE* JGuiObject::mEngine = NULL;

JGE* JGuiController::mEngine = NULL;


JGuiObject::JGuiObject(int id): mId(id) 
{ 
	mEngine = JGE::GetInstance(); 
}


JGuiObject::~JGuiObject() 
{ 
//	JGERelease(); 
};


bool JGuiObject::Leaving(u32 key) 
{
	return true; 
}


bool JGuiObject::ButtonPressed() 
{ 
	return false; 
}


void JGuiObject::Entering()
{
	
}


int JGuiObject::GetId()
{
	return mId;
}


void JGuiObject::Update(float dt) 
{
}

JGuiController::JGuiController(int id, JGuiListener* listener) : mId(id), mListener(listener)
{
	mEngine = JGE::GetInstance();
	
	mBg = NULL;
	mShadingBg = NULL;

	mCount = 0;
	mCurr = 0;

	mCursorX = SCREEN_WIDTH/2;
	mCursorY = SCREEN_HEIGHT/2;
	mShowCursor = false;

	mActionButton = PSP_CTRL_CIRCLE;
	mLastKey = 0;

	mStyle = JGUI_STYLE_WRAPPING;

	mActive = true;
}


JGuiController::~JGuiController()
{
	for (int i=0;i<mCount;i++)
		if (mObjects[i]!=NULL)
			delete mObjects[i];

//	JGERelease();
}


void JGuiController::Render()
{
//	if (mShadingBg != NULL)
//		jge->Gfx_BlendRect(mShadingBg, mShadingColor);

//	if (mBg != NULL)
//		jge->Gfx_DrawImage(mBg, mBgX, mBgY);

	for (int i=0;i<mCount;i++)
		if (mObjects[i]!=NULL)
			mObjects[i]->Render();
}


bool JGuiController::KeyRepeated(u32 key, float dt)
{

	bool doKey = false;
	if (mLastKey != PSP_CTRL_UP)
	{
		mLastKey = PSP_CTRL_UP;
		doKey = true;
		mKeyRepeatDelay = JGUI_INITIAL_DELAY;
	}
	else
	{
		mKeyRepeatDelay -= dt;
		if (mKeyRepeatDelay <= 0.0f)
		{
			mKeyRepeatDelay = JGUI_REPEAT_DELAY;
			doKey = true;
		}
	}

	return doKey;
}


void JGuiController::Update(float dt)
{
	for (int i=0;i<mCount;i++)
		if (mObjects[i]!=NULL)
			mObjects[i]->Update(dt);

	if (mEngine->GetButtonClick(mActionButton))
	{
		if (mObjects[mCurr] != NULL && mObjects[mCurr]->ButtonPressed())
		{
			if (mListener != NULL)
			{
				mListener->ButtonPressed(mId, mObjects[mCurr]->GetId());
				return;
			}
		}
	}
	
	if (mEngine->GetButtonState(PSP_CTRL_LEFT) || mEngine->GetButtonState(PSP_CTRL_UP) || mEngine->GetAnalogY()<64 || mEngine->GetAnalogX()<64)
	{
		if (KeyRepeated(PSP_CTRL_UP, dt))
		{
			int n = mCurr;
			n--;
			if (n<0)
			{
				if ((mStyle&JGUI_STYLE_WRAPPING))
					n = mCount-1;
				else
					n = 0;
			}

			if (n != mCurr && mObjects[mCurr] != NULL && mObjects[mCurr]->Leaving(PSP_CTRL_UP))
			{
				mCurr = n;
				mObjects[mCurr]->Entering();
			}
		}
	}
	else if (mEngine->GetButtonState(PSP_CTRL_RIGHT) || mEngine->GetButtonState(PSP_CTRL_DOWN) || mEngine->GetAnalogY()>192 || mEngine->GetAnalogX()>192)
	{
		if (KeyRepeated(PSP_CTRL_DOWN, dt))
		{
			int n = mCurr;
			n++;
			if (n>mCount-1)
			{
				if ((mStyle&JGUI_STYLE_WRAPPING))
					n = 0;
				else
					n = mCount-1;
			}

			if (n != mCurr && mObjects[mCurr] != NULL && mObjects[mCurr]->Leaving(PSP_CTRL_DOWN))
			{
				mCurr = n;
				mObjects[mCurr]->Entering();
			}
		}
	}
	else
		mLastKey = 0;

}


void JGuiController::Add(JGuiObject* ctrl)
{
	if (mCount<MAX_GUIOBJECT)
	{
		mObjects[mCount++] = ctrl;
	}
}


void JGuiController::Remove(int id)
{
	for (int i=0;i<mCount;i++)
	{
		if (mObjects[i] != NULL && mObjects[i]->GetId()==id)
		{
			delete mObjects[i];
			
			for (int j=i;j<mCount-1;j++)
			{
				mObjects[j] = mObjects[j+1];
			}
			mCount--;
			return;
		}
	}
}

void JGuiController::SetActionButton(u32 button) { mActionButton = button; }
void JGuiController::SetStyle(int style) { mStyle = style;	}
void JGuiController::SetCursor(JSprite* cursor) { mCursor = cursor; }
bool JGuiController::IsActive() { return mActive; }
void JGuiController::SetActive(bool flag) { mActive = flag; }


//void JGuiController::SetImageBackground(const JTexture* tex, int x, int y)
//{
//	mBg = tex;
//	mBgX = x;
//	mBgY = y;
//}
//
//
//void JGuiController::SetShadingBackground(int x, int y, int width, int height, PIXEL_TYPE color)
//{
//	if (mShadingBg)
//		delete mShadingBg;
//	
//	mShadingBg = new Rect(x, y, width, height);
//	mShadingColor = color;
//}
//
