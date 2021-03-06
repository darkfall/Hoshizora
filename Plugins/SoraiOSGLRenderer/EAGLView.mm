//
//  EAGLView.m
//  Sora-iPhone
//
//  Created by GriffinBu on 1/30/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import "EAGLView.h"
#import "SoraiOSTouchPool.h"
#import "SoraiOSInitializer.h"
#import "SoraiOSDeviceHelper.h"
#import "SoraiOSWrapper.h"
#import "SoraCore.h"

@interface EAGLView (PrivateMethods)
- (void)createFramebuffer;
- (void)deleteFramebuffer;
@end

@implementation EAGLView

@dynamic context;

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:.
- (id)initWithCoder:(NSCoder*)coder
{
    self = [super initWithCoder:coder];
	if (self)
    {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                        nil];
    }
    
    UIDevice *device = [UIDevice currentDevice];	
	[device beginGeneratingDeviceOrientationNotifications];
	NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
	[nc addObserver:self
		   selector:@selector(orientationChanged:)
			   name:UIDeviceOrientationDidChangeNotification
			 object:device];
    
    if(sora::_IS_RETINA_DISPLAY()) {
        self.contentScaleFactor = 2.0;
    }
    printf("%f, %f, %f\n", self.contentScaleFactor, ((CAEAGLLayer *)self.layer).contentsScale, [UIScreen mainScreen].scale);
    
    sora::setEAGLView(self);
    multisampling = true;
    initialized = true;
    
    msaaFramebuffer = nil;
    msaaColorBuffer = nil;
    msaaDepthBuffer = nil;
    
    enableOrientation = true;

    return self;
}

- (void)dealloc
{
    initialized = false;

    [self deleteMSAABuffer];
    [self deleteFramebuffer];    
 //   [context release];
    
    [super dealloc];
}

- (EAGLContext *)context
{
    return context;
}

- (void)setContext:(EAGLContext *)newContext
{
    if (context != newContext)
    {
        [self deleteFramebuffer];
        
        [context release];
        context = [newContext retain];
        
        [EAGLContext setCurrentContext:nil];
    }
}


- (void)enableMultiSampling:(BOOL)flag {
    multisampling = flag;
    if(multisampling && !msaaFramebuffer) {
        [self setupMSAABuffer];
    } else if(!multisampling && msaaFramebuffer) {
        [self deleteMSAABuffer];
    }
}

- (BOOL)isMultiSamplingEnabled {
    return multisampling;
}

- (void)setupMSAABuffer {
    // msaa 
    if(multisampling) {
        glGenFramebuffers(1, &msaaFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
        
        glGenRenderbuffers(1, &msaaColorBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, msaaColorBuffer);
        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, GL_RGBA8_OES, framebufferWidth, framebufferHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, msaaColorBuffer);
        
        glGenRenderbuffers(1, &msaaDepthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, msaaDepthBuffer);
        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT16, framebufferWidth, framebufferHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, msaaDepthBuffer);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }

}

- (GLuint)createFramebuffer:(GLuint)width height:(GLuint)height tex:(GLuint)tex {
    GLuint frameBuffer;
    GLuint depthBuffer;
    
    if (context)
    {        
        GLuint oldbuffer;
		// create framebuffer
        glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, (GLint *) &oldbuffer);
        
		glGenFramebuffersOES(1, &frameBuffer);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, frameBuffer);
		
		glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, tex, NULL);
        
		//create depth buffer
		glGenRenderbuffersOES(1, &depthBuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthBuffer);
        [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer *)self.layer];
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, 0);
        
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, depthBuffer);
    
        
		GLenum status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
		if(status != GL_FRAMEBUFFER_COMPLETE_OES) {
            sora::log_error("Error creating Render Target");
		}
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, oldbuffer);
        
    }
    return frameBuffer;
}

- (void)destroyFramebuffer:(GLuint)buffer {
    glDeleteFramebuffersOES(1, &buffer);
}

- (void)createFramebuffer
{
    if (context && !defaultFramebuffer)
    {
        [EAGLContext setCurrentContext:context];
        
        // Create default framebuffer object.
        glGenFramebuffers(1, &defaultFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
        // Create color render buffer and allocate backing store.
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferHeight);
		        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }
}

- (void)deleteMSAABuffer {
    
    if(multisampling) {
        if(msaaFramebuffer) {
            glDeleteFramebuffers(1, &msaaFramebuffer);
            msaaFramebuffer = 0;
        }
        
        if(msaaColorBuffer) {
            glDeleteRenderbuffers(1, &msaaColorBuffer);
            msaaColorBuffer = 0;
        }
        
        if(msaaDepthBuffer) {
            glDeleteRenderbuffers(1, &msaaDepthBuffer);
            msaaDepthBuffer = 0;
        }
    }
}

- (void)deleteFramebuffer
{
    if (context)
    {
        [EAGLContext setCurrentContext:context];
        
        if (defaultFramebuffer)
        {
            glDeleteFramebuffers(1, &defaultFramebuffer);
            defaultFramebuffer = 0;
        }
        
        if (colorRenderbuffer)
        {
            glDeleteRenderbuffers(1, &colorRenderbuffer);
            colorRenderbuffer = 0;
        }
    }
}

- (void) update {
    if(initialized)
        sora::SORA_IOS->update();
}

- (void)setFramebuffer
{
    
    if (context)
        {
            if (!defaultFramebuffer) {
                [self createFramebuffer];

            }
            if(multisampling && !msaaFramebuffer) {
                [self setupMSAABuffer];
            }
            
            [EAGLContext setCurrentContext:context];
            
            if(multisampling)
                glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer);
            else
                glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
            
            glColor4f(0.f, 0.f, 0.f, 0.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, framebufferWidth, framebufferHeight);
        }
}


- (BOOL)presentFramebuffer
{
    BOOL success = FALSE;
  
    if (context)
    {
        [EAGLContext setCurrentContext:context];
        
        if(multisampling) {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, defaultFramebuffer);
            glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, msaaFramebuffer);
            glResolveMultisampleFramebufferAPPLE();
            
            const GLenum discards[]  = {GL_COLOR_ATTACHMENT0,GL_DEPTH_ATTACHMENT};
            glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE,2,discards);
            
            glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        } else {
            const GLenum discards[]  = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
            glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
            glDiscardFramebufferEXT(GL_FRAMEBUFFER,1,discards);
    
            glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        } 
        success = [context presentRenderbuffer:GL_RENDERBUFFER];
    }
    return success;
}


- (BOOL)presentFramebuffer2:(GLuint)buffer {
    BOOL success = FALSE;
    
    if (context)
    {
        [EAGLContext setCurrentContext:context];
        
        glBindRenderbuffer(GL_RENDERBUFFER_OES, buffer);
        
        success = [context presentRenderbuffer:GL_RENDERBUFFER_OES];
    }
    return success;
}

- (void)touchesChangedWithEvent:(UIEvent*)event
{
	NSSet *touches = [event allTouches];
	sora::SoraiOSInputDelegate::updateTouchInfo(touches, (UIView*)self);
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	[self touchesChangedWithEvent:event];
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	[self touchesChangedWithEvent:event];

}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	[self touchesChangedWithEvent:event];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
	[self touchesChangedWithEvent:event];

}

- (void)orientationChanged:(NSNotification *)note {
    if(enableOrientation) {
        sora::iOSOrientation orientation;
        
        switch([[note object] orientation]) {
            case UIDeviceOrientationPortrait:
                orientation = sora::ORIENTATION_PORTRAIT;
                break;
            case UIDeviceOrientationPortraitUpsideDown:
                orientation = sora::ORIENTATION_PORTRAIT_UPSIDE_DOWN;
                break;
            case UIDeviceOrientationLandscapeLeft:
                orientation = sora::ORIENTATION_LANDSCAPE_LEFT;
                break;
            case UIDeviceOrientationLandscapeRight:
                orientation =  sora::ORIENTATION_LANDSCAPE_RIGHT;
                break;
            default:
                break;
        }
        
        sora::SoraiOSInitializer::Instance()->setOrientation(orientation);
    }
}

- (void)layoutSubviews
{
    // The framebuffer will be re-created at the beginning of the next setFramebuffer method call.
    [self deleteFramebuffer];
}

- (int)getScreenWidth {
    return framebufferWidth / [self contentScaleFactor];
}

- (int)getScreenHeight {
    return framebufferHeight / [self contentScaleFactor];
}

- (int)getViewWidth {
    return framebufferWidth;
}

- (int)getViewHeight {
    return framebufferHeight;
}

- (float)getContentsScale {
    if(sora::isUseRetina())
        return [self contentScaleFactor];
    return 1.0;
}

- (void)enableOrientationChange:(BOOL)flag {
    enableOrientation = flag;
}

- (bool)isOrientationChangeEnabled {
    return enableOrientation;
}

@end
