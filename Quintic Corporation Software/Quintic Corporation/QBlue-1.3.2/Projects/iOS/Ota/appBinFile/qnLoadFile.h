//
//  qnLoadFile.h
//  Qpp Demo
//
//  @brief Application Programming Interface Header File for Load File.
//
//  Created by quintic on 5/18/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface qnLoadFile : NSFileManager
{

}

/**
 ****************************************************************************************
 * @brief App get the Documents Directory of Ota.app.
 *
 * @return : the the Documents Directory.
 *
 ****************************************************************************************
 */
-(NSString *)appDocumentsDirectory;


/**
 ****************************************************************************************
 * @brief App enum all filenames under the Documents Directory of Ota.app.
 *
 * @param[in]  none
 *
 * @return :  these filenames list under the Documents Directory of Ota.app.
 ****************************************************************************************
 */
-(NSArray *)enumBinFiles;

/**
 ****************************************************************************************
 * @brief App access a new app file .
 *
 * @param[in] filename : the file to read.
 *
 * @return :  the new app's data object.
 ****************************************************************************************
 */
-(NSData *)readBinFile :(NSString *)filename;


+ (qnLoadFile *)sharedInstance;

@end


