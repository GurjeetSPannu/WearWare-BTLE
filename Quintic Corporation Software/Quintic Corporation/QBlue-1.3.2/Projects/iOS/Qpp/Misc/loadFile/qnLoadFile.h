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

#define keyQBlueSelectedFileName    @"key_QBlueSelectedFile"
#define qBlueSelectedOneFileNoti    @"qBlueSelectedOneFileNotification"


#define keyQppFileData              @"key-QppFileData"
#define qppFileDataNoti             @"qppFileDataNotification"

@interface qnLoadFile : NSFileManager
{
    NSData *fwCodeDataStr;
    
    const uint8_t *fwCodeBytes;
    uint16_t fwCodeBytesLength;
}

-(NSString *)appDocumentsDirectory;

-(NSData *)readBinFile :(NSString *)filename;
-(NSArray *)enumBinFiles;

+ (qnLoadFile *)sharedInstance;

@end


