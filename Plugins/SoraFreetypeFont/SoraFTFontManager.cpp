#include "SoraFTFontManager.h"

namespace sora {

	SoraFTFontManager::~SoraFTFontManager() {
		unsigned int i ;
	
		for(i=0; i<FTFonts.size(); ++i) {
			delete FTFonts[i].Font;
		}

		for (i=0; i<Faces.size(); ++i) {
			delete Faces[i].face;
		}
		
		delete lib;
	}

	
	template<typename T1, typename T2>
	int SoraFTFontManager::find(T1 &con, T2 &f) {
		int size = (int)con.size();
		for(int i = 0; i < size; ++i) {
			if(!(con.at(i) < f) && !(f < con.at(i))) {
				return i;
			}
		}
		return -1;
	}

	SoraFont* SoraFTFontManager::getFont(const char* filename, uint32 size, uint32 bsize, const char* pr) {
		if(!inited) return 0;
		
		SFTFace f;
		SFTFont tf;

		if(!filename)
			filename = "";

		if(bsize == 0)
			f.fileName = filename;
		else
			f.fileName = pr;

		int index = find(Faces, f);

		if(index == -1) {
			f.face = new FTFace();
			f.face->library = lib->library;
			if(f.face->load(filename, bsize)) {
				Faces.push_back(f);
				/* hold the memory, letting it dealloc when font destroys */
				if(bsize != 0)
					f.memoryHold.set((void*)filename, bsize);
				index = (int)Faces.size()-1;
			} else {
				delete f.face;
				return	0;
			}
		}
		SFTFace *face = &Faces[index];

		tf.fileName = face->fileName;
		tf.size = size;
		index = find(FTFonts, tf);
		if(index != -1) {
			if(FTFonts[index].Font)
				return FTFonts[index].Font;
		}

		// not existing yet. try to load font.

		SoraFTFont* font = new SoraFTFont();
		if(!font->attach(face->face, size)) {
			delete font;
			return 0;
		}

		// add to fonts.
		tf.Font = font;
		FTFonts.push_back(tf);
		
		font->setColor(0xFF000000);

		return font;
	}

} // namespace sora