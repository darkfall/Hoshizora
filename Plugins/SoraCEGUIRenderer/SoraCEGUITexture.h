//
//  SoraCEGUITexture.h
//  Sora
//
//  Created by Ruiwei Bu on 9/24/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraCEGUITexture_h
#define Sora_SoraCEGUITexture_h

#include "CEGUIBase/CEGUI.h"
#include "SoraPlatform.h"

namespace sora {
    
    class SoraCEGUITexture: public CEGUI::Texture {
    public:
        
        /*!
         \brief
         Returns the current pixel size of the texture.
         
         \return
         Reference to a Size object that describes the size of the texture in
         pixels.
         */
        const CEGUI::Size& getSize() const;
        
        /*!
         \brief
         Returns the original pixel size of the data loaded into the texture.
         
         \return
         reference to a Size object that describes the original size, in pixels,
         of the data loaded into the texture.
         */
        const CEGUI::Size& getOriginalDataSize() const;
        
        /*!
         \brief
         Returns pixel to texel scale values that should be used for converting
         pixel values to texture co-ords.
         
         \return
         Reference to a Vector2 object that describes the scaling values required
         to accurately map pixel positions to texture co-ordinates.
         */
        const CEGUI::Vector2& getTexelScaling() const;
        
        /*!
         \brief
         Loads the specified image file into the texture.  The texture is resized
         as required to hold the image.
         
         \param filename
         The filename of the image file that is to be loaded into the texture
         
         \param resourceGroup
         Resource group identifier to be passed to the resource provider when
         loading the image file.
         
         \return
         Nothing.
         */
        void loadFromFile(const CEGUI::String& filename,
                          const CEGUI::String& resourceGroup);
        
        /*!
         \brief
         Loads (copies) an image in memory into the texture.  The texture is
         resized as required to hold the image.
         
         \param buffer
         Pointer to the buffer containing the image data.
         
         \param buffer_size
         Size of the buffer (in pixels as specified by \a pixelFormat)
         
         \param pixel_format
         PixelFormat value describing the format contained in \a buffPtr.
         
         \return
         Nothing.
         */
        void loadFromMemory(const void* buffer,
                            const CEGUI::Size& buffer_size,
                            PixelFormat pixel_format);
        
        /*!
         \brief
         Save / dump the content of the texture to a memory buffer.  The dumped
         pixel format is always RGBA (4 bytes per pixel).
         
         \param buffer
         Pointer to the buffer that is to receive the image data.  You must make
         sure that this buffer is large enough to hold the dumped texture data,
         the required pixel dimensions can be established by calling getSize.
         */
        void saveToMemory(void* buffer);
        
    private:
        SoraTextureHandle mTexture;
        
        // cache for faster get
        CEGUI::Size mSize;
        CEGUI::Size mOriginalSize;
    };
    
    
} // namespace sora


#endif
