/*
 *  mAccelerometer.h
 *  OpenGLES Study 1
 *
 *  Created by griffin clare on 09年10月17日.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#import <UIKit/UIKit.h>

@interface SoraiOSAccelerometerController : NSObject <UIAccelerometerDelegate> {
@private
    float x, y, z;
    float interval;
}

@property(nonatomic,readonly) float x;
@property(nonatomic,readonly) float y;
@property(nonatomic,readonly) float z;
@property(nonatomic,readonly) float interval;

- (void) prepare;
- (void) stop;
- (void) setInterval:(float)interval;


@end