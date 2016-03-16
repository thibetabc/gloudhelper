//
//  NetServiceBrowser.m
//  PhoneHelper
//
//  Created by gloud on 16/2/23.
//  Bonjour客户端发现本地服务
//

#import "NetServiceBrowser.h"
#import "FindDeviceScene.h"

@interface NetServiceBrowser () <NSNetServiceBrowserDelegate>

@property (nonatomic, strong)NSNetServiceBrowser *serviceBrowser;

@end

@implementation NetServiceBrowser


- (instancetype)init
{
    self = [super init];
    if (self) {
        _serviceBrowser = [[NSNetServiceBrowser alloc] init];
        _serviceBrowser.delegate = self;
        [_serviceBrowser searchForServicesOfType:@"_gloudprivateprotocol._tcp." inDomain:@"local."];
        [NSTimer scheduledTimerWithTimeInterval:2.0 target:self selector:@selector(timerFireMethod:) userInfo:nil repeats:NO];
        NSLog(@"NetServiceBrowser start ");

    }
    return self;
}

- (void)timerFireMethod:(NSTimer*)theTimer
{
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NET_SERVICE_BROWSER_OVER, nullptr);
}


//解析服务失败，解析出错
- (void)netService:(NSNetService *)netService didNotResolve:(NSDictionary *)errorDict {
    
    NSLog(@"didNotResolve: errorDict = %@",errorDict);
    
}

//已发现服务
- (void)netServiceBrowser:(NSNetServiceBrowser *)netServiceBrowser didFindService:(NSNetService *)netService moreComing:(BOOL)moreServicesComing
{
    NSLog(@"didFindService netService name: %@\n", [netService name]);
    if (netService.name)
    {
        [self addData:netService.name];
    }
    
}


- (void)addData:(NSString *)msg
{
    auto scene = Director::getInstance()->getRunningScene();
    auto layer = (FindDeviceScene *)scene->getChildByTag(FIND_DEVICE_SCENE_TAG);
    if (layer)
    {
        layer->addMsgJmDNS([msg UTF8String]);
    }
}

- (void)dealloc
{
    
    _serviceBrowser.delegate = nil;
    [super dealloc];
    NSLog(@"NetServiceBrowser dealloc");
}

@end
