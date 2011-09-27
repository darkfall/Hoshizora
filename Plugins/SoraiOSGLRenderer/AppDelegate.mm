//
//  File.cpp
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/19/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#import "AppDelegate.h"

#import "SoraiOSInitializer.h"
#import "SoraiOSMainWindow.h"

@implementation SoraiOSAppDelegate

- (id)init
{
    previousDelegate = [UIApplication sharedApplication].delegate;
    [UIApplication sharedApplication].delegate = self;
    return self;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    if(previousDelegate != nil)
        [previousDelegate application:application didFinishLaunchingWithOptions:launchOptions];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    if(previousDelegate != nil)
        [previousDelegate applicationWillResignActive:application];
    sora::SoraiOSInitializer::Instance()->getMainWindow()->applicationWillResignActive();
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    if(previousDelegate != nil)
        [previousDelegate applicationDidBecomeActive:application];
    sora::SoraiOSInitializer::Instance()->getMainWindow()->applicationDidBecomeActive();
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    if(previousDelegate != nil)
        [previousDelegate applicationWillTerminate:application];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    if(previousDelegate != nil)
        [previousDelegate applicationDidEnterBackground:application];
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    if(previousDelegate != nil)
        [previousDelegate applicationWillEnterForeground:application];
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    if(previousDelegate != nil)
        [previousDelegate applicationDidReceiveMemoryWarning:application];
    sora::SoraiOSInitializer::Instance()->getMainWindow()->didReceiveMemoryWarning();
}

- (void)application:(UIApplication *)application willChangeStatusBarOrientation:(UIInterfaceOrientation)newStatusBarOrientation duration:(NSTimeInterval)duration {
//    if(previousDelegate != nil)
//        [previousDelegate application:application willChangeStatusBarOrientation:newStatusBarOrientation duration:duration];
}

- (void)application:(UIApplication *)application didChangeStatusBarFrame:(CGRect)oldStatusBarFrame {
  //  if(previousDelegate != nil)
  //      [previousDelegate application:application didChangeStatusBarFrame:oldStatusBarFrame];
}

- (void)application:(UIApplication *)application didChangeStatusBarOrientation:(UIInterfaceOrientation)oldStatusBarOrientation {
//    if(previousDelegate != nil)
//        [previousDelegate application:application didChangeStatusBarOrientation: oldStatusBarOrientation];
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error {
    if(previousDelegate != nil)
        [previousDelegate application:application didFailToRegisterForRemoteNotificationsWithError:error];
}

- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification {
    if(previousDelegate != nil)
        [previousDelegate application:application didReceiveLocalNotification: notification];
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
    if(previousDelegate != nil)
        [previousDelegate application:application didReceiveRemoteNotification:userInfo];
}

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    if(previousDelegate != nil)
        [previousDelegate application:application didRegisterForRemoteNotificationsWithDeviceToken:deviceToken];
}


@end