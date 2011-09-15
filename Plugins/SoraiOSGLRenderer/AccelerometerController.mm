/*
 *  mAccelerometer.cpp
 *  OpenGLES Study 1
 *
 *  Created by griffin clare on 09年10月17日.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "AccelerometerController.h"

@implementation AccelerometerController

@synthesize x;
@synthesize y;
@synthesize z;

- (void) prepare {
	UIAccelerometer* accelerometer = [UIAccelerometer sharedAccelerometer];
	
	accelerometer.updateInterval = 1.f/60.f;
	accelerometer.delegate = self;
}

- (void) stop {
    UIAccelerometer* accelerometer = [UIAccelerometer sharedAccelerometer];
    accelerometer.delegate = nil;
}

- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration {
	UIAcceleration* accl = acceleration;
    x = accl.x;
    y = accl.y;
    z = accl.z;
}

@end


