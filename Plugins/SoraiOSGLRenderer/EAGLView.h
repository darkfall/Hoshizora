//
//  EAGLView.h
//  Sora-iPhone
//
//  Created by GriffinBu on 1/30/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/EAGL.h>

// This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
// The view content is basically an EAGL surface you render your OpenGL scene into.
// Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
@interface EAGLView : UIView
{
@private
    EAGLContext *context;
    
    // The pixel dimensions of the CAEAGLLayer.
    GLint framebufferWidth;
    GLint framebufferHeight;
    
    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view.
    GLuint defaultFramebuffer, colorRenderbuffer;
    bool initialized;
    
    bool multisampling;
    GLuint msaaFramebuffer, msaaColorBuffer, msaaDepthBuffer;
}

@property (nonatomic, retain) EAGLContext *context;

- (void)enableMultiSampling:(BOOL)flag;
- (BOOL)isMultiSamplingEnabled;

- (void)setupMSAABuffer;
- (void)deleteMSAABuffer;


- (void)update;
- (void)setFramebuffer;
- (GLuint)createFramebuffer:(GLuint)width height:(GLuint)height tex:(GLuint)tex;
- (void)destroyFramebuffer:(GLuint)buffer;
- (BOOL)presentFramebuffer;
- (BOOL)presentFramebuffer2:(GLuint)buffer;

@end
