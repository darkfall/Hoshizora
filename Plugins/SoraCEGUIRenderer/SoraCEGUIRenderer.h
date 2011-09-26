//
//  SoraCEGUIRenderer.h
//  Sora
//
//  Created by Ruiwei Bu on 9/24/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraCEGUIRenderer_h
#define Sora_SoraCEGUIRenderer_h

#include <CEGUIBase/CEGUIRenderer.h>

namespace sora {
    
    class SoraCEGUIRenderer {
    public:
        virtual CEGUI::RenderingRoot& getDefaultRenderingRoot() = 0;
        
        /*!
         \brief
         Create a new GeometryBuffer and return a reference to it.  You should
         remove the GeometryBuffer from any RenderQueues and call
         destroyGeometryBuffer when you want to destroy the GeometryBuffer.
         
         \return
         GeometryBuffer object.
         */
        virtual CEGUI::GeometryBuffer& createGeometryBuffer() = 0;
        
        /*!
         \brief
         Destroy a GeometryBuffer that was returned when calling the
         createGeometryBuffer function.  Before destroying any GeometryBuffer
         you should ensure that it has been removed from any RenderQueue that
         was using it.
         
         \param buffer
         The GeometryBuffer object to be destroyed.
         */
        virtual void destroyGeometryBuffer(const CEGUI::GeometryBuffer& buffer) = 0;
        
        /*!
         \brief
         Destroy all GeometryBuffer objects created by this Renderer.
         */
        virtual void destroyAllGeometryBuffers() = 0;
        
        /*!
         \brief
         Create a TextureTarget that can be used to cache imagery; this is a
         RenderTarget that does not lose it's content from one frame to another.
         
         If the renderer is unable to offer such a thing, 0 should be returned.
         
         \return
         Pointer to a TextureTarget object that is suitable for caching imagery,
         or 0 if the renderer is unable to offer such a thing.
         */
        virtual CEGUI::TextureTarget* createTextureTarget() = 0;
        
        /*!
         \brief
         Function that cleans up TextureTarget objects created with the
         createTextureTarget function.
         
         \param target
         A pointer to a TextureTarget object that was previously returned from a
         call to createTextureTarget.
         */
        virtual void destroyTextureTarget(CEGUI::TextureTarget* target) = 0;
        
        /*!
         \brief
         Destory all TextureTarget objects created by this Renderer.
         */
        virtual void destroyAllTextureTargets() = 0;
        
        /*!
         \brief
         Create a 'null' Texture object.
         
         \return
         A newly created Texture object.  The returned Texture object has no size
         or imagery associated with it.
         */
        virtual CEGUI::Texture& createTexture() = 0;
        
        /*!
         \brief
         Create a Texture object using the given image file.
         
         \param filename
         String object that specifies the path and filename of the image file to
         use when creating the texture.
         
         \param resourceGroup
         String objet that specifies the resource group identifier to be passed
         to the resource provider when loading the texture file \a filename.
         
         \return
         A newly created Texture object.  The initial content of the texture
         memory is the requested image file.
         
         \note
         Due to possible limitations of the underlying hardware, API or engine,
         the final size of the texture may not match the size of the loaded file.
         You can check the ultimate sizes by querying the Texture object
         after creation.
         */
        virtual CEGUI::Texture& createTexture(const CEGUI::String& filename,
                                              const CEGUI::String& resourceGroup) = 0;
        
        /*!
         \brief
         Create a Texture object with the given pixel dimensions as specified by
         \a size.
         
         \param size
         Size object that describes the desired texture size.
         
         \return
         A newly created Texture object.  The initial contents of the texture
         memory is undefined.
         
         \note
         Due to possible limitations of the underlying hardware, API or engine,
         the final size of the texture may not match the requested size.  You can
         check the ultimate sizes by querying the Texture object after creation.
         */
        virtual CEGUI::Texture& createTexture(const CEGUI::Size& size) = 0;
        
        /*!
         \brief
         Destroy a Texture object that was previously created by calling the
         createTexture functions.
         
         \param texture
         Texture object to be destroyed.
         */
        virtual void destroyTexture(CEGUI::Texture& texture) = 0;
        
        /*!
         \brief
         Destroy all Texture objects created by this Renderer.
         */
        virtual void destroyAllTextures() = 0;
        
        /*!
         \brief
         Perform any operations required to put the system into a state ready
         for rendering operations to begin.
         */
        virtual void beginRendering() = 0;
        
        /*!
         \brief
         Perform any operations required to finalise rendering.
         */
        virtual void endRendering() = 0;
        
        /*!
         \brief
         Set the size of the display or host window in pixels for this Renderer
         object.
         
         This is intended to be called by the System as part of the notification
         process when display size changes are notified to it via the
         System::notifyDisplaySizeChanged function.
         
         \note
         The Renderer implementation should not use this function other than to
         perform internal state updates on the Renderer and related objects.
         
         \param size
         Size object describing the dimesions of the current or host window in
         pixels.
         */
        virtual void setDisplaySize(const CEGUI::Size& size) = 0;
        
        /*!
         \brief
         Return the size of the display or host window in pixels.
         
         \return
         Size object describing the pixel dimesntions of the current display or
         host window.
         */
        virtual const CEGUI::Size& getDisplaySize() const = 0;
        
        /*!
         \brief
         Return the resolution of the display or host window in dots per inch.
         
         \return
         Vector2 object that describes the resolution of the display or host
         window in DPI.
         */
        virtual const CEGUI::Vector2& getDisplayDPI() const = 0;
        
        /*!
         \brief
         Return the pixel size of the maximum supported texture.
         
         \return
         Size of the maximum supported texture in pixels.
         */
        virtual CEGUI::uint getMaxTextureSize() const = 0;
        
        /*!
         \brief
         Return identification string for the renderer module.
         
         \return
         String object holding text that identifies the Renderer in use.
         */
        virtual const CEGUI::String& getIdentifierString() const = 0;
    };
    
} // namespace sora



#endif
