//
//  SoraiOSMoviePlayerImpl.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 7/10/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_SoraiOSMoviePlayerImpl_h
#define Sora_iPhone_SoraiOSMoviePlayerImpl_h

#import <Foundation/Foundation.h>
#import <MediaPlayer/MPMoviePlayerController.h>

@interface SoraiOSMoviePlayerImpl : NSObject {
    MPMoviePlayerController* theMovie;
}

- (bool)playMovieAtURL:(NSURL*)url;
- (void)myMovieFinishedCallback:(NSNotification*)aNotification;

- (bool)playMovie:(NSString*)filename;

@end

#endif
