//
//  DevicesListViewController.m
//  bleDevMonitor
//
//  Created by Derek on 12/05/13.
//  Copyright (c) 2012 QN Inc. All rights reserved.
//

#import "QppClient.h"
#import "OtaDeviceViewController.h"

@interface OtaDeviceViewController ()

@end

@implementation OtaDeviceViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
    self.title = @"Peripheral List";
    bleDevMonitor *qppc = [bleDevMonitor sharedInstance];
    qppc.connectionDelegate = self;
    [qppc startScan];
}

- (void)dealloc {
    bleDevMonitor *qppc = [bleDevMonitor sharedInstance];
    qppc.connectionDelegate = nil;
    [qppc stopScan];
}

#pragma mark - bleDevMonitorDelegate

- (void)bleDevMonitor:(bleDevMonitor *)client didDisconnectPeripheral:(CBPeripheral *)aPeripheral {
    //TODO
}

#pragma mark - UIAlertViewDelegate

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex {
    [[bleDevMonitor sharedInstance] startScan];
}

#pragma mark - Table view data source

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    return [[bleDevMonitor sharedInstance].discoveredPeripherals count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";
    UITableViewCell *cell = nil;
    //[tableView dequeueReusableCellWithIdentifier:CellIdentifier forIndexPath:indexPath];
    
    if (!cell) {
        cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    NSArray *pripherals = [[bleDevMonitor sharedInstance] discoveredPeripherals];
    CBPeripheral *pripheral = [pripherals objectAtIndex:indexPath.row];
    
    cell.textLabel.text = [pripheral name];
    
    return cell;
}


#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    bleDevMonitor *qppc = [bleDevMonitor sharedInstance];
    [qppc stopScan];
    
    NSArray *pripherals = [qppc discoveredPeripherals];
    CBPeripheral *pripheral = [pripherals objectAtIndex:indexPath.row];
    
    [qppc connectPeripheral:pripheral];
}

@end
