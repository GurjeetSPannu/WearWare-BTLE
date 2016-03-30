//
//  qppFileViewController.m
//  Qpp Demo
//
//  @brief Application Source File for File List View Controller.
//
//  Created by quintic on 5/18/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import "qnLoadFile.h"

#import "qppPublic.h"
#import "qppFileViewController.h"

@interface qppFileViewController ()

@end

@implementation qppFileViewController

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
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(fileListReloadDataRsp) name:reloadFileListDataNoti object:nil];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)backQppCentral2PeriVC:(id)sender {
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
    
    NSArray *binFiles = [[qnLoadFile sharedInstance] enumBinFiles];
    
    // NSLog(@"arr peripherals:%@ \n",peripherals);
    NSString *binFileName = [binFiles objectAtIndex:indexPath.row];
    
    cell.textLabel.text = binFileName;
    
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
    // CBPeripheral *peripheral
    NSDictionary *dictFile = [NSDictionary dictionaryWithObject : [[[qnLoadFile sharedInstance] enumBinFiles] objectAtIndex:indexPath.row] forKey : keyQnSelectFile];
    
    [[NSNotificationCenter defaultCenter]postNotificationName:strQnSelectedOneFileNoti object:self userInfo:dictFile];
    
    [self backQppCentral2PeriVC:nil];
}

/**
 ****************************************************************************************
 * @brief      reload data for file list.
 *
 * @return :   none
 ****************************************************************************************
 */
-(void)fileListReloadDataRsp
{
    [qppFileList reloadData];
}

@end
