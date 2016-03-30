//
//  FwFileViewController.m
//  Ota Demo
//
//  @brief Application Source File for File List View Controller.
//
//  Created by quintic on 5/18/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import "qnLoadFile.h"
#import "otaAppPublic.h"
#import "FwFileViewController.h"

@interface FwFileViewController ()

@end

@implementation FwFileViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(fileListReloadDataRsp) name:appFileListReloadDataNoti object:nil];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/**
 ****************************************************************************************
 * @brief back to OTA Main ViewController.
 ****************************************************************************************
 */
- (IBAction)backOtaMainVC:(id)sender {
    [self dismissModalViewControllerAnimated : YES];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    return [[[qnLoadFile sharedInstance] enumBinFiles] count];
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
    
    if ([[qnLoadFile sharedInstance] enumBinFiles] == nil)
    {
        NSLog(@"cell:nil \n");
        return cell;
    }
    
    NSArray *fwFiles = [[qnLoadFile sharedInstance] enumBinFiles];
    
    // NSLog(@"arr peripherals:%@ \n",peripherals);
    NSString *fwFile = [fwFiles objectAtIndex:indexPath.row];
    
    cell.textLabel.text = fwFile;
    
    return cell;
}

/**
 ****************************************************************************************
 * @brief It is called after the user changes the selection.
 ****************************************************************************************
 */
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    // CBPeripheral *peripheral
    NSDictionary *dictFile = [NSDictionary dictionaryWithObject : [[[qnLoadFile sharedInstance] enumBinFiles] objectAtIndex:indexPath.row] forKey:keyOtaAppSelectFile];
    
    [[NSNotificationCenter defaultCenter]postNotificationName:otaAppSelectedOneFileNoti object:self userInfo:dictFile];
    
    [self backOtaMainVC:nil];
}

/**
 ****************************************************************************************
 * @brief response to reload data.
 ****************************************************************************************
 */
-(void)fileListReloadDataRsp
{
    NSLog(@"%s ", __func__);
    [fwFileList reloadData];
}
@end
