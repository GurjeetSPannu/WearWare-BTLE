//
//  DeviceViewController.h
//  Qpp Demo
//
//  @brief Application Source File for Device List View Controller.
//
//  Created by quintic on 5/18/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import "QBleClient.h"
#import "QppPublic.h"
#import "DeviceViewController.h"

@interface DeviceViewController ()

@end

@implementation DeviceViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

+(DeviceViewController *)sharedInstance{
    static DeviceViewController *_sharedInstance = nil;
    if (_sharedInstance == nil) {
        _sharedInstance = [[DeviceViewController alloc] init];
    }
    
    return _sharedInstance;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceListReloadDataRsp) name:ReloadDevListDataNoti object:nil];
    
    _refreshControl = [[UIRefreshControl alloc] init];
    [_refreshControl addTarget:self action:@selector(updatePeriInTableView:) forControlEvents:UIControlEventValueChanged];
    [deviceList addSubview:_refreshControl];
}

- (void)viewUnDidLoad
{
    deviceList = nil;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/**
 ****************************************************************************************
 * @brief       back to MainViewController.
 *
 * @param[in]  sender   : current sender id .
 * @return :   IBAction : Button Id Action
 ****************************************************************************************
 */
- (IBAction)backQppPeri2CentVC:(id)sender {
    [self dismissModalViewControllerAnimated:YES];
}

/**
 ****************************************************************************************
 * @brief       device number in the table view.
 *
 * @param[in]  sender   : current sender id.
 *
 * @return :   NSInteger : number of rows in the table
 ****************************************************************************************
 */
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    //    NSLog(@"%s ", __func__);
    
    return [[qBleClient sharedInstance].discoveredPeripherals count];
}

/**
 ****************************************************************************************
 * @brief
 * // Row display. Implementers should *always* try to reuse cells by setting each cell's
 *    reuseIdentifier and querying for available reusable cells with dequeueReusableCellWithIdentifier:
 * // Cell gets various attributes set automatically based on table (separators) and data
 *    source (accessory views, editing controls)
 *
 * @param[in]  indexPath   : index of the row.
 *
 * @return :   returns nil if cell is not visible or index path is out of range
 ****************************************************************************************
 */
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = nil;
    
    if (!cell) {
        cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    if ([[qBleClient sharedInstance] discoveredPeripherals] == nil)
    {
        NSLog(@"cell:nil \n");
        return cell;
    }
    
    NSArray *peripherals = [[qBleClient sharedInstance] discoveredPeripherals];
    
    //    NSLog(@"arr :%d \n",indexPath.row);
    CBPeripheral *peripheral = [peripherals objectAtIndex:indexPath.row];
    
    cell.textLabel.text = [peripheral name];
    
    return cell;
}

/**
 ****************************************************************************************
 * @brief      // Called after the user changes the selection.
 *
 * @return :   none
 ****************************************************************************************
 */
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    qBleClient *dev = [qBleClient sharedInstance];
    [dev stopScan];
    
    NSArray *peripherals = [dev discoveredPeripherals];
    
    // protect code
    uint8_t perIndex = indexPath.row;
    
    if (perIndex > [peripherals count]){
        perIndex = [peripherals count];
    };
    
    // CBPeripheral *peripheral
    CBPeripheral *selectedPeri = [peripherals objectAtIndex:perIndex /*indexPath.row*/];
    
    NSDictionary *dictPeri = [NSDictionary dictionaryWithObject : selectedPeri forKey:keyQppSelectPeri];
    [[NSNotificationCenter defaultCenter] postNotificationName: qppSelOnePeripheralNoti object:nil userInfo:dictPeri];
    
    [dev pubConnectPeripheral : selectedPeri];
    
    [self backQppPeri2CentVC:nil];
}

- (void)endRefresh
{
    [_refreshControl endRefreshing];
}

-(void) updatePeriInTableView:(UIRefreshControl *)refreshControl
{
    NSLog(@"%s ", __func__);
    
    [refreshControl beginRefreshing];
    
    [deviceList reloadData];
    
    [NSTimer scheduledTimerWithTimeInterval:1.0 target:self selector:@selector(endRefresh) userInfo:nil repeats:NO];
    //[self performSelector:@selector(endRefresh:) withObject:refreshControl afterDelay:1.0f];
}

-(void)deviceListReloadDataRsp
{
    NSLog(@"%s ", __func__);
    [deviceList reloadData];
}
@end
