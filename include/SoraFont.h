#ifndef SORA_FONT_H_
#define SORA_FONT_H_

#include "SoraPlatform.h"
#include "SoraImageEffect.h"

namespace sora {

	// Font Render Alignments
	enum {
		FONT_ALIGNMENT_LEFT     =   1,
		FONT_ALIGNMENT_RIGHT    =   2,
		FONT_ALIGNMENT_CENTER   =   3,
	};

	/*
		Abstract Font Class provides Font related APIs
	*/

	class SORA_API SoraFont {
	public:
		virtual void	render(float32 x, float32 y, const wchar_t* text, bool hcenter = false, bool vcenter = false) = 0;
		// render with alignments
		virtual void	print(float32 x, float32 y, int32 align, const wchar_t *format, ...) = 0;
		virtual void	render(float32 x, float32 y, int32 align, const wchar_t* text) = 0;

		virtual void	setColor(ulong32 dwColor, int32 i = -1) = 0;
		virtual ulong32	getColor(int32 i = 0) = 0;

		// set kernings
		virtual void	setKerningWidth(float32 kerning) = 0;
		virtual void	setKerningHeight(float32 kerning) = 0;

		// get kernings
		virtual float32	getKerningWidth() const = 0;
		virtual float32	getKerningHeight() const = 0;

		// get font dimensions
		virtual float32	getStringWidth(const wchar_t* text) = 0;
		virtual float32 getStringHeight(const wchar_t* text) = 0;
		virtual float32	getHeight() const = 0;

		// get/set font size
		virtual uint32	getFontSize() const = 0;
		virtual void	setFontSize(uint32 size) = 0;

		// get width of a char
		virtual float32	getWidthFromCharacter(wchar_t c, bool original = false) = 0;

		// font effects
		virtual void setCharRotation(float32 rot) = 0;
		virtual void setScale(float32 scale) = 0;
        
        // get/set line width
        virtual void setLineWidth(float32 width) = 0;
        virtual float32 getLineWidth() const= 0;
        
        virtual float32 getCharRotation() const = 0;
        virtual float32 getScale() const = 0;
        
        // set/get line rotation
        virtual void setLineRotation(float32 rot, bool rotateChar=false) = 0;
        virtual float32 getLineRotation() const = 0;
	};

} // namespace sora


#endif
