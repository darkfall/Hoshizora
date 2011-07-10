//
//  SoraiOSMoviePlayerImpl.mm
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 7/10/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#include "SoraiOSMoviePlayerImpl.h"

#include "SoraCore.h"

@implementation SoraiOSMoviePlayerImpl

- (bool)playMovieAtURL:(NSURL*)theURL 
{
    theMovie = [[MPMoviePlayerController alloc] initWithContentURL:theURL];
    if(!theMovie)
        return false;
    
    theMovie.scalingMode=MPMovieScalingModeAspectFill;
    //    theMovie.userCanShowTransportControls=NO;
	
    // Register for the playback finished notification.
    [[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(myMovieFinishedCallback:)
												 name:MPMoviePlayerPlaybackDidFinishNotification
											   object:theMovie];
	
    // pause the main window and wait for movie to finish
    sora::SoraCore::Instance()->pause(true, true);
    // Movie playback is asynchronous, so this method returns immediately.
    [theMovie play];
    
    return true;
}

// When the movie is done,release the controller.
- (void)myMovieFinishedCallback:(NSNotification*)aNotification
{
    MPMoviePlayerController* movie = [aNotification object];
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:MPMoviePlayerPlaybackDidFinishNotification
                                                  object:movie];
	
    // resume the core
    sora::SoraCore::Instance()->resume();
    
    [theMovie release];
} 

- (bool)playMovie:(NSString*)filename 
{
    NSURL* url = [NSURL fileURLWithPath:filename];
    NSLog(@"Playing URL: %@", url);
    return [self playMovieAtURL:url];
}

@end
