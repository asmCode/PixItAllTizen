//
//  Repaint_ItAppDelegate.m
//  Repaint It
//
//  Created by Majak on 2/7/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "Repaint_ItAppDelegate.h"
#import "Repaint_ItViewController.h"

@implementation Repaint_ItAppDelegate

@synthesize window;
@synthesize viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [self.window addSubview:self.viewController.view];
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
	
	[self.viewController handleEnterBackground];
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Handle any foreground procedures not related to animation here.
	
	[self.viewController handleEnterForeground];
}

- (void)dealloc
{
    [viewController release];
    [window release];
    
    [super dealloc];
}

@end
