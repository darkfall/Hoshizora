/*
 *  mColor.h
 *  OpenGLES Study 1
 *
 *  Created by griffin clare on 09年10月16日.
 *  Copyright 2009 ring0Dev. All rights reserved.
 *
 */
#ifndef _SORA_COLOR_H_
#define _SORA_COLOR_H_

#include "SoraPlatform.h"

namespace sora {

#define CARGB(a,r,g,b)	((ulong32(a)<<24) + (ulong32(r)<<16) + (ulong32(g)<<8) + ulong32(b))
#define CGETA(col)		(((col)>>24) & 0xFF)
#define CGETR(col)		(((col)>>16) & 0xFF)
#define CGETG(col)		(((col)>>8) & 0xFF)
#define CGETB(col)		((col) & 0xFF)
#define CSETA(col,a)		(((col) & 0x00FFFFFF) + (ulong32(a)<<24))
#define CSETR(col,r)		(((col) & 0xFF00FFFF) + (ulong32(r)<<16))
#define CSETG(col,g)		(((col) & 0xFFFF00FF) + (ulong32(g)<<8))
#define CSETB(col,b)		(((col) & 0xFFFFFF00) + ulong32(b))

class SoraColorRGBA {
public:
	SoraColorRGBA(float32 _r, float32 _g, float32 _b, float32 _a):r(_r), g(_g), b(_b), a(_a) {}
	SoraColorRGBA(uint32 col) { *this = col; }
	SoraColorRGBA():r(1.f), g(1.f), b(1.f), a(1.f) {}

	void Set(float32 _r, float32 _g, float32 _b, float32 _a) { r = _r; g = _g; b = _b; a = _a; }
	
	SoraColorRGBA operator = (const SoraColorRGBA& rhs)		{r = rhs.r; g = rhs.g; b = rhs.b; a = rhs.a; return *this; }
	SoraColorRGBA operator = (uint32 col)					{a = (col>>24)/255.0f; r = ((col>>16) & 0xFF)/255.0f; g = ((col>>8) & 0xFF)/255.0f; b = (col & 0xFF)/255.0f; return *this; }
	
	SoraColorRGBA& operator += (const SoraColorRGBA& rhs) {r += rhs.r; g += rhs.g; b += rhs.b; a += rhs.a; return *this; }
	SoraColorRGBA& operator -= (const SoraColorRGBA& rhs) {r -= rhs.r; g -= rhs.g; b -= rhs.b; a -= rhs.a; return *this; }
	SoraColorRGBA& operator *= (const SoraColorRGBA& rhs) {r *= rhs.r; g *= rhs.g; b *= rhs.b; a *= rhs.a; return *this; }
	SoraColorRGBA& operator /= (float32 s)			{r /= s; g /= s; b /= s; a /= s; return *this; }
	SoraColorRGBA& operator *= (float32 s)			{r *= s; g *= s; b *= s; a *= s; return *this; }
	
	SoraColorRGBA operator - (const SoraColorRGBA& rhs)	{return SoraColorRGBA(r-rhs.r, g-rhs.g, b-rhs.b, a-rhs.a); }
	
	SoraColorRGBA operator + (float32 s)				{return SoraColorRGBA(r+s, g+s, b+s, a+s); }
	SoraColorRGBA operator - (float32 s)				{return SoraColorRGBA(r-s, g-s, b-s, a-s); }
	SoraColorRGBA operator * (float32 s)				{return SoraColorRGBA(r*s, g*s, b*s, a*s); }
	SoraColorRGBA operator / (float32 s)				{return SoraColorRGBA(r/s, g/s, b/s, a/s); }
	SoraColorRGBA operator * (const SoraColorRGBA& rhs) {r *= rhs.r; g *= rhs.g; b *= rhs.b; a *= rhs.a; return *this; }

	
	SoraColorRGBA operator + (const SoraColorRGBA& rhs) {return SoraColorRGBA(r+rhs.r, g+rhs.g, b+rhs.b, a+rhs.a); }
	
	bool operator >= (const SoraColorRGBA& rhs)	{return (r>=rhs.r && g>=rhs.g && b>=rhs.b && a>=rhs.a); }
	bool operator <= (const SoraColorRGBA& rhs)	{return (r<=rhs.r && g<=rhs.g && b<=rhs.b && a<=rhs.a); }
	
	bool operator == (float32 t)				{return (r==t && g==t && b==t && a==t); }
	bool operator != (float32 t)				{return (r!=t || g!=t || b!=t || a!=t); }
	bool operator == (const SoraColorRGBA& rhs)	{return (r==rhs.r && g==rhs.g && b==rhs.b && a==rhs.a); }
	bool operator != (const SoraColorRGBA& rhs)	{return (r!=rhs.r || g!=rhs.g && b!=rhs.b && a!=rhs.a); }

	unsigned long GetHWColor() const { return ((unsigned long)(a*255.0f)<<24) + ((unsigned long)(r*255.0f)<<16) + ((unsigned long)(g*255.0f)<<8) + (unsigned long)(b*255.0f); }
	static uint32 GetHWColor(float32 oR, float32 og, float32 ob, float32 oa)  { return ((unsigned long)(oa*255.0f)<<24) + ((unsigned long)(oR*255.0f)<<16) + ((unsigned long)(og*255.0f)<<8) + (unsigned long)(ob*255.0f); }

	
	SoraColorRGBA Normalize() {if(r<0.f) r=-r; if(g<0.f) g=-g; if(b<0.f) b=-b; if(a<0.f) a=-a; if(r>1.f) r=1.f; if(g>1.f) g=1.f; if(b>1.f) b=1.f; if(a>1.f) a=1.f; return *this; }
	
	float32 r;
	float32 g;
	float32 b;
	float32 a;
};
	
	static SoraColorRGBA COLOR_WHITE = SoraColorRGBA(1.f, 1.f, 1.f, 1.f);
	static SoraColorRGBA COLOR_BLACK = SoraColorRGBA(0.f, 0.f, 0.f, 1.f);
	static SoraColorRGBA COLOR_RED	 = SoraColorRGBA(1.f, 0.f, 0.f, 1.f);
	static SoraColorRGBA COLOR_BLUE  = SoraColorRGBA(0.f, 0.f, 1.f, 1.f);
	static SoraColorRGBA COLOR_GREEN = SoraColorRGBA(0.f, 1.f, 0.f, 1.f);

} // namespace sora

#endif