#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "JniAndroid.h"
#include "FindDeviceScene.h"
#include "socket/SocketThread.h"
#include "DeviceControllerScene.h"
#import "QRReaderViewController.h"

#import <sys/utsname.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>
#import <SystemConfiguration/SCNetworkReachability.h>
#import <SystemConfiguration/CaptiveNetwork.h>
#import "Vibrate.h"
#import "NetServiceBrowser.h"
#import "Reachability.h"

#import <AddressBook/AddressBook.h>
#import <Contacts/Contacts.h>

NetServiceBrowser *netServiceBrowser = nil;//Bonjour客户端发现本地服务

JniAndroid::JniAndroid()
{
	mJniRequestedOrientation = SCREEN_ORIENTATION_UNSPECIFIED;
}

JniAndroid::~JniAndroid()
{
}


void JniAndroid::JniSetRequestedOrientation(int requestedOrientation)
{
    mJniRequestedOrientation = requestedOrientation;
}

int JniAndroid::JniGetRequestedOrientation()
{
    return mJniRequestedOrientation;
}

void JniAndroid::JniStartJmdns()
{
    JniRemoveJmdns();
    netServiceBrowser = [[NetServiceBrowser alloc] init];
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(NET_SERVICE_BROWSER_OVER);
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(NET_SERVICE_BROWSER_OVER, [=](EventCustom* eve)
                                                                          {
                                                                              auto scene = Director::getInstance()->getRunningScene();
                                                                              auto layer = (FindDeviceScene *)scene->getChildByTag(FIND_DEVICE_SCENE_TAG);
                                                                              if (layer)
                                                                              {
                                                                                  layer->replaceScene(false);
                                                                              }
                                                                          });
   
}

void JniAndroid::JniRemoveJmdns()
{
    if(netServiceBrowser)
    {
        [netServiceBrowser release];
        netServiceBrowser = nil;
    }
}


void JniAndroid::JniCreateScene()
{
    
}

void JniAndroid::JnidownloadHelper(const char *url)
{
//    IOS_APP_STORE or IOS_ENTERPRISE url, e.g. xiami app store url
//    url = "https://itunes.apple.com/cn/app/xia-mi-yin-le-wo-shi-ge-shou/id595594905?l=en&mt=8";
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];
}
int JniAndroid::JniGetClientVersionCode()
{
    return [[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"] intValue];
}
std::string JniAndroid::JniGetClientVersionName()
{
    return [[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"] UTF8String];
}
std::string JniAndroid::JniGetWifiName()
{
    NSString *ssid = @"";
    NSArray *ifs = (id)CNCopySupportedInterfaces();
    NSLog(@"ifs:%@",ifs);
    for (NSString *ifnam in ifs) {
        NSDictionary *info = (id)CNCopyCurrentNetworkInfo((CFStringRef)ifnam);
        NSLog(@"dici：%@",[info  allKeys]);
        if (info[@"SSID"]) {
            ssid = info[@"SSID"];
            
        }
    }
    return [ssid UTF8String];
}
void JniAndroid::JniVibrate(long milliseconds)
{
    [[Vibrate sharedVibrate] start:(int)milliseconds];
}
void JniAndroid::JniDownload()
{
}
bool JniAndroid::JniIsInstalledApk()
{
    return false;
}
void JniAndroid::JniOpenApk()
{
}

void JniAndroid::JniSetupWifi()
{
    // WIFI设置界面
    NSURL *url = [NSURL URLWithString:@"prefs:root=WIFI"];
    if ([[UIApplication sharedApplication] canOpenURL:url])
    {
        [[UIApplication sharedApplication] openURL:url];
    }
}

JniAndroid::NetWorkState JniAndroid::JniGetConnectState()
{
    Reachability *reachability=[Reachability reachabilityForLocalWiFi];
    NetworkStatus netStatus = [reachability currentReachabilityStatus];
    if (netStatus == ReachableViaWiFi)
    {
        return NetWorkState::WIFI;
    }
    else
    {
        return NetWorkState::NONE;
    }
}

void JniAndroid::JniOpenSanQr()
{
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(QR_VIEW_CONTROLLER_RESULT);
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(QR_VIEW_CONTROLLER_RESULT, [=](EventCustom* eve)
                                                                          {
                                                                              std::string *strResult = (std::string*)eve->getUserData();
                                                                              if (strResult)
                                                                              {
                                                                                  auto scene = Director::getInstance()->getRunningScene();
                                                                                  auto layer = (DeviceControllerScene *)scene->getChildByName(DEVICE_CONTROLLER_SCENE);
                                                                                  if (layer)
                                                                                  {
                                                                                      layer->dialogQrSan(0, *strResult);
                                                                                  }
                                                                              }
                                                                          });
    QRReaderViewController *vc = [[QRReaderViewController alloc] init];
    vc.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;//混合溶入模式
    [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:vc animated:YES completion:^{
     NSLog(@"cocos2dx调用oc函数");
     }];
}

std::string JniAndroid::JniGetPhoneModel()
{
    struct utsname systemInfo;
    uname(&systemInfo);
    
    NSString *platform = [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
    
    if ([platform isEqualToString:@"iPhone1,1"]) return "iPhone 2G (A1203)";
    if ([platform isEqualToString:@"iPhone1,2"]) return "iPhone 3G (A1241/A1324)";
    if ([platform isEqualToString:@"iPhone2,1"]) return "iPhone 3GS (A1303/A1325)";
    if ([platform isEqualToString:@"iPhone3,1"]) return "iPhone 4 (A1332)";
    if ([platform isEqualToString:@"iPhone3,2"]) return "iPhone 4 (A1332)";
    if ([platform isEqualToString:@"iPhone3,3"]) return "iPhone 4 (A1349)";
    if ([platform isEqualToString:@"iPhone4,1"]) return "iPhone 4S (A1387/A1431)";
    if ([platform isEqualToString:@"iPhone5,1"]) return "iPhone 5 (A1428)";
    if ([platform isEqualToString:@"iPhone5,2"]) return "iPhone 5 (A1429/A1442)";
    if ([platform isEqualToString:@"iPhone5,3"]) return "iPhone 5c (A1456/A1532)";
    if ([platform isEqualToString:@"iPhone5,4"]) return "iPhone 5c (A1507/A1516/A1526/A1529)";
    if ([platform isEqualToString:@"iPhone6,1"]) return "iPhone 5s (A1453/A1533)";
    if ([platform isEqualToString:@"iPhone6,2"]) return "iPhone 5s (A1457/A1518/A1528/A1530)";
    if ([platform isEqualToString:@"iPhone7,1"]) return "iPhone 6 Plus (A1522/A1524)";
    if ([platform isEqualToString:@"iPhone7,2"]) return "iPhone 6 (A1549/A1586)";
    if ([platform isEqualToString:@"iPhone8,1"]) return "iPhone 6s Plus";
    if ([platform isEqualToString:@"iPhone8,2"]) return "iPhone 6s";
    
    if ([platform isEqualToString:@"iPod1,1"])   return "iPod Touch 1G (A1213)";
    if ([platform isEqualToString:@"iPod2,1"])   return "iPod Touch 2G (A1288)";
    if ([platform isEqualToString:@"iPod3,1"])   return "iPod Touch 3G (A1318)";
    if ([platform isEqualToString:@"iPod4,1"])   return "iPod Touch 4G (A1367)";
    if ([platform isEqualToString:@"iPod5,1"])   return "iPod Touch 5G (A1421/A1509)";
    
    if ([platform isEqualToString:@"iPad1,1"])   return "iPad 1G (A1219/A1337)";
    
    if ([platform isEqualToString:@"iPad2,1"])   return "iPad 2 (A1395)";
    if ([platform isEqualToString:@"iPad2,2"])   return "iPad 2 (A1396)";
    if ([platform isEqualToString:@"iPad2,3"])   return "iPad 2 (A1397)";
    if ([platform isEqualToString:@"iPad2,4"])   return "iPad 2 (A1395+New Chip)";
    if ([platform isEqualToString:@"iPad2,5"])   return "iPad Mini 1G (A1432)";
    if ([platform isEqualToString:@"iPad2,6"])   return "iPad Mini 1G (A1454)";
    if ([platform isEqualToString:@"iPad2,7"])   return "iPad Mini 1G (A1455)";
    
    if ([platform isEqualToString:@"iPad3,1"])   return "iPad 3 (A1416)";
    if ([platform isEqualToString:@"iPad3,2"])   return "iPad 3 (A1403)";
    if ([platform isEqualToString:@"iPad3,3"])   return "iPad 3 (A1430)";
    if ([platform isEqualToString:@"iPad3,4"])   return "iPad 4 (A1458)";
    if ([platform isEqualToString:@"iPad3,5"])   return "iPad 4 (A1459)";
    if ([platform isEqualToString:@"iPad3,6"])   return "iPad 4 (A1460)";
    
    if ([platform isEqualToString:@"iPad4,1"])   return "iPad Air (A1474)";
    if ([platform isEqualToString:@"iPad4,2"])   return "iPad Air (A1475)";
    if ([platform isEqualToString:@"iPad4,3"])   return "iPad Air (A1476)";
    if ([platform isEqualToString:@"iPad4,4"])   return "iPad Mini 2G (A1489)";
    if ([platform isEqualToString:@"iPad4,5"])   return "iPad Mini 2G (A1490)";
    if ([platform isEqualToString:@"iPad4,6"])   return "iPad Mini 2G (A1491)";
    
    if ([platform isEqualToString:@"i386"])      return "iPhone Simulator";
    if ([platform isEqualToString:@"x86_64"])    return "iPhone Simulator";
    return [platform UTF8String];
    
    
}




void JniAndroid::getPhoneContacts(std::vector<Contacts> &mContacts)
{
    
    CFErrorRef error;
    //新建一个通讯录类
    ABAddressBookRef _addressBook = ABAddressBookCreateWithOptions(NULL,&error);
    if ([[UIDevice currentDevice].systemVersion floatValue]>=6.0)
    {
        ABAddressBookRequestAccessWithCompletion(_addressBook, nullptr);
        ABAuthorizationStatus authStatus = ABAddressBookGetAuthorizationStatus();
        if(authStatus != kABAuthorizationStatusAuthorized)
        {
            log("未授权，请打开");
            UIAlertView *av = [[UIAlertView alloc]
                               initWithTitle:@"好牌"
                               message:@"你未开启”允许好牌访问通讯录“，请到系统设置-隐私-通讯录中开启"
                               delegate:nil
                               cancelButtonTitle:nil
                               otherButtonTitles:@"确定", nil, nil];
            [av show];
            return;
        }
    }
    
    
    if (_addressBook)
    {
        if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 9.0)
        {
            CNContactStore *store = [[CNContactStore alloc] init];
            CNContactFetchRequest *request = [[CNContactFetchRequest alloc] initWithKeysToFetch:@[CNContactFamilyNameKey,CNContactGivenNameKey,CNContactPhoneNumbersKey]];
            NSError *error = nil;
            //执行获取通讯录请求，若通讯录可获取，flag为YES，代码块也会执行，若获取失败，flag为NO，代码块不执行
            BOOL flag = [store enumerateContactsWithFetchRequest:request error:&error usingBlock:^(CNContact * _Nonnull contact, BOOL * _Nonnull stop)
                         {
                             
                             struct Contacts tmpContacts;
                             
                             //名字
                             NSString *name = @"";
                             if ([NSString stringWithFormat:@"%@%@",contact.familyName,contact.givenName]) {
                                 name =  [NSString stringWithFormat:@"%@%@",contact.familyName,contact.givenName];
                                 
                             }
                             tmpContacts.name = [name UTF8String];
                             
                             //号码
                             NSString *strPhone = @"";
                             for(CNLabeledValue<CNPhoneNumber*>* phone in contact.phoneNumbers)
                             {
                                 strPhone = phone.value.stringValue;
                                 tmpContacts.strTelephone.push_back([strPhone UTF8String]);
                             }
                             
                             mContacts.push_back(tmpContacts);
                         }];
            if (flag)
            {
                NSLog(@"通讯录可获取");
            }
            [request autorelease];
            [store autorelease];
            
        }
        else
        {
            //获取所有联系人的数组
            NSArray*  _arrayRef = (NSArray*) ABAddressBookCopyArrayOfAllPeople(_addressBook);
            //循环, 获取每个人的个人信息
            for (id obj in _arrayRef)
            {
                struct Contacts tmpContacts;
                
                //获取个人
                ABRecordRef person = (__bridge ABRecordRef)obj;
                //获取个人名字
                CFTypeRef abName = ABRecordCopyValue(person, kABPersonFirstNameProperty);
                CFTypeRef abLastName = ABRecordCopyValue(person, kABPersonLastNameProperty);
                CFStringRef abFullName = ABRecordCopyCompositeName(person);
                NSString* nameString = (__bridge NSString*)abName;
                NSString* lastNameString = (__bridge NSString*)abLastName;
                if ((__bridge id) abFullName != nil)
                {
                    nameString = (__bridge NSString*)abFullName;
                }
                else
                {
                    if ((__bridge id)abLastName != nil)
                    {
                        nameString = [NSString stringWithFormat:@"%@%@",nameString,lastNameString];
                    }
                }
                
                tmpContacts.name = [nameString UTF8String];
                ABMultiValueRef phones= ABRecordCopyValue(obj, kABPersonPhoneProperty);
                for (NSInteger j=0; j<ABMultiValueGetCount(phones); j++)
                {
                    NSString *phone =(__bridge NSString*)ABMultiValueCopyValueAtIndex(phones, j);
                    NSLog(@"%@: %@", nameString, phone);
                    tmpContacts.strTelephone.push_back([phone UTF8String]);
                }
                
                if (abName) CFRelease(abName);
                if (abLastName) CFRelease(abLastName);
                if (abFullName) CFRelease(abFullName);
                mContacts.push_back(tmpContacts);
                
            }
        }
        log("===========得到通讯录数据===================");
        for(auto person : mContacts)
        {
            for(auto telephone : person.strTelephone)
            {
                log("%s : %s", person.name.c_str(), telephone.c_str());
            }
        }
    }

    
    //详见：http://www.oschina.net/code/snippet_2248391_51913
    //去除数字以外的所有字符
//    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 9.0)
//    {
//        CNContactStore *store = [[CNContactStore alloc] init];
//        CNContactFetchRequest *request = [[CNContactFetchRequest alloc] initWithKeysToFetch:@[CNContactFamilyNameKey,CNContactGivenNameKey,CNContactPhoneNumbersKey]];
//        NSError *error = nil;
//        NSMutableArray *nameArray = [[NSMutableArray alloc] init];
//        NSMutableArray *phoneArray = [[NSMutableArray alloc]init];
//        //执行获取通讯录请求，若通讯录可获取，flag为YES，代码块也会执行，若获取失败，flag为NO，代码块不执行
//        BOOL flag = [store enumerateContactsWithFetchRequest:request error:&error usingBlock:^(CNContact * _Nonnull contact, BOOL * _Nonnull stop)
//                     {
//                         //去除数字以外的所有字符
//                         NSCharacterSet *setToRemove = [[ NSCharacterSet characterSetWithCharactersInString:@"0123456789"]
//                                                        invertedSet ];
//                         NSString *strPhone = @"";
//                         if (contact.phoneNumbers.count>0) {
//                             strPhone  = [[[contact.phoneNumbers firstObject].value.stringValue componentsSeparatedByCharactersInSet:setToRemove] componentsJoinedByString:@""];
//                         }
//                         [phoneArray addObject:strPhone];
//                         NSString *name = @"";
//                         if ([NSString stringWithFormat:@"%@%@",contact.familyName,contact.givenName]) {
//                             name =  [NSString stringWithFormat:@"%@%@",contact.familyName,contact.givenName];
//                         }
//                         [nameArray addObject:name];
//                     }];
//        if (flag) {
//            NSLog(@"手机号%@",[phoneArray componentsJoinedByString:@","]);
//            NSLog(@"名字%@",[nameArray componentsJoinedByString:@","]);
//        }
//    }
//    else
//    {
//        NSMutableArray *nameArray = [[NSMutableArray alloc] init];
//        NSMutableArray *phoneArray = [[NSMutableArray alloc]init];
//        CFErrorRef *error = nil;
//        ABAddressBookRef addressBook = ABAddressBookCreateWithOptions(NULL, error);
//        __block BOOL accessGranted = NO;
//        
//        dispatch_semaphore_t sema = dispatch_semaphore_create(0);
//        ABAddressBookRequestAccessWithCompletion(addressBook, ^(bool granted, CFErrorRef error)
//        {
//            accessGranted = granted;
//            dispatch_semaphore_signal(sema);
//        });
//        dispatch_semaphore_wait(sema, DISPATCH_TIME_FOREVER);
//        
//        if (accessGranted)
//        {
//            
//            CFArrayRef allPeople = ABAddressBookCopyArrayOfAllPeople (addressBook);
//            CFIndex nPeople = ABAddressBookGetPersonCount (addressBook);
//            
//            
//            for ( NSInteger i = 0 ; i < nPeople; i++)
//            {
//                ABRecordRef person = CFArrayGetValueAtIndex (allPeople, i);
//                NSString *givenName = (__bridge NSString *)(ABRecordCopyValue (person, kABPersonFirstNameProperty )) == nil ? @"" : (__bridge NSString *)(ABRecordCopyValue (person, kABPersonFirstNameProperty ));
//                NSString *familyName = (__bridge NSString *)(ABRecordCopyValue (person, kABPersonLastNameProperty )) == nil ? @"" : (__bridge NSString *)(ABRecordCopyValue (person, kABPersonLastNameProperty ));
//                ABMultiValueRef phoneNumbers = ABRecordCopyValue(person, kABPersonPhoneProperty);
//                NSArray *array = CFBridgingRelease(ABMultiValueCopyArrayOfAllValues(phoneNumbers));
//                NSString *phoneNumber = @"";
//                if (array.count > 0)
//                {
//                    phoneNumber = [array firstObject];
//                }
//                
//                NSDictionary *dic = [[NSDictionary alloc] initWithObjectsAndKeys:[NSString stringWithFormat:@"%@%@",familyName,givenName],@"name",phoneNumber,@"phone",[NSNumber numberWithBool:NO],@"isUser",nil];
//                //去除数字以外的所有字符
//                NSCharacterSet *setToRemove = [[ NSCharacterSet characterSetWithCharactersInString:@"0123456789"]
//                                               invertedSet ];
//                NSString *strPhone = [[phoneNumber componentsSeparatedByCharactersInSet:setToRemove] componentsJoinedByString:@""];
//                [phoneArray addObject:strPhone];
//                NSString *name = [NSString stringWithFormat:@"%@%@",familyName,givenName];
//                [nameArray addObject:name];
//            }
//            NSLog(@"手机号%@",[phoneArray componentsJoinedByString:@","]);
//            NSLog(@"名字%@",[nameArray componentsJoinedByString:@","]);
//        }
//    }
    
}



#endif
