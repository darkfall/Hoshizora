//
//  GyroScopeController.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/14/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_GyroScopeController_h
#define Sora_iPhone_GyroScopeController_h

#import <CoreMotion/CoreMotion.h>

@interface GyroScopeController : NSObject {
@private
    
}
@end


/**
 self.motionManager = [[CMMotionManager alloc] init];
 
 
 //Gyroscope
 if([self.motionManager isGyroAvailable])
 {
if([self.motionManager isGyroActive] == NO)
{
    [self.motionManager setGyroUpdateInterval:1.0f / 2.0f];
    
    
    [self.motionManager startGyroUpdatesToQueue:[NSOperationQueue mainQueue]
                                    withHandler:^(CMGyroData *gyroData, NSError *error)
     {
         NSString *x = [[NSString alloc] initWithFormat:@"%.02f",gyroData.rotationRate.x];
         self.gyro_xaxis.text = x;
         
         NSString *y = [[NSString alloc] initWithFormat:@"%.02f",gyroData.rotationRate.y];
         self.gyro_yaxis.text = y;
         
         NSString *z = [[NSString alloc] initWithFormat:@"%.02f",gyroData.rotationRate.z];
         self.gyro_zaxis.text = z;
     }];
}
}
else
{
    NSLog(@"Gyroscope not Available!");
}
**/

#endif