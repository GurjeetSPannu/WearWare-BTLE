//
//  CustomAlertView.m
//  Qpp Demo
//
//  @brief Application Source File for Customer Alert View.
//
//  Created by quintic on 5/18/14.
//  Copyright (c) 2014 Quintic. All rights reserved.
//

#define keyAlertViewTitleFont             [UIFont boldSystemFontOfSize:20]
#define keyAlertViewTitleTextColor        [UIColor colorWithWhite:244.0/255.0 alpha:1.0]
#define keyAlertViewTitleShadowColor      [UIColor blackColor]
#define keyAlertViewTitleShadowOffset     CGSizeMake(0, -1)

#define keyAlertViewMessageFont           [UIFont systemFontOfSize:18]
#define keyAlertViewMessageTextColor      [UIColor colorWithWhite:244.0/255.0 alpha:1.0]
#define keyAlertViewMessageShadowColor    [UIColor blackColor]
#define keyAlertViewMessageShadowOffset   CGSizeMake(0, -1)

#define keyAlertViewButtonFont            [UIFont boldSystemFontOfSize:18]
#define keyAlertViewButtonTextColor       [UIColor whiteColor]
#define keyAlertViewButtonShadowColor     [UIColor blackColor]
#define keyAlertViewButtonShadowOffset    CGSizeMake(0, -1)

#define keyAlertViewBackground            @"alertBar.png"
#define keyAlertViewBackgroundCapHeight   38


#import "CustomAlertView.h"

@implementation CustomAlertView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

-(void)layoutSubviews{
    
    for (UIView *v in self.subviews) {
        if ([v isKindOfClass:[UIImageView class]]) {
            UIImageView *imageV = (UIImageView *)v;
            UIImage *image = [UIImage imageNamed:keyAlertViewBackground];
            image = [image stretchableImageWithLeftCapWidth:0 topCapHeight:keyAlertViewBackgroundCapHeight /* retain */];
            [imageV setImage:image];
        }
        if ([v isKindOfClass:[UILabel class]]) {
            UILabel *label = (UILabel *)v;
            label.textAlignment = 1;
            label.lineBreakMode = 0;
            label.numberOfLines = 0;
            label.backgroundColor = [UIColor clearColor];
            
            if ([label.text isEqualToString:self.title])
            {
                label.font = keyAlertViewTitleFont;
                label.textColor = keyAlertViewTitleTextColor;
                label.shadowColor = keyAlertViewTitleShadowColor;
                label.shadowOffset = keyAlertViewTitleShadowOffset;
                
            }
            else
            {
                label.font = keyAlertViewMessageFont;
                label.textColor = keyAlertViewMessageTextColor;
                label.shadowColor = keyAlertViewMessageShadowColor;
                label.shadowOffset = keyAlertViewMessageShadowOffset;
            }

        }
        
        if ([v isKindOfClass:NSClassFromString(@"UIAlertButton")])
        {
            UIButton *button = (UIButton *)v;
            UIImage *image = nil;
            
            if (button.tag == 1)
            {
                image = [UIImage imageNamed:[NSString stringWithFormat:@"QnGrayButton.png"]];
            }
            else
            {
                image = [UIImage imageNamed:[NSString stringWithFormat:@"QnBlackButton"]];
            }
            
            image = [image stretchableImageWithLeftCapWidth:(int)(image.size.width+1)>>1 topCapHeight:0];
            
            button.titleLabel.font = keyAlertViewButtonFont;
            button.titleLabel.textAlignment = 1;
            button.titleLabel.shadowOffset = keyAlertViewButtonShadowOffset;
            button.backgroundColor = [UIColor clearColor];
            [button setBackgroundImage:image forState:UIControlStateNormal];
            [button setTitleColor:keyAlertViewButtonTextColor forState:UIControlStateNormal];
            [button setTitleShadowColor:keyAlertViewButtonShadowColor forState:UIControlStateNormal];
        }
    }
}
 
@end
