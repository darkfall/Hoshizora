#ifndef _GRAPHIC_EFFECTS_H
#define _GRAPHIC_EFFECTS_H

#include <cmath>
#include <memory.h>

#if !defined(__APPLE__)
#include <malloc.h>
#endif
extern "C" {

int gaussBlur(unsigned long *data, int width, int height, double sigma, int radius);
int gray(unsigned long *data, int width, int height);
int reverse(unsigned long* data, int width, int height);
int alphaMix(unsigned long* data1, unsigned long* data2, int width, int height, int width2, int height2, double a, int posx=0, int posy=0);

}

#endif
