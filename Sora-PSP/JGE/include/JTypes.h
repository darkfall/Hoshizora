//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#ifndef _JTYPES_H
#define _JTYPES_H

#ifdef WIN32

#else

#include <pspgu.h>
#include <pspgum.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h> 
#include <pspctrl.h>
#include <time.h>
#include <string.h>
#include <pspaudiolib.h>
#include <psprtc.h>

#include "JAudio.h"

#endif


#define MAX_CHANNEL		128


#ifndef M_PI
#define M_PI	3.14159265358979323846f
#define M_PI_2	1.57079632679489661923f
#define M_PI_4	0.785398163397448309616f
#define M_1_PI	0.318309886183790671538f
#define M_2_PI	0.636619772367581343076f
#endif

#define RAD2DEG		57.29577951f
#define DEG2RAD		0.017453293f

#define SAFE_DELETE(x)			if (x) { delete x; x = NULL; }
#define SAFE_DELETE_ARRAY(x)	if (x) { delete [] x; x = NULL; }


#define SCREEN_WIDTH 			480
#define SCREEN_HEIGHT 			272
#define SCREEN_WIDTH_F 			480.0f
#define SCREEN_HEIGHT_F			272.0f

#ifdef WIN32
//	#define DEFAULT_BLEND		BLEND_DEFAULT
//	#define BLEND_OPTION_ADD	BLEND_COLORADD
//	#define BLEND_OPTION_BLEND	(BLEND_COLORADD | BLEND_ALPHABLEND | BLEND_NOZWRITE)
#else
	#define DEFAULT_BLEND		GU_TFX_MODULATE
	#define BLEND_OPTION_ADD	GU_TFX_ADD
	#define BLEND_OPTION_BLEND	GU_TFX_BLEND
#endif


#if defined (WIN32)

	#include <windows.h>
	#include <gl/gl.h>
	#include <gl/glu.h>

	#include "../Dependencies/include/fmod.h"

	//#include "../HGE/include/hge.h"
	//#include "../HGE/include/hgeSprite.h"

	//#define u8				BYTE
	//#define u16				WORD
	//#define u32				DWORD
	typedef signed char s8;
	typedef signed short s16;
	typedef signed long s32;
	typedef unsigned char u8;
	typedef unsigned short u16;
	typedef unsigned long u32;


	#define BLEND_ZERO					GL_ZERO
	#define BLEND_ONE					GL_ONE
	#define BLEND_SRC_COLOR				GL_SRC_COLOR
	#define BLEND_ONE_MINUS_SRC_COLOR	GL_ONE_MINUS_SRC_COLOR
	#define BLEND_SRC_ALPHA				GL_SRC_ALPHA
	#define BLEND_ONE_MINUS_SRC_ALPHA	GL_ONE_MINUS_SRC_ALPHA
	#define BLEND_DST_ALPHA				GL_DST_ALPHA
	#define BLEND_ONE_MINUS_DST_ALPHA	GL_ONE_MINUS_DST_ALPHA
	#define BLEND_DST_COLOR				GL_DST_COLOR
	#define BLEND_ONE_MINUS_DST_COLOR	GL_ONE_MINUS_DST_COLOR
	#define BLEND_SRC_ALPHA_SATURATE	GL_SRC_ALPHA_SATURATE

	#define ARGB(a, r, g, b)		((a << 24) | (r << 16) | (g << 8) | b)
	#define RGBA(r, g, b, a)		((a << 24) | (b << 16) | (g << 8) | r)

	enum PspCtrlButtons
	{
		PSP_CTRL_SELECT     = 0x000001,
		PSP_CTRL_START      = 0x000008,
		PSP_CTRL_UP         = 0x000010,
		PSP_CTRL_RIGHT      = 0x000020,
		PSP_CTRL_DOWN      	= 0x000040,
		PSP_CTRL_LEFT      	= 0x000080,
		PSP_CTRL_LTRIGGER   = 0x000100,
		PSP_CTRL_RTRIGGER   = 0x000200,
		PSP_CTRL_TRIANGLE   = 0x001000,
		PSP_CTRL_CIRCLE     = 0x002000,
		PSP_CTRL_CROSS      = 0x004000,
		PSP_CTRL_SQUARE     = 0x008000,
		PSP_CTRL_HOME       = 0x010000,
		PSP_CTRL_HOLD       = 0x020000,
		PSP_CTRL_NOTE       = 0x800000,
	};


	#define PIXEL_TYPE				DWORD

#else	// PSP

	#ifndef ABGR8888
	#define ABGR8888
	#endif


	#if defined (ABGR8888)
		#ifndef ARGB
		#define ARGB(a, r, g, b)		((a << 24) | (b << 16) | (g << 8) | r)	// macro to assemble pixels in correct format
		#endif
		#define MAKE_COLOR(a, c)		(a << 24 | c)
		#define MASK_ALPHA				0xFF000000							// masks for accessing individual pixels
		#define MASK_BLUE				0x00FF0000
		#define MASK_GREEN				0x0000FF00
		#define MASK_RED				0x000000FF
		#define PIXEL_TYPE				u32
		#define PIXEL_SIZE				4
		#define PIXEL_FORMAT			PSP_DISPLAY_PIXEL_FORMAT_8888

		#define	BUFFER_FORMAT			GU_PSM_8888
		#define TEXTURE_FORMAT			GU_PSM_8888
		#define TEXTURE_COLOR_FORMAT	GU_COLOR_8888


	#elif defined (ABGR5551)

		#ifndef ARGB
		#define ARGB(a, r, g, b)		((r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10) | ((a >> 7) << 15))
		#endif
		#define MAKE_COLOR(a, c)		(((a>>7)<<15) | c)
		#define MASK_ALPHA				0x8000
		#define MASK_BLUE				0x7C00
		#define MASK_GREEN				0x03E0
		#define MASK_RED				0x001F
		#define PIXEL_TYPE				u16
		#define PIXEL_SIZE				2
		#define PIXEL_FORMAT			PSP_DISPLAY_PIXEL_FORMAT_5551

		#define	BUFFER_FORMAT			GU_PSM_8888
		#define TEXTURE_FORMAT			GU_PSM_5551
		#define TEXTURE_COLOR_FORMAT	GU_COLOR_5551

	#elif defined (ABGR4444)
		#ifndef ARGB
		#define ARGB(a, r, g, b)		((r >> 4) | ((g >> 4) << 4) | ((b >> 4) << 8) | ((a >> 4) << 12))
		#endif
		#define MAKE_COLOR(a, c)		(((a>>4)<<12) | c)
		#define MASK_ALPHA				0xF000
		#define MASK_BLUE				0x0F00
		#define MASK_GREEN				0x00F0
		#define MASK_RED				0x000F
		#define PIXEL_TYPE				u16
		#define PIXEL_SIZE				2
		#define PIXEL_FORMAT			PSP_DISPLAY_PIXEL_FORMAT_4444

		#define	BUFFER_FORMAT			GU_PSM_4444
		#define TEXTURE_FORMAT			GU_PSM_4444
		#define TEXTURE_COLOR_FORMAT	GU_COLOR_4444

	#endif

	#define	FRAME_BUFFER_WIDTH 		512
	#define FRAME_BUFFER_SIZE		FRAME_BUFFER_WIDTH*SCREEN_HEIGHT*PIXEL_SIZE

	#define SLICE_SIZE_F			64.0f
	typedef unsigned long DWORD;

	#define BLEND_ZERO					0x1000
	#define BLEND_ONE					0x1002 
	#define BLEND_SRC_COLOR				GU_SRC_COLOR
	#define BLEND_ONE_MINUS_SRC_COLOR	GU_ONE_MINUS_SRC_COLOR
	#define BLEND_SRC_ALPHA				GU_SRC_ALPHA
	#define BLEND_ONE_MINUS_SRC_ALPHA	GU_ONE_MINUS_SRC_ALPHA
	#define BLEND_DST_ALPHA				GU_DST_ALPHA
	#define BLEND_ONE_MINUS_DST_ALPHA	GU_ONE_MINUS_DST_ALPHA
	#define BLEND_DST_COLOR				GU_DST_COLOR
	#define BLEND_ONE_MINUS_DST_COLOR	GU_ONE_MINUS_DST_COLOR
	#define BLEND_SRC_ALPHA_SATURATE	BLEND_ONE

	typedef struct
	{
		ScePspFVector2 texture;
		//PIXEL_TYPE color;
		//ScePspFVector3 normal;
		ScePspFVector3 pos;
	} PSPVertex3D; 


#endif




//------------------------------------------------------------------------------------------------
struct Vertex
{
	float u, v;
	PIXEL_TYPE color;
	float x, y, z;
};


//------------------------------------------------------------------------------------------------
struct Vertex3D
{
	float u, v;
	//float nx, ny, nz;
	float x, y, z;
};


//------------------------------------------------------------------------------------------------
struct VertexColor
{
	PIXEL_TYPE color;
	float x, y, z;
};


struct JColor
{
	union
	{
		struct
		{
			u8 b;
			u8 g;
			u8 r;
			u8 a;
		};
		DWORD color;
	};
};



enum
{
	TEX_TYPE_NONE,
	TEX_TYPE_USE_VRAM,
	TEX_TYPE_MIPMAP,
	TEX_TYPE_NORMAL,
	TEX_TYPE_SKYBOX
};


enum
{
	MODE_UNKNOWN,
	MODE_2D,
	MODE_3D
};


enum
{
	TEX_FILTER_NONE,
	TEX_FILTER_LINEAR,
	TEX_FILTER_NEAREST
};

//------------------------------------------------------------------------------------------------
class JTexture
{
public:
	JTexture();
	~JTexture();

	void UpdateBits(int x, int y, int width, int height, PIXEL_TYPE* bits);

	int mWidth;
	int mHeight;
	int mTexWidth;
	int mTexHeight;

	int mFilter;

#ifdef WIN32
	GLuint mTexId;
#else

	int mTexId;
	bool mInVideoRAM;
	PIXEL_TYPE* mBits;
#endif
};


//////////////////////////////////////////////////////////////////////////
/// Custom filter for processing the texture image while loading. You 
/// can change the pixels by using a custom filter before the image is
/// created as a texture.
/// 
//////////////////////////////////////////////////////////////////////////
class JImageFilter
{
public:

	//////////////////////////////////////////////////////////////////////////
	/// Pure virtual function for the custom filter to implement.
	///
	/// @param pix - Image data.
	/// @param width - Width of the image.
	/// @param height - Height of the image.
	///
	//////////////////////////////////////////////////////////////////////////
	virtual void ProcessImage(PIXEL_TYPE* pix, int width, int height) = 0;
};


//////////////////////////////////////////////////////////////////////////
/// Image quad.
/// 
//////////////////////////////////////////////////////////////////////////
class JQuad
{
public:

	//////////////////////////////////////////////////////////////////////////
	/// Constructor.
	/// 
	/// @param tex - Texture of the quad.
	/// @param x - X position of the quad in texture.
	/// @param y - Y position of the quad in texture.
	/// @param width - Width of the quad.
	/// @param height - Height of the quad.
	/// 
	//////////////////////////////////////////////////////////////////////////
	JQuad(JTexture *tex, float x, float y, float width, float height);

	//////////////////////////////////////////////////////////////////////////
	/// Set blending color of the quad.
	/// 
	/// @param color - Color.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void SetColor(PIXEL_TYPE color);

	//////////////////////////////////////////////////////////////////////////
	/// Set anchor point of the quad.
	/// 
	/// @param x - X position of the anchor point.
	/// @param y - Y position of the anchor point.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void SetHotSpot(float x, float y);
	
	//////////////////////////////////////////////////////////////////////////
	/// Set UV positions of the quad.
	/// 
	/// @param x - X position of the quad in texture.
	/// @param y - Y position of the quad in texture.
	/// @param w - Width of the quad.
	/// @param h - Height of the quad.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void SetTextureRect(float x, float y, float w, float h);

	//////////////////////////////////////////////////////////////////////////
	/// Get UV positions of the quad.
	/// 
	/// @return x - X position of the quad in texture.
	/// @return y - Y position of the quad in texture.
	/// @return w - Width of the quad.
	/// @return h - Height of the quad.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void GetTextureRect(float *x, float *y, float *w, float *h);

	//////////////////////////////////////////////////////////////////////////
	/// Set horizontal flipping.
	/// 
	/// @param hflip - flipping flag;
	/// 
	//////////////////////////////////////////////////////////////////////////
	void SetHFlip(bool hflip) { mHFlipped = hflip; }

	//////////////////////////////////////////////////////////////////////////
	/// Set vetical flipping.
	/// 
	/// @param hflip - flipping flag;
	/// 
	//////////////////////////////////////////////////////////////////////////
	void SetVFlip(bool vflip) { mVFlipped = vflip; }

	JTexture* mTex;
	
#ifdef WIN32
	float mTX0;
	float mTY0;
	float mTX1;
	float mTY1;
	JColor mColor[4];		// up to 4 vertices
#else
	PIXEL_TYPE mColor[4];	// up to 4 vertices
	int mBlend;				// GU_TFX_MODULATE, GU_TFX_DECAL, GU_TFX_BLEND, GU_TFX_REPLACE, GU_TFX_ADD
#endif

	float mX;
	float mY;
	float mWidth;
	float mHeight;
	float mHotSpotX;
	float mHotSpotY;

	bool mHFlipped;
	bool mVFlipped;
};

//#endif


//////////////////////////////////////////////////////////////////////////
/// \enum JFONT_TEXT_ALIGNMENT
/// 
/// Font alignment.
/// 
//////////////////////////////////////////////////////////////////////////
enum JFONT_TEXT_ALIGNMENT
{
	JGETEXT_LEFT,		///< Text alignment to left.
	JGETEXT_CENTER,		///< Text alignment to center.
	JGETEXT_RIGHT		///< Text alignment to right.
};


enum JINIT_FLAG
{
	JINIT_FLAG_NORMAL,
	JINIT_FLAG_ENABLE3D
};


//------------------------------------------------------------------------------------------------
class JFont
{
public:
	JQuad* mQuad;
	int mWidth;
	int mHeight;
	int mSpacing;
	int mAlign;
	float mScale;
};


//------------------------------------------------------------------------------------------------
class Rect
{
public:
	int x;
	int y;
	int width;
	int height;

public:
	Rect(int _x, int _y, int _width, int _height): x(_x), y(_y), width(_width), height(_height) {}

};



#endif
