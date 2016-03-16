//
//  Vibrate.h
//  PhoneHelper
//
//  Created by gloud on 16/2/23.
//
//

#import <Foundation/Foundation.h>

@interface Vibrate : NSObject
+ (id)sharedVibrate;
- (void)start:(int)time;
@end
