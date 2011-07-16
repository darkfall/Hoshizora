/*
 *  mColor.h
 *  OpenGLES Study 1
 *
 *  Created by griffin clare on 09.10.16
 *  Copyright 2009 ring0Dev. All rights reserved.
 *
 */
#ifndef _SORA_COLOR_H_
#define _SORA_COLOR_H_

#include "SoraPlatform.h"
#include <cmath>

namespace sora {

#define CARGB(a,r,g,b)	((uint32(a)<<24) + (uint32(r)<<16) + (uint32(g)<<8) + uint32(b))
#define CGETA(col)		(((col)>>24) & 0xFF)
#define CGETR(col)		(((col)>>16) & 0xFF)
#define CGETG(col)		(((col)>>8) & 0xFF)
#define CGETB(col)		((col) & 0xFF)
#define CSETA(col,a)		(((col) & 0x00FFFFFF) + (uint32(a)<<24))
#define CSETR(col,r)		(((col) & 0xFF00FFFF) + (uint32(r)<<16))
#define CSETG(col,g)		(((col) & 0xFFFF00FF) + (uint32(g)<<8))
#define CSETB(col,b)		(((col) & 0xFFFFFF00) + uint32(b))

    inline float32 ColorNormalize(float32 c) {
        if(c < 0.0f) c = 0.0f;
        else if(c > 1.0f) c = 1.0f;
        return c;
    }
    
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
        
        SoraColorRGBA operator - (const SoraColorRGBA& rhs)	const {return SoraColorRGBA(r-rhs.r, g-rhs.g, b-rhs.b, a-rhs.a); }
        
        SoraColorRGBA operator + (float32 s)	const 		{return SoraColorRGBA(r+s, g+s, b+s, a+s); }
        SoraColorRGBA operator - (float32 s)	const       {return SoraColorRGBA(r-s, g-s, b-s, a-s); }
        SoraColorRGBA operator * (float32 s)	const       {return SoraColorRGBA(r*s, g*s, b*s, a*s); }
        SoraColorRGBA operator / (float32 s)	const       {return SoraColorRGBA(r/s, g/s, b/s, a/s); }
        SoraColorRGBA operator * (const SoraColorRGBA& rhs) const {return SoraColorRGBA(r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a); }
        
        
        SoraColorRGBA operator + (const SoraColorRGBA& rhs) const {return SoraColorRGBA(r+rhs.r, g+rhs.g, b+rhs.b, a+rhs.a); }
        
        bool operator >= (const SoraColorRGBA& rhs)	{return (r>=rhs.r && g>=rhs.g && b>=rhs.b && a>=rhs.a); }
        bool operator <= (const SoraColorRGBA& rhs)	{return (r<=rhs.r && g<=rhs.g && b<=rhs.b && a<=rhs.a); }
        
        bool operator == (float32 t)				{return (r==t && g==t && b==t && a==t); }
        bool operator != (float32 t)				{return (r!=t || g!=t || b!=t || a!=t); }
        bool operator == (const SoraColorRGBA& rhs)	{return (r==rhs.r && g==rhs.g && b==rhs.b && a==rhs.a); }
        bool operator != (const SoraColorRGBA& rhs)	{return (r!=rhs.r || g!=rhs.g || b!=rhs.b || a!=rhs.a); }
        
        unsigned int GetHWColor() const { return ((unsigned int)(a*255.0f)<<24) + ((unsigned int)(r*255.0f)<<16) + ((unsigned int)(g*255.0f)<<8) + (unsigned int)(b*255.0f); }
        static uint32 GetHWColor(float32 oR, float32 og, float32 ob, float32 oa)  { return ((unsigned int)(oa*255.0f)<<24) + ((unsigned int)(oR*255.0f)<<16) + ((unsigned int)(og*255.0f)<<8) + (unsigned int)(ob*255.0f); }
        
        
        SoraColorRGBA Normalize() {
            r = ColorNormalize(r);
            g = ColorNormalize(g);
            b = ColorNormalize(b);
            a = ColorNormalize(a);
            return *this;
        }
        
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
    
    class SoraColorHSV {
    public:
        float32 h, s, v, a;
        
        SoraColorHSV(float32 _h, float32 _s, float32 _v, float32 _a):
        h(_h), s(_s), v(_v), a(_a) {}
        SoraColorHSV(uint32 col) {
            SetHWColor(col);
        }
        
        SoraColorHSV operator-(const SoraColorHSV& c) const {
            return SoraColorHSV(h-c.h, s-c.s, v-c.v, a-c.a);
        }
        SoraColorHSV operator+(const SoraColorHSV& c) const {
            return SoraColorHSV(h+c.h, s+c.s, v+c.v, a+c.a);
        }
        SoraColorHSV operator*(const SoraColorHSV& c) const {
            return SoraColorHSV(h*c.h, s*c.s, v*c.v, a*c.a);
        }
        SoraColorHSV operator/(float32 c) const {
            assert(c != 0.f);
            return SoraColorHSV(h/c, s/c, v/c, a/c);
        }
        SoraColorHSV& operator-=(const SoraColorHSV& c) {
            h -= c.h;
            s -= c.s;
            v -= c.v;
            a -= c.a;
            return *this;
        }
        SoraColorHSV& operator+=(const SoraColorHSV& c) {
            h += c.h;
            s += c.s;
            v += c.v;
            a += c.a;
            return *this;
        }
        SoraColorHSV& operator*=(const SoraColorHSV& c) {
            h *= c.h;
            s *= c.s;
            v *= c.v;
            a *= c.a;
            return *this;
        }
        
        SoraColorHSV& operator=(const SoraColorHSV& c) {
            h = c.h;
            s = c.s;
            v = c.v;
            a = c.a;
            return *this;
        }
        
        bool operator ==(const SoraColorHSV& c) const;
        bool operator !=(const SoraColorHSV& c) const;
        
        SoraColorHSV& Normalize() {
            h = ColorNormalize(h);
            s = ColorNormalize(s);
            v = ColorNormalize(v);
            a = ColorNormalize(a);
            return *this;
        }
        
        void SetHWColor(uint32 col);
        uint32 GetHWColor() const;
    };
    
    inline void SoraColorHSV::SetHWColor(uint32 col) {
        float r, g, b;
        float minv, maxv, delta;
        float del_R, del_G, del_B;
        
        a = (col>>24) / 255.0f;
        r = ((col>>16) & 0xFF) / 255.0f;
        g = ((col>>8)  & 0xFF) / 255.0f;
        b = (col       & 0xFF) / 255.0f;
        
        minv = std::min(std::min(r, g), b);
        maxv = std::max(std::max(r, g), b);
        delta = maxv - minv;
        
        v = maxv;
        
        if (delta == 0) {
            h = 0;
            s = 0;
        }
        else {
            s = delta / maxv;
            del_R = (((maxv - r) / 6) + (delta / 2)) / delta;
            del_G = (((maxv - g) / 6) + (delta / 2)) / delta;
            del_B = (((maxv - b) / 6) + (delta / 2)) / delta;
            
            if      (r == maxv) {h = del_B - del_G;}
            else if (g == maxv) {h = (1 / 3) + del_R - del_B;}
            else if (b == maxv) {h = (2 / 3) + del_G - del_R;}
            
            if (h < 0) h += 1;
            if (h > 1) h -= 1;
        }
    }
    
    inline uint32 SoraColorHSV::GetHWColor() const {
        float r, g, b;
        float xh, i, p1, p2, p3;
        
        if (s == 0) {
            r = v;
            g = v;
            b = v;
        }
        else {
            xh = h * 6;
            if(xh == 6) xh=0;
            i = floorf(xh);
            p1 = v * (1 - s);
            p2 = v * (1 - s * (xh - i));
            p3 = v * (1 - s * (1 - (xh - i)));
            
            if      (i == 0) {r = v;  g = p3; b = p1;}
            else if (i == 1) {r = p2; g = v;  b = p1;}
            else if (i == 2) {r = p1; g = v;  b = p3;}
            else if (i == 3) {r = p1; g = p2; b = v; }
            else if (i == 4) {r = p3; g = p1; b = v; }
            else			 {r = v;  g = p1; b = p2;}
        }
        
        return (uint32(a*255.0f)<<24) + (uint32(r*255.0f)<<16) + (uint32(g*255.0f)<<8) + uint32(b*255.0f);
    }
    
    inline SoraColorRGBA operator*(const float sc, const SoraColorRGBA& c) {
        return c * sc;
    }
    
    inline SoraColorHSV operator*(const float sc, const SoraColorHSV& c) {
        return c * sc;
    }
    

} // namespace sora

#endif