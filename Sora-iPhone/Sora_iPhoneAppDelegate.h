//
//  Sora_iPhoneAppDelegate.h
//  Sora-iPhone
//
//  Created by GriffinBu on 1/30/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class Sora_iPhoneViewController;

@interface Sora_iPhoneAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    Sora_iPhoneViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet Sora_iPhoneViewController *viewController;

@end

