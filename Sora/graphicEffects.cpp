#include "graphicEffects.h"

#include <fstream>

int gaussBlur(unsigned long *data, int width, int height, double sigma, int radius) {    
    double *gaussMatrix, gaussSum = 0.0, _2sigma2 = 2 * sigma * sigma;    
    int x, y, xx, yy, xxx, yyy;    
    double *pdbl, a, r, g, b, d;    
    unsigned char *bbb, *pout, *poutb, *pczdata = (unsigned char*)data;    
    pout = poutb = (unsigned char *)malloc(width * height * 4);    
    if (!pout) return 0;    
    gaussMatrix = pdbl = (double *)malloc((radius * 2 + 1) * (radius * 2 + 1) * sizeof(double));    
    if (!gaussMatrix) {    
        free(pout);    
        return 0;    
    }    
    for (y = -radius; y <= radius; y++) {    
        for (x = -radius; x <= radius; x++) {    
            a = exp(-(double)(x * x + y * y) / _2sigma2);    
            *pdbl++ = a;    
            gaussSum += a;    
        }    
    }    
    pdbl = gaussMatrix;    
    for (y = -radius; y <= radius; y++) {    
        for (x = -radius; x <= radius; x++) {    
            *pdbl++ /= gaussSum;    
        }    
    }    
    for (y = 0; y < height; y++) {    
        for (x = 0; x < width; x++) {    
            a = r = g = b = 0.0;    
            pdbl = gaussMatrix;    
            for (yy = -radius; yy <= radius; yy++) {    
                yyy = y + yy;    
                if (yyy >= 0 && yyy < height) {    
                    for (xx = -radius; xx <= radius; xx++) {    
                        xxx = x + xx;    
                        if (xxx >= 0 && xxx < width) {
                            bbb = &pczdata[(xxx + yyy * width)*4];    
                            d = *pdbl;    
                            b += d * bbb[0];    
                            g += d * bbb[1];    
                            r += d * bbb[2];    
                            a += d * bbb[3];    
                        }    
                        pdbl++;    
                    }    
                } else {    
                    pdbl += (radius * 2 + 1);    
                }    
            }    
            *pout++ = (unsigned char)b;    
            *pout++ = (unsigned char)g;    
            *pout++ = (unsigned char)r;    
            *pout++ = (unsigned char)a;    
        }    
    }    
    memcpy(data, poutb, width * height * 4);    
    free(gaussMatrix);    
    free(poutb);    
    return 1;    
}  

int gray(unsigned long *data, int width, int height) {
	unsigned char a, r, g, b;    
	unsigned char* pout, *poutb;   
    unsigned char* pczdata = (unsigned char*)data;
    pout = poutb =(unsigned char *)malloc(width * height * 4);    
    if (!pout) return 0;   

    int y, x;
	double gray;
	for (y = 0; y < height; y++) {    
        for (x = 0; x < width; x++) {    
            a = r = g = b = 0;    			
			b = *pczdata++;
			g = *pczdata++;
			r = *pczdata++;
			a = *pczdata++;
			
			gray = r*0.3+g*0.59+b*0.11;

            *pout++ = (unsigned char)gray;    
            *pout++ = (unsigned char)gray;    
            *pout++ = (unsigned char)gray;    
            *pout++ = (unsigned char)a;    
        }    
    }    
    memcpy(data, poutb, width * height * 4);    
    free(poutb);    
    return 1; 
}

int reverse(unsigned long* data, int width, int height) {
	unsigned char a, r, g, b;    
	unsigned char* pout, *poutb, *pczdata = (unsigned char*)data;    
    pout = poutb =(unsigned char *)malloc(width * height * 4);    
    if (!pout) return 0;   

    int y, x;
	for (y = 0; y < height; y++) {    
        for (x = 0; x < width; x++) {    
            a = r = g = b = 0;    
			
			b = *pczdata++;
			g = *pczdata++;
			r = *pczdata++;
			a = *pczdata++;
		
            *pout++ = (unsigned char)(255-b);    
            *pout++ = (unsigned char)(255-g);    
            *pout++ = (unsigned char)(255-r);    
            *pout++ = (unsigned char)a;    
        }    
    }    
    memcpy(data, poutb, width * height * 4);    
    free(poutb);    
    return 1; 
}

int alphaMix(unsigned long* data1, unsigned long* data2, int width, int height, int width2, int height2, double a, int posx, int posy) {
	double sa, sr, sg, sb, da, dr, dg, db;    
	unsigned char* pout, *poutb, *pczdata1 = (unsigned char*)data1, *pczdata2 = (unsigned char*)data2;    
    pout = poutb =(unsigned char *)malloc(width * height * 4);    
    if (!pout) return 0;   

    int y, x;
	int rh, rw;

	width2+posx>=width?rw=width:rw=width2+posx;
	height2+posy>=height?rh=height:rh=height2+posx;

	for (y = 0; y < height; y++) {    
        for (x = 0; x < width; x++) {    
			if(x >= posx && y >= posy && x < rw && y < rh) {
				sb = *pczdata1++;
				sg = *pczdata1++;
				sr = *pczdata1++;
				sa = *pczdata1++;

				db = *pczdata2++;
				dg = *pczdata2++;
				dr = *pczdata2++;
				da = *pczdata2++;
				
				if(da != 0.f) {
					*pout++ = (unsigned char)(sb*a+(1-a)*db);    
					*pout++ = (unsigned char)(sg*a+(1-a)*dg);    
					*pout++ = (unsigned char)(sr*a+(1-a)*dr);    
					*pout++ = (unsigned char)sa;
				} else {
					*pout++ = (unsigned char)sb;    
					*pout++ = (unsigned char)sg;    
					*pout++ = (unsigned char)sr;    
					*pout++ = (unsigned char)sa;
				}
			} else {
				unsigned char *c1 = (unsigned char *)&data1[x + y * width]; 
				*pout++ = (unsigned char)*pczdata1++;    
				*pout++ = (unsigned char)*pczdata1++;    
				*pout++ = (unsigned char)*pczdata1++;    
				*pout++ = (unsigned char)*pczdata1++;
			}
        }    
    }    
	
	memcpy(data1, poutb, width * height * 4);    
    free(poutb);    
    return 1; 
}
