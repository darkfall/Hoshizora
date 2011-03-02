/*
 *  mRenderSystem.cpp
 *  OpenGLES Study 1
 *
 *  Created by DarkfallGriffin on 7/8/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "mRenderSystem.h"

#include <math.h>
#include "mSprite.h"
#include "Wrapper.h"

#include "SoraPlatform.h"
namespace sora {

static int mVertexCount = 0;
static GLfloat mVertices[MAX_VERTEX_BUFFER<<1];
static GLfloat mUVs[MAX_VERTEX_BUFFER<<1];
static GLfloat mColors[MAX_VERTEX_BUFFER<<2];


//void glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);
//在这个函数中，有六下参数，这六个参数，指的是到眼睛的平面距离，
//具体参见
//http://blog.csdn.net/huawenguang/archive/2007/03/30/1546150.aspx

static void InitPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    GLfloat xmin, xmax, ymin, ymax;
	
    ymax = zNear * (GLfloat)tan(fovy * 3.1415962f / 360.0);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;
	
    glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
}

mRenderSystem* mRenderSystem::mInstance = 0;
mRenderSystem* mRenderSystem::getInstance() {
	if(mInstance == NULL) {
		mInstance = new mRenderSystem();
		mInstance->initSystem();
	}
	return mInstance;
}

void mRenderSystem::destroy() {
	if(mInstance) {
	//	mInstance->shutdownSystem();
		if(mInstance)
			delete mInstance;
	}
}

mRenderSystem::mRenderSystem() {
	mCurrentTex = -1;
	mLinearFiltering = true;
	mUseColorPointer = false;
	
	mScreenWidth = SORA_IOS_WINDOW_WIDTH;
	mScreenHeight = SORA_IOS_WINDOW_HEIGHT;
	
	orientation = mViewModeNormal;
	
	shiftx = shifty = 0;
	shiftSpeed = 0;
	shiftMode = mScreenShiftNone;
	of = cf = 0;
}

void mRenderSystem::enableLinearFiltering(bool flag) {
	if(mLinearFiltering != flag) {
		mCurrentTex = -1;
		mLinearFiltering = flag;
	}
}

void mRenderSystem::initSystem() {
	float aspect = (float)mScreenWidth/(float)mScreenHeight;
	
	glViewport          (0, 0, mScreenWidth, mScreenHeight);
    glScissor           (0, 0, mScreenWidth, mScreenHeight);
	
	glMatrixMode        (GL_MODELVIEW);
    glLoadIdentity      ();
	
    glHint              (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
    glShadeModel        (GL_SMOOTH);
    glDisable           (GL_DITHER);
    glClearColor        (0.0f, 0.0f, 0.0f, 0.0f );
	
    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	
	glEnable			(GL_TEXTURE_2D);
	
	glEnable			(GL_BLEND);
	glBlendFunc			(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable			(GL_SCISSOR_TEST);
	
    glMatrixMode        (GL_PROJECTION);
    glLoadIdentity      ();
	
    InitPerspective     (60.f, aspect, 0.1f, 1000.f);
}

void mRenderSystem::switchTo2D() {
	flush();
	
	glViewport				(0, 0, mScreenWidth, mScreenHeight);
	
	glMatrixMode			(GL_PROJECTION);
    glLoadIdentity			();
	glOrthof				(0, (float)mScreenWidth, 0, (float)mScreenHeight, 0, 100);
    glMatrixMode			(GL_MODELVIEW);
	glLoadIdentity			();
	
	glEnable				(GL_BLEND);
	glEnable				(GL_TEXTURE_2D);
	glDisable				(GL_CULL_FACE);
	glDisable				(GL_DEPTH_TEST);
	glDisable				(GL_LIGHTING);
	glDisableClientState	(GL_NORMAL_ARRAY);
}

void mRenderSystem::switchTo3D() {
	flush();
	
	float aspect = (float)mScreenWidth/(float)mScreenHeight;
	
	glViewport          (0, 0, mScreenWidth, mScreenHeight);
    glScissor           (0, 0, mScreenWidth, mScreenHeight);
	
    glMatrixMode        (GL_PROJECTION);
    glLoadIdentity      ();
    InitPerspective     (60.f, aspect, 0.1f, 1000.f);
    
    glMatrixMode        (GL_MODELVIEW);
    glLoadIdentity      ();
	
	glEnable			(GL_CULL_FACE);
	glEnable			(GL_DEPTH_TEST);
}
void mRenderSystem::flush() {
	if (mVertexCount > 0) {
		glVertexPointer(2, GL_FLOAT, 0, mVertices);
		glTexCoordPointer(2, GL_FLOAT, 0, mUVs);
		
		if (mUseColorPointer)
			glColorPointer (4, GL_FLOAT, 0, mColors);
		
		glDrawArrays(GL_TRIANGLES, 0, mVertexCount);
		mVertexCount = 0;
	}
}

void mRenderSystem::bindTexture(SoraTexture *texture) {
	if (mCurrentTex != texture->mTextureID) {
		flush();
		
		glBindTexture(GL_TEXTURE_2D, texture->mTextureID);
		mCurrentTex = texture->mTextureID;
		
		if (mLinearFiltering) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}
}

void Swap(float *a, float *b) {
	float n=*a;
	*a = *b;
	*b = n;
}

void mRenderSystem::renderImage(mSprite* quad, float xo, float yo, float angle) {
	bindTexture(quad->texture);
	
	yo = mScreenHeight - yo;
	
	float width = quad->mWidth*quad->xScale;
	float height = quad->mHeight*quad->yScale;
	float x = xo - quad->centerX*quad->xScale;
	float y = yo + quad->centerY*quad->yScale;
	
	float xx, yy;
	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);
	
	GLfloat uvs[] = {
		quad->mTX0, quad->mTY0,
		quad->mTX1, quad->mTY0,
		quad->mTX0, quad->mTY1,
		quad->mTX1, quad->mTY1,
		quad->mTX0, quad->mTY1,
		quad->mTX1, quad->mTY0
	};
	
	
	if (quad->mHFlipped) {
		uvs[ 0] = quad->mTX1;
		uvs[ 2] = quad->mTX0;
		uvs[ 4] = quad->mTX1;
		uvs[ 6] = quad->mTX0;
		uvs[ 8] = quad->mTX1;
		uvs[10] = quad->mTX0;
	}
	
	if (quad->mVFlipped) {
		uvs[ 1] = quad->mTY1;
		uvs[ 3] = quad->mTY1;
		uvs[ 5] = quad->mTY0;
		uvs[ 7] = quad->mTY0;
		uvs[ 9] = quad->mTY0;
		uvs[11] = quad->mTY1;
	}
	
	GLfloat vertices[] = {
		x,			y,
		x+width,	y,
		x,			y-height,
		x+width,	y-height,
		0, 0,
		0, 0
	};
	
	int idx = 8;
	if (angle != 0.0f) {
		idx = 0;
		
		for (int i=0;i<4;i++) {
			xx = (cosAngle*(vertices[idx]-xo) - sinAngle*(vertices[idx+1]-yo) + xo);
			yy = (sinAngle*(vertices[idx]-xo) + cosAngle*(vertices[idx+1]-yo) + yo); 
			
			vertices[idx] = xx;
			vertices[idx+1] = yy;
			idx += 2;
		}
		
	}
	
	vertices[idx++] = vertices[4];
	vertices[idx++] = vertices[5];
	
	vertices[idx++] = vertices[2];
	vertices[idx++] = vertices[3];
	
	if (mVertexCount >= MAX_VERTEX_BUFFER-6)
		flush();
	
	
	int u = 0;
	idx = 0;
	for (int i=0;i<6;i++) {
		mVertices[(mVertexCount<<1)] = vertices[idx++];
		mVertices[(mVertexCount<<1)+1] = vertices[idx++];
		mUVs[(mVertexCount<<1)] = uvs[u++];
		mUVs[(mVertexCount<<1)+1] = uvs[u++];
		if (mUseColorPointer) {
			mColors[(mVertexCount<<2)] = quad->mColor.r;
			mColors[(mVertexCount<<2)+1] = quad->mColor.g;
			mColors[(mVertexCount<<2)+2] = quad->mColor.b;
			mColors[(mVertexCount<<2)+3] = quad->mColor.a;
		}
		mVertexCount++;
		
	}
}


void mRenderSystem::renderImage(mSprite* quad, float xo, float yo, float xScale, float yScale, float angle) {
	bindTexture(quad->texture);
	
	yo = mScreenHeight - yo;
	
	float width = quad->mWidth*xScale;
	float height = quad->mHeight*yScale;
	float x = xo - quad->centerX*xScale+shiftx;
	float y = yo + quad->centerY*yScale+shifty;
	
	float xx, yy;
	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);
	
	GLfloat uvs[] = {
		quad->mTX0, quad->mTY0,
		quad->mTX1, quad->mTY0,
		quad->mTX0, quad->mTY1,
		quad->mTX1, quad->mTY1,
		quad->mTX0, quad->mTY1,
		quad->mTX1, quad->mTY0
	};
	
	
	if (quad->mHFlipped) {
		uvs[ 0] = quad->mTX1;
		uvs[ 2] = quad->mTX0;
		uvs[ 4] = quad->mTX1;
		uvs[ 6] = quad->mTX0;
		uvs[ 8] = quad->mTX1;
		uvs[10] = quad->mTX0;
	}
	
	if (quad->mVFlipped) {
		uvs[ 1] = quad->mTY1;
		uvs[ 3] = quad->mTY1;
		uvs[ 5] = quad->mTY0;
		uvs[ 7] = quad->mTY0;
		uvs[ 9] = quad->mTY0;
		uvs[11] = quad->mTY1;
	}
	
	GLfloat vertices[] = {
		x,			y,
		x+width,	y,
		x,			y-height,
		x+width,	y-height,
		0, 0,
		0, 0
	};
	
	int idx = 8;
	if (angle != 0.0f) {
		idx = 0;
		
		for (int i=0;i<4;i++) {
			xx = (cosAngle*(vertices[idx]-xo) - sinAngle*(vertices[idx+1]-yo) + xo);
			yy = (sinAngle*(vertices[idx]-xo) + cosAngle*(vertices[idx+1]-yo) + yo); 
			
			vertices[idx] = xx;
			vertices[idx+1] = yy;
			idx += 2;
		}
		
	}
	
	vertices[idx++] = vertices[4];
	vertices[idx++] = vertices[5];
	
	vertices[idx++] = vertices[2];
	vertices[idx++] = vertices[3];
	
	if (mVertexCount >= MAX_VERTEX_BUFFER-6)
		flush();
	
	
	int u = 0;
	idx = 0;
	for (int i=0;i<6;i++) {
		mVertices[(mVertexCount<<1)] = vertices[idx++];
		mVertices[(mVertexCount<<1)+1] = vertices[idx++];
		mUVs[(mVertexCount<<1)] = uvs[u++];
		mUVs[(mVertexCount<<1)+1] = uvs[u++];
		if (mUseColorPointer) {
			mColors[(mVertexCount<<2)] = quad->mColor.r;
			mColors[(mVertexCount<<2)+1] = quad->mColor.g;
			mColors[(mVertexCount<<2)+2] = quad->mColor.b;
			mColors[(mVertexCount<<2)+3] = quad->mColor.a;
		}
		mVertexCount++;
		
	}
}

void mRenderSystem::renderImage(mSprite* quad) {
	if(!quad->texture)
		return;
	
	if(quad->mColor.a == 0)
		return;
	
	bindTexture(quad->texture);
	float xo = quad->cX;
	float yo = quad->cY;
	
	
	
	yo = mScreenHeight - yo;
	
	float width = quad->mWidth*quad->xScale;
	float height = quad->mHeight*quad->yScale;
	float x = xo - quad->centerX*quad->xScale+shiftx;
	float y = yo + quad->centerY*quad->yScale+shifty;
	
	float xx, yy;
	float cosAngle = cosf(quad->degree);
	float sinAngle = sinf(quad->degree);
	
	GLfloat uvs[] = {
		quad->mTX0, quad->mTY0,
		quad->mTX1, quad->mTY0,
		quad->mTX0, quad->mTY1,
		quad->mTX1, quad->mTY1,
		quad->mTX0, quad->mTY1,
		quad->mTX1, quad->mTY0
	};
	
	
	if (quad->mHFlipped) {
		uvs[ 0] = quad->mTX1;
		uvs[ 2] = quad->mTX0;
		uvs[ 4] = quad->mTX1;
		uvs[ 6] = quad->mTX0;
		uvs[ 8] = quad->mTX1;
		uvs[10] = quad->mTX0;
	}
	
	if (quad->mVFlipped) {
		uvs[ 1] = quad->mTY1;
		uvs[ 3] = quad->mTY1;
		uvs[ 5] = quad->mTY0;
		uvs[ 7] = quad->mTY0;
		uvs[ 9] = quad->mTY0;
		uvs[11] = quad->mTY1;
	}
	
	GLfloat vertices[] = {
		x,			y,
		x+width,	y,
		x,			y-height,
		x+width,	y-height,
		0, 0,
		0, 0
	};
	
	int idx = 8;
	if (quad->degree != 0.0f) {
		idx = 0;
		
		for (int i=0;i<4;i++) {
			xx = (cosAngle*(vertices[idx]-xo) - sinAngle*(vertices[idx+1]-yo) + xo);
			yy = (sinAngle*(vertices[idx]-xo) + cosAngle*(vertices[idx+1]-yo) + yo); 
			
			vertices[idx] = xx;
			vertices[idx+1] = yy;
			idx += 2;
		}
		
	}
	
	vertices[idx++] = vertices[4];
	vertices[idx++] = vertices[5];
	
	vertices[idx++] = vertices[2];
	vertices[idx++] = vertices[3];
	
	if (mVertexCount >= MAX_VERTEX_BUFFER-6)
		flush();
	
	
	int u = 0;
	idx = 0;
	for (int i=0;i<6;i++) {
		mVertices[(mVertexCount<<1)] = vertices[idx++];
		mVertices[(mVertexCount<<1)+1] = vertices[idx++];
		mUVs[(mVertexCount<<1)] = uvs[u++];
		mUVs[(mVertexCount<<1)+1] = uvs[u++];
		if (mUseColorPointer) {
			mColors[(mVertexCount<<2)] = quad->mColor.r;
			mColors[(mVertexCount<<2)+1] = quad->mColor.g;
			mColors[(mVertexCount<<2)+2] = quad->mColor.b;
			mColors[(mVertexCount<<2)+3] = quad->mColor.a;
		}
		mVertexCount++;
		
	}
	
}

void mRenderSystem::setColor(float r, float g, float b, float a) {
	flush();
	
	glColor4f(r, g, b, a);
}

void mRenderSystem::setAlpha(float a) {
	flush();
	
	glColor4f(1.f, 1.f, 1.f, a);
}


void mRenderSystem::enableColorPointer(bool flag) {
	flush();
	
	if (flag) {
		mUseColorPointer = true;
		glEnableClientState(GL_COLOR_ARRAY);
	}
	else {
		mUseColorPointer = false;
		glDisableClientState(GL_COLOR_ARRAY);
	}
}


void mRenderSystem::enableAddictiveDraw(bool flag) {
	flush();
	
	if (flag)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	else
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
}

void mRenderSystem::setViewMode(mViewMode viewMode) {
	orientation = viewMode;
	switch(viewMode) {
		case mViewModeLandscapeRight:
			[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationLandscapeRight animated:YES];
			break;
			
		case mViewModeLandscapeLeft:
			[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationLandscapeLeft animated:YES];
			break;
			
		default:
			//[[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationPortrait animated:NO];
			break;
	}
}

void mRenderSystem::shift(size_t frame, float max, mScreenShiftMode mode) {
	shiftMode = mode;
	shiftSpeed = 10.f;
	shiftx = shifty = 0;
	of = frame; cf = 0;
	shiftMax = max;
	up = true;
}

void mRenderSystem::renderBegin() {
	glLoadIdentity();
	
	switch (orientation) {
		case mViewModeLandscapeRight:
			glTranslatef(-(mScreenHeight-mScreenWidth), mScreenHeight, 0);
			glRotatef(-90, 0, 0, 1);
			break;
			
		case mViewModeLandscapeLeft:
			glTranslatef(mScreenHeight, 0, 0);
			glRotatef(90, 0, 0, 1);
			break;
	}
	
	switch (shiftMode) {
		case mScreenShiftVerticle:
			if(up)
				shifty += shiftSpeed;
			else {
				shifty -= shiftSpeed;
			}
			
			if(shifty  > shiftMax) 
				up = false;
			else if(shifty < -shiftMax)
				up = true;
			
			//shiftMax -= shiftMax/of;
			
			++cf;
			if(cf > of) {
				shiftMode = mScreenShiftNone;
				shifty = shiftx = 0;
			}
			break;
			
		case mScreenShiftHorzion:
			if(up)
				shiftx += shiftSpeed;
			else {
				shiftx -= shiftSpeed;
			}
			
			if(shiftx  > shiftMax) 
				up = false;
			else if(shiftx < -shiftMax)
				up = true;
			
			//shiftMax -= shiftMax/of;
			
			++cf;
			if(cf > of) {
				shiftMode = mScreenShiftNone;
				shifty = shiftx = 0;
			}
			
			break;
			
		default:
			break;
	}
	
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
}


void mRenderSystem::renderEnd() {
	flush();
}
	
} // namespace 
