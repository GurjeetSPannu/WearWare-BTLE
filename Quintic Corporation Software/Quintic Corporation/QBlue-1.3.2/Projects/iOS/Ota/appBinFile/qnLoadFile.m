//
//  qnLoadFile.m
//  Qpp Demo
//
//  @brief Application Programming Interface Source File for Load File.
//
//  Created by quintic on 5/18/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import "qnLoadFile.h"

@implementation qnLoadFile

#pragma mark - file process

+ (qnLoadFile *)sharedInstance
{
    static qnLoadFile *_sharedInstance = nil;
    
    if (_sharedInstance == nil) {
        _sharedInstance = [[qnLoadFile alloc] init];
    }
    
    return _sharedInstance;
}

/**
 ****************************************************************************************
 * @brief App get the Documents Directory of Ota.app.
 *
 * @return : the the Documents Directory.
 *
 ****************************************************************************************
 */
-(NSString *)appDocumentsDirectory
{
    // create file manager
    NSFileManager *fileManager = [NSFileManager defaultManager];
    // get the path
    // arg:NSDocumentDirectory: to get the path
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];//
    // change to the path to be access
    [fileManager changeCurrentDirectoryPath:[documentsDirectory stringByExpandingTildeInPath]];
    
    return documentsDirectory;
}

/**
 ****************************************************************************************
 * @brief App enum all filenames under the Documents Directory of Ota.app.
 *
 * @param[in]  none
 *
 * @return :  these filenames list under the Documents Directory of Ota.app.
 ****************************************************************************************
 */
-(NSArray * )enumBinFiles
{
    NSError *error;

    NSString *documentsDirectory = [self appDocumentsDirectory];
    
    // to get file's path
    NSArray *filesList = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:documentsDirectory error:&error];

    return filesList;
}

/**
 ****************************************************************************************
 * @brief App access a new app file .
 *
 * @param[in] filename : the file to read.
 *
 * @return :  the new app's data object.
 ****************************************************************************************
 */
-(NSData *)readBinFile:(NSString *)filename
{
    NSString *documentsDirectory = [self appDocumentsDirectory];
    
    // to get file's path
    NSString *path = [documentsDirectory stringByAppendingPathComponent:filename];
    
    NSData *fileData = [NSData dataWithContentsOfFile:path];
    
    return fileData;
}

@end
