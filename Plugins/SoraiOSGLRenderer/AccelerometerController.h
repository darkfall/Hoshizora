/*
 *  mAccelerometer.h
 *  OpenGLES Study 1
 *
 *  Created by griffin clare on 09年10月17日.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#import <UIKit/UIKit.h>

@interface AccelerometerController : NSObject <UIAccelerometerDelegate> {
@private
    float x, y, z;
}

@property(nonatomic,readonly) float x;
@property(nonatomic,readonly) float y;
@property(nonatomic,readonly) float z;

- (void) prepare;
- (void) stop;


@end