/*
 *  SoraPNGWritter.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/26/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef PNG_WRITTER_H_
#define PNG_WRITTER_H_

#include "libPNG/png.h"

#include <iostream>

class PNGError {};

static void PNGWarningCallback(png_structp png_ptr,
							   png_const_charp msg) {
	std::cerr<<"libPNG Warning: "<<msg<<std::endl;
}

static void PNGErrorCallback(png_structp png_ptr,
				   png_const_charp msg) {
	std::cerr<<"libPNG Error: "<<msg<<std::endl;
	throw PNGError();
}

static bool writePNGWithPitch(FILE* fp, uint32* bits, int32 width, int32 height, int32 pitch, bool setbgr=false, bool needAlpha=true) {
	png_structp png_ptr = 0;
	png_infop info_ptr = 0;
	try {
		//	create png structs
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, PNGErrorCallback, PNGWarningCallback);
		if (png_ptr == 0) return false;
		
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == 0) return false;
		
		//	init i/o
		png_init_io(png_ptr, fp);
		
		//	setup params
		if ( needAlpha ) {
			png_set_IHDR( png_ptr, info_ptr, width, height, 
						 8, 
						 PNG_COLOR_TYPE_RGBA, 
						 PNG_INTERLACE_NONE,
						 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		}
		else {
			png_set_IHDR( png_ptr, info_ptr, width, height, 
						 8, 
						 PNG_COLOR_TYPE_RGB,
						 PNG_INTERLACE_NONE,
						 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		}
		
		png_write_info(png_ptr, info_ptr);
		
		if ( !needAlpha )
			png_set_filler(png_ptr, 0, PNG_FILLER_AFTER);	//	strip alpha
		
		if(setbgr)
			png_set_bgr(png_ptr);	//	switch to little-endian notation
		
		//	writing
		unsigned char* pSrc = (unsigned char*)bits;
		for ( int i = 0; i < height; i++, pSrc += pitch*4 ) {
			png_write_row( png_ptr, pSrc );
		}
		
		png_write_end(png_ptr, info_ptr);
	} 
	catch(PNGError) {
		png_destroy_write_struct(&png_ptr, (info_ptr == 0) ? NULL : &info_ptr);
		return false;
	}
	
	//	cleanup
	png_destroy_write_struct( &png_ptr, (info_ptr == 0) ? NULL : &info_ptr);
	return true;		
}



#endif // PNG_WRITTER_H_