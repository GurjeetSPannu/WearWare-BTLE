//
//  qppFileViewController.h
//  Qpp Demo
//
//  @brief Application Header File for File List View Controller.
//
//  Created by quintic on 5/18/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface qppFileViewController : UIViewController
{
    __weak IBOutlet UITableView *qppFileList;
}

/// back to "central to Peripheral" VC
- (IBAction)backQppCentral2PeriVC:(id)sender;

@end
