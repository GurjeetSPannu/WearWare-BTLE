//
//  TabBarAppDelegate.m
//  Qpp Demo
//
//  @brief Application Source File for App Delegate.
//
//  Created by quintic on 5/18/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import "TabBarAppDelegate.h"

#import "TabBarP2CViewController.h"

#import "TabBarC2PViewController.h"
#import "TabBarHelpViewController.h"

@implementation TabBarAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    // Override point for customization after application launch.
    UIViewController *viewController1, *viewController2, *helpViewController;
    
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        viewController1 = [[TabBarP2CViewController alloc] initWithNibName:@"TabBarP2CViewController_iPhone" bundle:nil];
        viewController2 = [[TabBarC2PViewController alloc] initWithNibName:@"TabBarC2PViewController_iPhone" bundle:nil];
        helpViewController = [[TabBarHelpViewController alloc] initWithNibName:@"TabBarHelpViewController_iPhone" bundle:nil];
        
    } else {
        viewController1 = [[TabBarP2CViewController alloc] initWithNibName:@"TabBarP2CViewController_iPad" bundle:nil];
        viewController2 = [[TabBarC2PViewController alloc] initWithNibName:@"TabBarC2PViewController_iPad" bundle:nil];
        helpViewController = [[TabBarHelpViewController alloc] initWithNibName:@"TabBarHelpViewController_iPad" bundle:nil];
    }
    
    self.tabBarController = [[UITabBarController alloc] init];
    self.tabBarController.viewControllers = @[viewController1, viewController2, helpViewController];
    self.window.rootViewController = self.tabBarController;
    [self.window makeKeyAndVisible];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

/*
// Optional UITabBarControllerDelegate method.
- (void)tabBarController:(UITabBarController *)tabBarController didSelectViewController:(UIViewController *)viewController
{
}
*/

/*
// Optional UITabBarControllerDelegate method.
- (void)tabBarController:(UITabBarController *)tabBarController didEndCustomizingViewControllers:(NSArray *)viewControllers changed:(BOOL)changed
{
}
*/

@end
