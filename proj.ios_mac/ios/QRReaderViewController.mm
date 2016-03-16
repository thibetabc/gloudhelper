//
//  QRReaderViewController.m
//  PhoneHelper
//
//  Created by gloud on 16/2/1.
//

#import "QRReaderViewController.h"
#import <AVFoundation/AVFoundation.h>
#import <objc/runtime.h>
#import "Global.h"

@interface QRReaderViewController() <AVCaptureMetadataOutputObjectsDelegate>

@property (strong, nonatomic)AVCaptureSession *session;
@property (strong, nonatomic)AVCaptureVideoPreviewLayer *previewLayer;
@property (strong, nonatomic) UILabel *infoLabel;
@property (strong, nonatomic)UIImageView *scanLineImageView;
@property (strong, nonatomic)NSTimer *scanLineTimer;

@end


@implementation QRReaderViewController

- (void)initViewAndSubViews
{
    CGRect mainBoundle = [[UIScreen mainScreen] bounds];
    self.view.frame = mainBoundle;
    
    CGRect readerFrame = self.view.frame;
    CGSize viewFrameSize = CGSizeMake(readerFrame.size.width - 80, readerFrame.size.width - 80);
    /**********************************摄像头开始**********************************/
    // 1 实例化摄像头设备
    AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    // An AVCaptureDevice object abstracts a physical capture device that provides input data (such as audio or video) to an AVCaptureSession object.
    // 2 设置输入,把摄像头作为输入设备
    // 因为模拟器是没有摄像头的，因此在此最好做个判断
    NSError *error = nil;
    AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:device error:&error];
    if(error)
    {
        NSLog(@"没有摄像头: %@", error.localizedDescription);
        return;
    }
    
    // 3 设置输出(Metadata元数据)
    AVCaptureMetadataOutput *output = [[AVCaptureMetadataOutput alloc]init];
    CGRect scanCrop = CGRectMake((readerFrame.size.width - viewFrameSize.width)/2,
                                 (readerFrame.size.height - viewFrameSize.height)/2,
                                 viewFrameSize.width,
                                 viewFrameSize.height);
    
    output.rectOfInterest = CGRectMake(scanCrop.origin.y/readerFrame.size.height,
                                       scanCrop.origin.x/readerFrame.size.width,
                                       scanCrop.size.height/readerFrame.size.height,
                                       scanCrop.size.width/readerFrame.size.width);
    
    // 3.1 设置输出的代理
    // 使用主线程队列，相应比较同步，使用其他队列，相应不同步，容易让用户产生不好的体验。
    [output setMetadataObjectsDelegate:self queue:dispatch_get_main_queue()];
    
    // 4 拍摄会话
    AVCaptureSession *session = [[AVCaptureSession alloc] init];
    session.sessionPreset = AVCaptureSessionPreset640x480;
    //添加session的输入输出
    [session addInput:input];
    [session addOutput:output];
    //4.1设置输出格式
    [output setMetadataObjectTypes:@[AVMetadataObjectTypeQRCode]];
    
    //5 设置预览图层，用户可看的扫描情况
    AVCaptureVideoPreviewLayer *previewLayer = [AVCaptureVideoPreviewLayer layerWithSession:session];
    //5.1设置图层属性
    [previewLayer setVideoGravity:AVLayerVideoGravityResizeAspectFill];
    //5.2图层大小
    [previewLayer setFrame:self.view.bounds];
    //5.3添加到图层
    [self.view.layer addSublayer:previewLayer];
    self.previewLayer = previewLayer;
    self.session = session;
    
    /**********************************摄像头结束**********************************/
    
    
    /* 画一个取景框开始 */
    // 正方形取景框的边长
    CGFloat edgeLenght = 20.0;
    UIImageView *topLeft = [[UIImageView alloc] initWithFrame:CGRectMake((readerFrame.size.width -viewFrameSize.width)/2,
                                                                        (readerFrame.size.height - viewFrameSize.height)/2,
                                                                        edgeLenght,
                                                                        edgeLenght)];
    topLeft.image = [UIImage imageNamed:@"qr_top_left"];
    [self.view addSubview:topLeft];
    
    
    UIImageView *topRight = [[UIImageView alloc] initWithFrame:CGRectMake((readerFrame.size.width + viewFrameSize.width)/2 - edgeLenght,
                                                                          (readerFrame.size.height - viewFrameSize.height)/2,
                                                                          edgeLenght,
                                                                          edgeLenght)];
    topRight.image = [UIImage imageNamed:@"qr_top_right"];
    [self.view addSubview:topRight];
    
    
    UIImageView *bottomLeft = [[UIImageView alloc] initWithFrame:CGRectMake((readerFrame.size.width - viewFrameSize.width)/2,
                                                                            (readerFrame.size.height + viewFrameSize.height)/2 - edgeLenght,
                                                                            edgeLenght,
                                                                            edgeLenght)];
    bottomLeft.image = [UIImage imageNamed:@"qr_bottom_left"];
    [self.view addSubview:bottomLeft];
    
    UIImageView *bottomRight = [[UIImageView alloc] initWithFrame:CGRectMake((readerFrame.size.width + viewFrameSize.width)/2 - edgeLenght,
                                                                             (readerFrame.size.height + viewFrameSize.height)/2 - edgeLenght,
                                                                             edgeLenght,
                                                                             edgeLenght)];
    bottomRight.image = [UIImage imageNamed:@"qr_bottom_right"];
    [self.view addSubview:bottomRight];
    
    
    
    UIView *topLine = [[UIView alloc] initWithFrame:CGRectMake((readerFrame.size.width - viewFrameSize.width)/2-1,
                                                               (readerFrame.size.height - viewFrameSize.height)/2-1,
                                                               viewFrameSize.width+2,
                                                               1)];
    topLine.backgroundColor = [UIColor grayColor];
    [self.view addSubview:topLine];
    
    UIView *bottomLine = [[UIView alloc] initWithFrame:CGRectMake((readerFrame.size.width - viewFrameSize.width)/2-1,
                                                                 (readerFrame.size.height + viewFrameSize.height)/2,
                                                                  viewFrameSize.width+2,
                                                                  1)];
    bottomLine.backgroundColor = [UIColor grayColor];
    [self.view addSubview:bottomLine];
    
    
    UIView *leftLine = [[UIView alloc] initWithFrame:CGRectMake((readerFrame.size.width - viewFrameSize.width)/2-1,
                                                               (readerFrame.size.height - viewFrameSize.height)/2-1,
                                                                1,
                                                                viewFrameSize.height+2)];
    leftLine.backgroundColor = [UIColor grayColor];
    [self.view addSubview:leftLine];
    
    
    UIView *rightLine = [[UIView alloc] initWithFrame:CGRectMake((readerFrame.size.width + viewFrameSize.width)/2,
                                                                 (readerFrame.size.height - viewFrameSize.height)/2-1,
                                                                 1,
                                                                 viewFrameSize.height+2)];
    
    rightLine.backgroundColor = [UIColor grayColor];
    [self.view addSubview:rightLine];
    
    self.scanLineImageView = [[UIImageView alloc] initWithFrame:CGRectMake((readerFrame.size.width - 230)/2,
                                                                           (readerFrame.size.height - viewFrameSize.height)/2,
                                                                           230,
                                                                           10)];
    self.scanLineImageView.image = [UIImage imageNamed:@"qr_scan_line"];
    [self.view addSubview:self.scanLineImageView];
    
    /* 画一个取景框结束 */
    
    /* 配置取景框之外颜色开始 */
    
    //scanCrop
    UIView *viewTopScan = [[UIView alloc] initWithFrame:CGRectMake(0,
                                                                   0,
                                                                   mainBoundle.size.width,
                                                                   scanCrop.origin.y)];
    UIView *viewBottomScan = [[UIView alloc] initWithFrame:CGRectMake(0,
                                                                      scanCrop.origin.y + scanCrop.size.height,
                                                                      mainBoundle.size.width,
                                                                      mainBoundle.size.height - scanCrop.size.height - scanCrop.origin.y)];
    UIView *viewLeftScan = [[UIView alloc] initWithFrame:CGRectMake(0,
                                                                   scanCrop.origin.y,
                                                                    scanCrop.origin.x,
                                                                    scanCrop.size.height)];
    UIView *viewRightScan = [[UIView alloc] initWithFrame:CGRectMake(scanCrop.origin.x+ scanCrop.size.width,
                                                                     scanCrop.origin.y,
                                                                     mainBoundle.size.width-scanCrop.origin.x- scanCrop.size.width,
                                                                     scanCrop.size.height)];
    UIColor *color = [UIColor colorWithRed:1.0 green:1.0 blue:1.0 alpha:0.2];
    viewTopScan.backgroundColor = color;
    viewBottomScan.backgroundColor = color;
    viewLeftScan.backgroundColor = color;
    viewRightScan.backgroundColor = color;
    [self.view addSubview:viewTopScan];
    [self.view addSubview:viewBottomScan];
    [self.view addSubview:viewLeftScan];
    [self.view addSubview:viewRightScan];
    /* 配置取景框之外颜色结束 */
    
    // 返回键
    UIButton *goBackButton = ({
        UIButton *button = [[UIButton alloc] initWithFrame:CGRectMake(20, 30, 36, 36)];
        [button setImage:[UIImage imageNamed:@"qr_vc_left"] forState:UIControlStateNormal];
        button.layer.cornerRadius = 18.0;
        button.layer.backgroundColor = [[UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:0.5] CGColor];
        [button addTarget:self action:@selector(goBack:) forControlEvents:UIControlEventTouchUpInside];
        button;
    });
    [self.view addSubview: goBackButton];
    
    
    // 闪光灯
//    UIButton *torchSwitch = ({
//        UIButton *button = [[UIButton alloc] initWithFrame:CGRectMake(mainBoundle.size.width - 64, 30, 36, 36)];
//        [button setImage:[UIImage imageNamed:@"qr_vc_right"] forState:UIControlStateNormal];
//        button.layer.cornerRadius = 18.0;
//        button.layer.backgroundColor = [[UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:0.5] CGColor];
//        [button addTarget:self action:@selector(torchSwitch:) forControlEvents:UIControlEventTouchUpInside];
//        button;
//    });
//    
//    [self.view addSubview:torchSwitch];
    
    self.infoLabel = [[UILabel alloc]initWithFrame:CGRectMake((readerFrame.size.width- viewFrameSize.width)/2,
                                                             (readerFrame.size.height+viewFrameSize.height)/2+20,
                                                              viewFrameSize.width, 30)];
    self.infoLabel.text = @"将二维码放入取景框中即可自动扫描";
    self.infoLabel.font = [UIFont systemFontOfSize:13.0];
    self.infoLabel.layer.cornerRadius = self.infoLabel.frame.size.height/2;
    self.infoLabel.layer.backgroundColor = [[UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:0.5] CGColor];
    self.infoLabel.textColor = [UIColor whiteColor];
    self.infoLabel.textAlignment = NSTextAlignmentCenter;
    [self.view addSubview:self.infoLabel];
    
    self.view.backgroundColor = [UIColor blackColor];
    
    
    
}



- (void)viewDidLoad
{
    [super viewDidLoad];
}

- (void)viewWillAppear:(BOOL)animated
{
    self.navigationController.navigationBar.hidden = YES;
    [[UIApplication sharedApplication] setStatusBarHidden:YES];
    [super viewWillAppear:animated];
    [self initViewAndSubViews];
    [self.session startRunning];
}


- (void)viewDidAppear:(BOOL)animated
{
    AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    NSError *error = nil;
    AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice: device error:&error];
    if (error)
    {
        NSLog(@"没有摄像头%@", error.localizedDescription);
        input = nil;
        [self.navigationController popViewControllerAnimated:YES];
        return;
    }
    
    if(self.scanLineTimer == nil)
    {
        [self moveUpAndDownLine];
        [self createTimer];
    }
    
    
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    self.navigationController.navigationBar.hidden = NO;
    [[UIApplication sharedApplication] setStatusBarHidden:NO];
    [super viewWillDisappear:animated];
    
}

//返回
- (void)goBack:(id)sender
{    
    [self dismissViewControllerAnimated:YES completion:^{[self.view removeFromSuperview];}];
}

//闪光灯
-(void)torchSwitch:(id)sender
{
    AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
   
    if (device.hasTorch) {
        NSError *error = nil;
        BOOL has  = [device lockForConfiguration:&error];
        if(!has){
            if(error)
            {
                NSLog(@"闪光灯操作失败: %@", error.localizedDescription);
            }
            return;
        }
        
        device.torchMode = device.torchMode == AVCaptureTorchModeOff ? AVCaptureTorchModeOn : AVCaptureTorchModeOff;
        [device unlockForConfiguration];
    }
    
    
}

#define  LINE_SCAN_TIMER 3.0 // 扫描线从上到下扫描所历时间（s）
- (void)createTimer
{
    self.scanLineTimer = [NSTimer scheduledTimerWithTimeInterval:LINE_SCAN_TIMER
                                                          target:self
                                                        selector:@selector(moveUpAndDownLine)
                                                        userInfo:nil
                                                         repeats:YES];
}

// 扫描条上下滚动
-(void)moveUpAndDownLine
{
    CGRect readerFrame = self.view.frame;
    CGSize viewFinderSize = CGSizeMake(self.view.frame.size.width - 80, self.view.frame.size.width - 80);
    CGRect scanLineFrame = self.scanLineImageView.frame;
    scanLineFrame.origin.y = (readerFrame.size.height - viewFinderSize.height)/2;
    self.scanLineImageView.frame = scanLineFrame;
    self.scanLineImageView.hidden = NO;
    __weak __typeof(self) weakSelf = self;
    
    [UIView animateWithDuration:LINE_SCAN_TIMER - 0.05
                     animations:^{
                         CGRect scanLineFrame = weakSelf.scanLineImageView.frame;
                         scanLineFrame.origin.y = (readerFrame.size.height + viewFinderSize.height)/2 - weakSelf.scanLineImageView.frame.size.height;
                         weakSelf.scanLineImageView.frame = scanLineFrame;
                     }
                     completion:^(BOOL finished){
                         weakSelf.scanLineImageView.hidden = YES;
                     }];
    
}

//此方法是在识别到QRCode并且完成转换，如果QRCode的内容越大，转换需要的时间就越长。
#pragma mark AVCaptureMetadataOutputObjectsDelegate
-(void)captureOutput:(AVCaptureOutput *)captureOutput didOutputMetadataObjects:(NSArray *)metadataObjects fromConnection:(AVCaptureConnection *)connection
{
    // 会频繁的扫描，调用代理方法
    // 1如果扫描完成，停止会话
    [self.session stopRunning];
    
    // 2删除预览图层
    [self.previewLayer removeFromSuperlayer];
    //设置界面显示结果
    if(metadataObjects.count > 0)
    {
        AVMetadataMachineReadableCodeObject *obj = metadataObjects[0];
        NSLog(@"扫描结果: %@", obj.stringValue);
        std::string strUrl = [obj.stringValue UTF8String];
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(QR_VIEW_CONTROLLER_RESULT,&strUrl);
    }
    
    [self dismissViewControllerAnimated:YES completion:^{[self.view removeFromSuperview];}];

}

//方向
- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskPortrait;
}


@end