//
//  GryoScopeController.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/19/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#import "GyroScopeController.h"

#import <CoreMotion/CoreMotion.h>

#import "debug/SoraInternalLogger.h"

@implementation SoraiOSGyroScopeController 

@synthesize motionManager;

@synthesize x;
@synthesize y;
@synthesize z;
@synthesize interval;

-(void)prepare {
    self.motionManager = [[CMMotionManager alloc] init];
    
    //Gyroscope
    if([self.motionManager isGyroAvailable]) {
        if([self.motionManager isGyroActive] == NO) {
            [self.motionManager setGyroUpdateInterval: interval];
            
            [self.motionManager startGyroUpdatesToQueue:[NSOperationQueue mainQueue]
                                            withHandler:^(CMGyroData *gyroData, NSError *error) {
                                                x = gyroData.rotationRate.x;
                                                y = gyroData.rotationRate.y;
                                                z = gyroData.rotationRate.z;
                                            }];
        }
    }
    else {
        sora::log_error("Gyroscope not Available!");
    }
}

-(void)setInterval:(float)_interval {
    interval = _interval;
}

-(void)stop {
    [self.motionManager stopDeviceMotionUpdates];
}

-(void)dealloc {
    [self.motionManager release];
    [super dealloc];
}

@end
