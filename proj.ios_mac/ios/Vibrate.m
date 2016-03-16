//
//  Vibrate.m
//  PhoneHelper
//
//  Created by gloud on 16/2/23.
//
//

#import "Vibrate.h"
#import <AudioToolbox/AudioToolbox.h>
@implementation Vibrate

+ (id)sharedVibrate
{

    static Vibrate *sharedVibrate = nil;
    static dispatch_once_t onceToken;
    //GCG线程安全单例
    dispatch_once(&onceToken, ^{
        sharedVibrate = [[self alloc] init];
    });
    return sharedVibrate;
}

- (void)start:(int)timer
{
    if (true)
    {
        //private api有风险，注意不要在上架app中使用
        NSMutableDictionary* patternsDict = [@{} mutableCopy];
        NSMutableArray* patternsArray = [@[] mutableCopy];
        
        [patternsArray addObject:@(YES)];
        [patternsArray addObject:@(timer)]; //vibrate for timer
        
        [patternsDict setObject:patternsArray forKey:@"VibePattern"];
        [patternsDict setObject:[NSNumber numberWithInt:1.0] forKey:@"Intensity"];
        
        // suppress warnings
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
        AudioServicesStopSystemSound(kSystemSoundID_Vibrate);
        
        AudioServicesPlaySystemSoundWithVibration(kSystemSoundID_Vibrate,nil,patternsDict);
#pragma clang diagnostic pop
        
    }
    else
    {
        //1s~2s
        AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
    }
    

}

- (id)init
{
    if (self = [super init]) {
    }
    return self;
}

@end
