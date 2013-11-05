//
//  Repaint_ItAppDelegate.h
//  Repaint It
//
//  Created by Majak on 2/7/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class Repaint_ItViewController;

@interface Repaint_ItAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    Repaint_ItViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet Repaint_ItViewController *viewController;

@end

