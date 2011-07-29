//
//  Sora_iPhoneAppDelegate.m
//  Sora-iPhone
//
//  Created by GriffinBu on 1/30/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "Sora_iPhoneAppDelegate.h"
#import "Sora_iPhoneViewController.h"

@implementation Sora_iPhoneAppDelegate

@synthesize window;
@synthesize viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window.rootViewController = self.viewController;
	CGRect  rect = [[UIScreen mainScreen] bounds];
    [window setFrame:rect];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [self.viewController stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [self.viewController startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [self.viewController stopAnimation];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Handle any background procedures not related to animation here.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Handle any foreground procedures not related to animation here.
}


- (void)dealloc
{
    [viewController release];
    [window release];
    
    [super dealloc];
}

@end
