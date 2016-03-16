package org.cocos2dx.controller;
import java.io.File;
import java.io.FileFilter;
import java.net.NetworkInterface;
import java.util.Collections;
import java.util.List;
import java.util.Locale;
import java.util.UUID;
import java.util.regex.Pattern;


import com.yunos.tv.exdeviceservice.client.EXDevice;

import android.annotation.TargetApi;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Build;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.util.SparseIntArray;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.InputDevice.MotionRange;

@TargetApi(Build.VERSION_CODES.GINGERBREAD)
public class Utility {

    public static GamePadInfo m_GamePads[] = new GamePadInfo[GamePadInfo.XINPUT_USER_MAX];

    public static SparseIntArray m_GamePadMap = new SparseIntArray();
	public static final int XINPUT_GAMEPAD_DPAD_UP    = 0x0001;
    public static final int XINPUT_GAMEPAD_DPAD_DOWN  = 0x0002;
    public static final int XINPUT_GAMEPAD_DPAD_LEFT  = 0x0004;
    public static final int XINPUT_GAMEPAD_DPAD_RIGHT = 0x0008;
    public static final int XINPUT_GAMEPAD_START      = 0x0010;
    public static final int XINPUT_GAMEPAD_BACK       = 0x0020;
    public static final int XINPUT_GAMEPAD_LEFT_THUMB = 0x0040;
    public static final int XINPUT_GAMEPAD_RIGHT_THUMB= 0x0080;
    public static final int XINPUT_GAMEPAD_LEFT_SHOULDER = 0x0100;
    public static final int XINPUT_GAMEPAD_RIGHT_SHOULDER = 0x0200;
    public static final int XINPUT_GAMEPAD_A = 0x1000;
    public static final int XINPUT_GAMEPAD_B = 0x2000;
    public static final int XINPUT_GAMEPAD_X = 0x4000;
    public static final int XINPUT_GAMEPAD_Y = 0x8000;
    public static final int XINPUT_GAMEPAD_LEFT_TRIGGER  = 0x100000; // 这个值无意义，只是用来表示这个键而已
    public static final int XINPUT_GAMEPAD_RIGHT_TRIGGER = 0x200000; // 这个值无意义，只是用来表示这个键而已
    
static {
//	Log.d("TAG", "MODEL: [" + android.os.Build.MODEL + "] Manu: [" + android.os.Build.MANUFACTURER + "] Product: [" + android.os.Build.PRODUCT + "] ID: [" + android.os.Build.ID + "] Display: [" + android.os.Build.DISPLAY + "] CPUCores: [" + Utility.getNumCores() + "]");
//	Log.d("TAG", "BOARD: [" + android.os.Build.BOARD + "] BOOTLOADER: [" + android.os.Build.BOOTLOADER + "] BRAND: [" + android.os.Build.BRAND + "] DEVICE: [" + android.os.Build.DEVICE + "] HARDWARE: [" + android.os.Build.HARDWARE + "]");
//	Log.d("TAG", "FINGERPRINT: [" + android.os.Build.FINGERPRINT + "] HOST: [" + android.os.Build.HOST + "] TAGS: [" + android.os.Build.TAGS + "] CPU_ABI: [" + android.os.Build.CPU_ABI + "] CPU_ABI2: [" + android.os.Build.CPU_ABI2 + "]");
    
    /* Initialize gamepad variables*/
	for (int i = 0; i < GamePadInfo.XINPUT_USER_MAX; ++i){
		m_GamePads[i] = new GamePadInfo();
	}

	// 上下左右
	m_GamePadMap.append(KeyEvent.KEYCODE_DPAD_UP, XINPUT_GAMEPAD_DPAD_UP);
	m_GamePadMap.append(KeyEvent.KEYCODE_DPAD_DOWN, XINPUT_GAMEPAD_DPAD_DOWN);
	m_GamePadMap.append(KeyEvent.KEYCODE_DPAD_LEFT, XINPUT_GAMEPAD_DPAD_LEFT);
	m_GamePadMap.append(KeyEvent.KEYCODE_DPAD_RIGHT, XINPUT_GAMEPAD_DPAD_RIGHT);
	
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_START, XINPUT_GAMEPAD_START);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_SELECT, XINPUT_GAMEPAD_BACK);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_THUMBL, XINPUT_GAMEPAD_LEFT_THUMB);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_THUMBR, XINPUT_GAMEPAD_RIGHT_THUMB);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_L1, XINPUT_GAMEPAD_LEFT_SHOULDER);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_R1, XINPUT_GAMEPAD_RIGHT_SHOULDER);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_L2, XINPUT_GAMEPAD_LEFT_TRIGGER);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_R2, XINPUT_GAMEPAD_RIGHT_TRIGGER);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_A, XINPUT_GAMEPAD_A);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_B, XINPUT_GAMEPAD_B);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_X, XINPUT_GAMEPAD_X);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_Y, XINPUT_GAMEPAD_Y);
	
	// 北通战戟2  北通神鹰Pro 可编程手柄的按键映射
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_1, XINPUT_GAMEPAD_Y);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_2, XINPUT_GAMEPAD_B);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_3, XINPUT_GAMEPAD_A);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_4, XINPUT_GAMEPAD_X);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_5, XINPUT_GAMEPAD_LEFT_SHOULDER);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_6, XINPUT_GAMEPAD_RIGHT_SHOULDER);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_7, XINPUT_GAMEPAD_LEFT_TRIGGER);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_8, XINPUT_GAMEPAD_RIGHT_TRIGGER);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_9, XINPUT_GAMEPAD_BACK);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_10,XINPUT_GAMEPAD_START);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_11,XINPUT_GAMEPAD_LEFT_THUMB);
	m_GamePadMap.append(KeyEvent.KEYCODE_BUTTON_12,XINPUT_GAMEPAD_RIGHT_THUMB);
}

	//private static String LogTag = "Utility";

	public static int getCorrectKeyCode(KeyEvent event){
		
		int keyCode = event.getKeyCode();
		//int originalKeyCode = keyCode;
		int scanCode = event.getScanCode();
		
		int user_index = Utility.getGamePadUserIndex(event.getDevice());
		if (user_index == -1)
			return keyCode;
		
		GamePadInfo gamepad = Utility.m_GamePads[user_index];
		
		String deviceName = "";
		try {
			if (event.getDevice() != null && event.getDevice().getName() != null)
				deviceName = event.getDevice().getName().toLowerCase(Locale.getDefault());
		} catch (StackOverflowError e) {
		}
		// PS模式（D-A模式）的阿修罗SE/TE手柄、致动某型号手柄、莱仕达龙之血刃、WAMO手柄等等在盒子上识别的键值有误，通过scancode进行纠正。
		if (gamepad.m_mode == GamePadInfo.PC_MODE)
		/*
		if (deviceName.contains("2175") || deviceName.contains("2185") || deviceName.contains("btp-") ||  
				(deviceName.contains("betop") && !deviceName.contains("memsart") && !deviceName.contains("bfm") && !deviceName.contains("2171")) ||  
				deviceName.contains("hjc game") || deviceName.contains("wamo") || 
				deviceName.contains("litestar ls pc controller"))
		*/
		{
			switch(scanCode) {
			case 304:
				keyCode = KeyEvent.KEYCODE_BUTTON_Y; // 误识别为KEYCODE_BUTTON_A
				if (deviceName.contains("litestar")) // 莱仕达手柄
					keyCode = KeyEvent.KEYCODE_BUTTON_X; // 误识别为KEYCODE_BUTTON_A
				break;
			case 305:
				keyCode = KeyEvent.KEYCODE_BUTTON_B;
				if (deviceName.contains("litestar")) // 莱仕达手柄
					keyCode = KeyEvent.KEYCODE_BUTTON_A; // 误识别为KEYCODE_BUTTON_B
				break;
			case 306:
				keyCode = KeyEvent.KEYCODE_BUTTON_A; // 误识别为KEYCODE_BUTTON_C
				if (deviceName.contains("litestar")) // 莱仕达手柄
					keyCode = KeyEvent.KEYCODE_BUTTON_B; // 误识别为KEYCODE_BUTTON_C
				break;
			case 307:
				keyCode = KeyEvent.KEYCODE_BUTTON_X;
				if (deviceName.contains("litestar")) // 莱仕达手柄
					keyCode = KeyEvent.KEYCODE_BUTTON_Y; // 误识别为KEYCODE_BUTTON_X
				break;
			case 308:
				keyCode = KeyEvent.KEYCODE_BUTTON_L1; // 误识别为KEYCODE_BUTTON_Y
				break;
			case 309:
				keyCode = KeyEvent.KEYCODE_BUTTON_R1; // 误识别为KEYCODE_BUTTON_Z
				break;
			case 310:
				keyCode = KeyEvent.KEYCODE_BUTTON_L2; // 误识别为KEYCODE_BUTTON_L1
				break;
			case 311:
				keyCode = KeyEvent.KEYCODE_BUTTON_R2; // 误识别为KEYCODE_BUTTON_R1
				break;
			case 312:
				keyCode = KeyEvent.KEYCODE_BUTTON_SELECT; // 误识别为KEYCODE_BUTTON_L2
				break;
			case 313:
				keyCode = KeyEvent.KEYCODE_BUTTON_START; // 误识别为KEYCODE_BUTTON_R2
				break;
			case 314:
				keyCode = KeyEvent.KEYCODE_BUTTON_THUMBL; // 误识别为KEYCODE_BUTTON_SELECT
				break;
			case 315:
				keyCode = KeyEvent.KEYCODE_BUTTON_THUMBR; // 误识别为KEYCODE_BUTTON_START
				break;
			}
		}
		
		// 2014.06.26。更新Rom之后的天猫魔盒无法正确识别北通阿修罗的X模式下的，L2、R2、THUMBL、THUMBR
		// 2014.10.29。天猫魔盒2/天猫魔盒1s识别X模式的手柄按键也很乱，仍然缺少THUMBL、THUMBR。
		// 但是THUMBL、THUMBR按下时有如下日志，说明还是可以通过某种方法识别按键。可以找找天猫魔盒的文档。
		// D/TVSensorDevice(2981): TVSensorDevice, onDKey:mData:mSize=1, mKeyCodes[0]=106, mActions[0]=0 , REMOTECONTROL_INPUT:true
		// D/TVSensorDevice(2981): TVSensorDevice, onDKey:mData:mSize=1, mKeyCodes[0]=106, mActions[0]=1 , REMOTECONTROL_INPUT:true

		
		
		//原有莱仕达|| deviceName.contains("alitv-vgamepad") 用该方法会错误修改按键，所以去掉 ,天猫升级ROM后以前问题已修改
		if (deviceName.contains("alitv-vkeyboard") ) {
			switch(scanCode) {
			case 304:
				keyCode = KeyEvent.KEYCODE_BUTTON_Y; // 误识别为KEYCODE_BUTTON_A
				break;
			case 28:
				keyCode = KeyEvent.KEYCODE_BUTTON_A; // 误识别为DPAD_CENTER
				break;
			case 1:
				keyCode = KeyEvent.KEYCODE_BUTTON_B; // 误识别为BACK
				break;
			case 307:
				keyCode = KeyEvent.KEYCODE_BUTTON_X;
				break;
			case 312:
				keyCode = KeyEvent.KEYCODE_BUTTON_START; // 误识别为KEYCODE_BUTTON_L2
				break;
			case 313:
				keyCode = KeyEvent.KEYCODE_BUTTON_SELECT; // 误识别为KEYCODE_BUTTON_R2
				break;
			case 308:
				keyCode = KeyEvent.KEYCODE_BUTTON_L1; // 误识别为KEYCODE_BUTTON_Y
				break;
			case 309:
				keyCode = KeyEvent.KEYCODE_BUTTON_R1; // 误识别为KEYCODE_BUTTON_Z
				break;
			case 310:
				keyCode = 0; // 无意义，有Brake值
				break;
			case 311:
				keyCode = 0; // 无意义，有GAS值
				break;
			}
		}
		
		//shield back键
		if (scanCode == 158 && deviceName.contains("gpio-keys")) {
			keyCode = KeyEvent.KEYCODE_BUTTON_SELECT;
		}
		
		// 小米盒子增强版上 XBOX模式手柄的BACK键
		if ((event.getSource() & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD && keyCode == KeyEvent.KEYCODE_BACK) {
			keyCode = KeyEvent.KEYCODE_BUTTON_SELECT;
		}
		
		/*
        // 对于不支持北通战戟2的烽火安卓4.0，其keycode为0时，可以根据scancode来判断是哪个键
        if (keyCode == 0 && scanCode >= 288 && scanCode <= 299
        		 && ((event.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) != 0 || deviceName.contains("betop"))) // 判断是游戏手柄或者包含北通字样
        	keyCode = KeyEvent.KEYCODE_BUTTON_1 + (scanCode - 288);

    	if (scanCode >= 59 && scanCode <= 68) // F1~F10
    		keyCode = KeyEvent.KEYCODE_F1 + (scanCode - 59);
    	else if (scanCode == 87 || scanCode == 88) // F11~F12
    		keyCode = KeyEvent.KEYCODE_F11 + (scanCode - 87);
        
    	// 对于安卓未能支持键盘上的部分键，其keycode为0时，可以根据scancode来判断是哪个键
        if (keyCode == 0 && scanCode != 0 && (event.getSource() & InputDevice.SOURCE_KEYBOARD) != 0)
        {
        	switch (keyCode) {
        	case 1: // Escape
        		keyCode = KeyEvent.KEYCODE_ESCAPE;
        		break;
        	case 14: // Backspace
        		keyCode = KeyEvent.KEYCODE_FORWARD_DEL;
        		break;
        	case 58: // Caps Lock
        		keyCode = KeyEvent.KEYCODE_CAPS_LOCK;
        		break;
        	case 99: // PrtScr/SysReq
        		keyCode = KeyEvent.KEYCODE_SYSRQ;
        		break;
        	case 70: // Scroll Lock
        		keyCode = KeyEvent.KEYCODE_SCROLL_LOCK;
        		break;
        	case 119: // Pause/Break
        		keyCode = KeyEvent.KEYCODE_BREAK;
        		break;
        	case 110: // Insert
        		keyCode = KeyEvent.KEYCODE_INSERT;
        		break;
        	case 111: // Delete
        		keyCode = KeyEvent.KEYCODE_DEL;
        		break;
        	case 29: // Left Ctrl
        		keyCode = KeyEvent.KEYCODE_CTRL_LEFT;
        		break;
        	case 97: // Right Ctrl
        		keyCode = KeyEvent.KEYCODE_CTRL_RIGHT;
        		break;
        	case 125: // Left Win
        		// TODO:
        		break;
        	case 126: // Right Win
        		// TODO:
        		break;
        	case 127: // Menu
        		// TODO:
        		break;
        	case 69: // Number Lock
        		keyCode = KeyEvent.KEYCODE_NUM_LOCK;
        		break;
        	case 98: // 小键盘的/
        		keyCode = KeyEvent.KEYCODE_NUMPAD_DIVIDE;
        		break;
        	case 55: // 小键盘的*
        		keyCode = KeyEvent.KEYCODE_NUMPAD_MULTIPLY;
        		break;
        	case 74: // 小键盘的-
        		keyCode = KeyEvent.KEYCODE_NUMPAD_SUBTRACT;
        		break;
        	case 78: // 小键盘的+
        		keyCode = KeyEvent.KEYCODE_NUMPAD_ADD;
        		break;
        	case 96: // 小键盘的Enter
        		keyCode = KeyEvent.KEYCODE_NUMPAD_ENTER;
        		break;
        	case 83: // 小键盘的.
        		keyCode = KeyEvent.KEYCODE_NUMPAD_DOT;
        		break;
        	case 82: // 小键盘的0
        		keyCode = KeyEvent.KEYCODE_NUMPAD_0;
        		break;
        	case 79: // 小键盘的1
        		keyCode = KeyEvent.KEYCODE_NUMPAD_1;
        		break;
        	case 80: // 小键盘的2
        		keyCode = KeyEvent.KEYCODE_NUMPAD_2;
        		break;
        	case 81: // 小键盘的3
        		keyCode = KeyEvent.KEYCODE_NUMPAD_3;
        		break;
        	case 75: // 小键盘的4
        		keyCode = KeyEvent.KEYCODE_NUMPAD_4;
        		break;
        	case 76: // 小键盘的5
        		keyCode = KeyEvent.KEYCODE_NUMPAD_5;
        		break;
        	case 77: // 小键盘的6
        		keyCode = KeyEvent.KEYCODE_NUMPAD_6;
        		break;
        	case 71: // 小键盘的7
        		keyCode = KeyEvent.KEYCODE_NUMPAD_7;
        		break;
        	case 72: // 小键盘的8
        		keyCode = KeyEvent.KEYCODE_NUMPAD_8;
        		break;
        	case 73: // 小键盘的9
        		keyCode = KeyEvent.KEYCODE_NUMPAD_9;
        		break;
        	}
        }*/
        
//        if (keyCode == 0){
//        	Log.e(LogTag, "unknown key event. scancode:" + scanCode);
//        }
        

//    	Log.i(LogTag, "getKeyCodeFromScanCode. original keyCode:" + originalKeyCode + " scanCode:" + scanCode + " corrected keyCode:" + keyCode + " source:" + event.getSource());
        return keyCode;
	}

	@TargetApi(16)
    public static int getGamePadUserIndex(InputDevice device){
		if (device == null)
		{
			return -1;
		}
		// 对于非手柄设备，如键盘、鼠标、触屏等，均认为是来自第一个用户的操作；只支持多个手柄。
		if ((device.getSources() & InputDevice.SOURCE_CLASS_JOYSTICK) == 0) {
//        	Log.i("getGamePadUserIndex", "not joystick. treat as user 0.");
			return 0;
		}
		String deviceName = "";
		try {
			if (device != null && device.getName() != null)
				deviceName = device.getName().toLowerCase(Locale.getDefault());
		} catch (StackOverflowError e) {
		}

		// 支持XBOX模式的手柄
		// XBOX官方手柄		有线				Microsoft X-Box 360 pad
		// 北通阿修罗TE		无线				Microsoft X-Box 360 pad				P、X灯亮
		// 北通阿修罗SE		无线				Microsoft X-Box 360 pad				P、X灯亮
		// 北通神鹰XPro		有线				Microsoft X-Box 360 pad
		// 北通斯巴达SE		有线				Microsoft X-Box 360 pad
		// 莱仕达剑神		有线				Microsoft X-Box 360 pad
		// WAMO手柄			无线				Microsoft X-Box 360 pad				x灯亮

		// PC模式
		// 北通阿修罗TE		无线				HJC Game [ BETOP CONTROLLER ]		P、D-A灯亮
		// 北通阿修罗SE		有线				HJC Game [ BETOP CONTROLLER ]		P、D-A灯亮
		// 北通神鹰Pro		有线  			betop controller
		// 北通神鹰XPro		有线  			ShanWan BETOP CONTROLLER
		// 北通斯巴达SE		有线				ShanWan BETOP CONTROLLER
		// 北通蝙蝠BTP2126	有线				betop controller
		// 北通战戟2			有线				betop controller
		// 金拓达(IMECOO)	无线				ShanWan USB WirelessGamepad
		// 莱仕达剑神		有线				PC Game Controller
		// 莱仕达龙之血刃	有线				LiteStar LS PC Controller
		// 致动某型号手柄	有线				HJC Came ZO-H
		// 大麦手柄(黑色)	无线				Memsart Game [ BETOP CONTROLLER ]
		// WAMO手柄			无线				wamo								d灯亮
		
		// 安卓模式
		// 北通智游者卡洛	无线				BETOP 2171S BFM						下半部分灯亮
		// WAMO手柄			无线				wamo								a灯亮
		// 北通阿修罗TE		无线				HJC Game [ BETOP CONTROLLER ]		D-A、D-D灯亮
		// 北通阿修罗SE		无线				HJC Game [ BETOP CONTROLLER ]		D-A、D-D灯亮
		// NVSHIELD			N/A				NVIDIA Corporation NVIDIA Controller v01.01
		// 小米手柄			蓝牙				Broadcom Bluetooth HID
		
		// NV SHIELD怪异的地方在于，它的BACK键的device name是gpio-keys，和其他键都不一样
		// 为了避免把NV的输入键值处理成两个设备，这里写死把Back键属于第一个设备。
		if (deviceName.contains("gpio-keys")) {	  //shield back键
			return 0;
		}
		// 停止处理阿里手柄的消息，使用YunOS的SDK来处理
		//if (deviceName.contains("alitv-vkeyboard") || deviceName.contains("alitv-vgamepad"))
		//	return -1;
    	int user_index = -1;
    	int null_index = -1;
    	// 是否已经记录在案的手柄
    	for (int i = 0; i < GamePadInfo.XINPUT_USER_MAX; ++i){
        	//Log.w("getGamePadUserIndex", "index:" + i + " device: " + m_GamePads[i].getDeviceId() + " YunOS: " + m_GamePads[i].isYunOSDevice());
    		if (m_GamePads[i].isTheDeviceID(device.getId())){ // 之前发现过的设备（普通手柄或移动手柄），继续使用
    			user_index = i;
    			break;
    		}
    		if (m_GamePads[i].isNull() && null_index == -1) // 找到第一个为null的，如果没找到对应的设备就把新设备放到这里。
    			null_index = i;
    	}
    	//Log.i("getGamePadUserIndex", "null_index:" + null_index + " user_index:" + user_index);
    	if (user_index == -1 && null_index != -1){
            // Cache the most recently obtained device information.
            // The device information may change over time but it can be
            // somewhat expensive to query.
    		if (m_GamePads[null_index].isNull() && device != null){
    			m_GamePads[null_index].setDevice(device);
				int rxryrz_count = 0;
				int ltrt_count = 0;
				int gas_break_throttle_count = 0;
	            final List<MotionRange> ranges = device.getMotionRanges();
	            for (MotionRange range : ranges) {
	                if ((range.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) != 0) {
	                    if (range.getAxis() == MotionEvent.AXIS_RX || 
	                    		range.getAxis() == MotionEvent.AXIS_RY || 
	                    		range.getAxis() == MotionEvent.AXIS_RZ)
	                    	rxryrz_count++;
	                    if (range.getAxis() == MotionEvent.AXIS_LTRIGGER || 
	                    		range.getAxis() == MotionEvent.AXIS_RTRIGGER)
	                    	ltrt_count++;
	                    if (range.getAxis() == MotionEvent.AXIS_BRAKE || 
	                    		range.getAxis() == MotionEvent.AXIS_GAS ||
	                    		range.getAxis() == MotionEvent.AXIS_THROTTLE)
//	                    	||range.getAxis()==MotionEvent.AXIS_Z
	                    	//MotionEvent.AXIS_Z 识别其他android 手柄
	                    {
	                    	gas_break_throttle_count++;
	                    }
	                    	
	                }
	            }
    			if (deviceName.contains("x-box")) {
    				// XBOX模式
    				m_GamePads[null_index].m_mode = GamePadInfo.XBOX_MODE;
    				m_GamePads[null_index].m_brakeOnLeft = true;
					m_GamePads[null_index].m_dropGasBrake = false;
    				m_GamePads[null_index].m_hasRXRYRZ = false;
    				m_GamePads[null_index].m_hasLTRT = false;
    	            if (rxryrz_count == 3)
        				m_GamePads[null_index].m_hasRXRYRZ = true;
    	            if (ltrt_count == 2)
        				m_GamePads[null_index].m_hasLTRT = true;
    			}
    			else if (gas_break_throttle_count >= 1 || deviceName.contains("flydigi motionelf x9")
    					|| deviceName.contains("broadcom bluetooth hid")||deviceName.contains("bluetooth")||rxryrz_count == 3)
    			{
    				//rxryrz_count == 3 NOKIA平板上使用FUNBOX手柄时会误认为是PC模式 ，名字又是gamepad 所以只能是用rxryrz_count去识别
    				// 安卓模式
    				m_GamePads[null_index].m_mode = GamePadInfo.ANDROID_MODE;
    				m_GamePads[null_index].m_hasRXRYRZ = false;
    				m_GamePads[null_index].m_hasLTRT = false;
    				m_GamePads[null_index].m_brakeOnLeft = true;
					m_GamePads[null_index].m_dropGasBrake = false;
					if (gas_break_throttle_count == 0)
						m_GamePads[null_index].m_dropGasBrake = true;
					else if (deviceName.contains("wamo"))
    					m_GamePads[null_index].m_brakeOnLeft = false;
					else if (deviceName.contains("betop"))
    					m_GamePads[null_index].m_dropGasBrake = true;
    			}
    			else {
    				
    				// PC模式
    				m_GamePads[null_index].m_mode = GamePadInfo.PC_MODE;
    				m_GamePads[null_index].m_hasRXRYRZ = false;
    				m_GamePads[null_index].m_hasLTRT = false;
    				m_GamePads[null_index].m_brakeOnLeft = true;
					m_GamePads[null_index].m_dropGasBrake = false;
    			}
				// 发现了一个新设备，看看它是不是北通安卓手柄，如果是那么就可以支持双震动了,北通安卓手柄固定为VID为0x20BC,PID为0x5500
    			// 由于需要申请权限导致非常有可能从游戏画面退出,所以暂时去除.
				/*if (device.getName().toUpperCase().equals("BETOP 2171S BFM"))
				{
					UsbDevice usbDevice = InputUtils.Instance().RequestUSB_Permission(device, 0x20BC, 0x5500);
					if (usbDevice != null)
					{
						m_GamePads[null_index].m_BetopBfmEntity = new BetopBfmEntity(usbDevice, null_index);
					}
				}*/
				user_index = null_index;
			}
    	}
    	
//    	if (user_index >= 0 && user_index < GamePadInfo.XINPUT_USER_MAX)
//    		Log.i("getGamePadUserIndex", "device:" + device.getId() + " device:" + device.getName() + 
//    			" null_index:" + null_index + " user_index:" + user_index + " mode:" + m_GamePads[user_index].m_mode
//    			 + " rxryrz:" + m_GamePads[user_index].m_hasRXRYRZ+ " ltrt:" + m_GamePads[user_index].m_hasLTRT);
    	return user_index;
    }

    public static int getYunOSGamePadUserIndex(EXDevice device){
    	int user_index = -1;
    	int null_index = -1;
    	// 是否已经记录在案的手柄
    	for (int i = 0; i < GamePadInfo.XINPUT_USER_MAX; ++i){
        	//Log.w("getYunOSGamePadUserIndex", "index:" + i + " device: " + m_GamePads[i].getDeviceId() + " YunOS: " + m_GamePads[i].isYunOSDevice());
    		if (m_GamePads[i].isTheDeviceID(device.getDeviceId())){ // 之前发现过的设备（普通手柄或移动手柄），继续使用
    			user_index = i;
    			break;
    		}
    		if (m_GamePads[i].isNull() && null_index == -1) // 找到第一个为null且非移动手柄的，如果没找到对应的设备就把新设备放到这里。
    			null_index = i;
    	}
    	//Log.i("getYunOSGamePadUserIndex", "null_index:" + null_index + " user_index:" + user_index);
    	if (user_index == -1 && null_index != -1){
            // Cache the most recently obtained device information.
            // The device information may change over time but it can be
            // somewhat expensive to query.
    		if (m_GamePads[null_index].isNull()){
    			m_GamePads[null_index].setEXDevice(device);
				// 安卓模式
				m_GamePads[null_index].m_mode = GamePadInfo.ANDROID_MODE;
				m_GamePads[null_index].m_hasRXRYRZ = false;
				m_GamePads[null_index].m_hasLTRT = false;
				m_GamePads[null_index].m_brakeOnLeft = true;
				m_GamePads[null_index].m_dropGasBrake = false;
				
				user_index = null_index;
			}
    	}
    	
//    	if (user_index >= 0 && user_index < GamePadInfo.XINPUT_USER_MAX)
//    		Log.i("getYunOSGamePadUserIndex", "device:" + device.getDeviceId() + 
//    			" null_index:" + null_index + " user_index:" + user_index + " mode:" + m_GamePads[user_index].m_mode
//    			 + " rxryrz:" + m_GamePads[user_index].m_hasRXRYRZ+ " ltrt:" + m_GamePads[user_index].m_hasLTRT);
    	return user_index;
    }

	public static String printGamePads()
	{
		String tResult = "GamePads Information\n";
		for (int i = 0; i < GamePadInfo.XINPUT_USER_MAX; ++i)
		{
			tResult += "{" + i + ", " + m_GamePads[i].toString() + "}\n";
		}
		return tResult;
	}
    /**
     * 产生一个绑定机器的UUID, 目标是机器不换则UUID不变.
     * @return  a unique user id for this instance.
     */
    public static String GenerateUUID(Context ctx)
    {
		//Log.i(AppName, "ip v4:" + getIPAddress(true) + "  ip v6:" + getIPAddress(false));
		//Log.i(AppName, "wlan0:" + getMACAddress("wlan0") + "  eth0:" + getMACAddress("eth0"));

    	try {
	        final TelephonyManager tm = (TelephonyManager)ctx.getSystemService(Context.TELEPHONY_SERVICE);
	
	        final String tmDevice, tmSerial, androidId;
	        tmDevice = "" + tm.getDeviceId();
	        tmSerial = "" + tm.getSimSerialNumber();
	        androidId = "" + android.provider.Settings.Secure.getString(ctx.getContentResolver(), android.provider.Settings.Secure.ANDROID_ID);
	        Log.i("ZQ", "___deviceId:" + tmDevice + " SimSerialNumber:" + tmSerial +  " androidId:" + androidId);
	        
	        // 如果androidid为空或者长度不足，则使用随机生成的ID
	        if (androidId.isEmpty() || androidId.length() < 10)
	        	return UniqueRandomID(ctx);
	
	        // 使用设备ID，SIM卡号，设备号生成的UUID
	        UUID deviceUuid = new UUID(androidId.hashCode(), ((long)tmDevice.hashCode() << 32) | tmSerial.hashCode());
	        String deviceId = deviceUuid.toString();
	        return deviceId;
    	}
    	catch (Exception e) {
    		e.printStackTrace();
        	return UniqueRandomID(ctx);
    	}

        // 使用无线和有线的MAC地址作为UUID的一部分
        //return "Gloud_" + getMACAddress("wlan0").hashCode() + "_" + getMACAddress("eth0").hashCode();
    }
    
	/**
	 * Returns MAC address of the given interface name.
	 * 
	 * @param interfaceName
	 *            eth0, wlan0 or NULL=use first interface
	 * @return mac address or empty string
	 */
	public static String getMACAddress(String interfaceName) {
		try {
			List<NetworkInterface> interfaces = Collections
					.list(NetworkInterface.getNetworkInterfaces());
			for (NetworkInterface intf : interfaces) {
				if (interfaceName != null) {
					if (!intf.getName().equalsIgnoreCase(interfaceName))
						continue;
				}
				byte[] mac = intf.getHardwareAddress();
				if (mac == null)
					return "";
				StringBuilder buf = new StringBuilder();
				for (int idx = 0; idx < mac.length; idx++)
					buf.append(String.format("%02X:", mac[idx]));
				if (buf.length() > 0)
					buf.deleteCharAt(buf.length() - 1);
				return buf.toString();
			}
		} catch (Exception ex) {
		} // for now eat exceptions
		return "";
		/*
		 * try { // this is so Linux hack return
		 * loadFileAsString("/sys/class/net/" +interfaceName +
		 * "/address").toUpperCase().trim(); } catch (IOException ex) { return
		 * null; }
		 */
	}	
	
	private static String uniqueID = null;
	private static final String PREF_UNIQUE_ID = "PREF_UNIQUE_ID";

	public synchronized static String UniqueRandomID(Context context) {
	    if (uniqueID == null) {
	        SharedPreferences sharedPrefs = context.getSharedPreferences(
	                PREF_UNIQUE_ID, Context.MODE_PRIVATE);
	        uniqueID = sharedPrefs.getString(PREF_UNIQUE_ID, null);
	        if (uniqueID == null) {
	            uniqueID = UUID.randomUUID().toString();
	            Editor editor = sharedPrefs.edit();
	            editor.putString(PREF_UNIQUE_ID, uniqueID);
	            editor.commit();
	        }
	    }
	    return uniqueID;
	}
	/**
	 * Gets the number of cores available in this device, across all processors.
	 * Requires: Ability to peruse the filesystem at "/sys/devices/system/cpu"
	 * @return The number of cores, or 1 if failed to get result
	 */
	public static int getNumCores() {
	    //Private Class to display only CPU devices in the directory listing
	    class CpuFilter implements FileFilter {
	        @Override
	        public boolean accept(File pathname) {
	            //Check if filename is "cpu", followed by a single digit number
	            if(Pattern.matches("cpu[0-9]+", pathname.getName())) {
	                return true;
	            }
	            return false;
	        }      
	    }

	    try {
	        //Get directory containing CPU info
	        File dir = new File("/sys/devices/system/cpu/");
	        //Filter to only list the devices we care about
	        File[] files = dir.listFiles(new CpuFilter());
	        //Return the number of cores (virtual CPU devices)
	        return files.length;
	    } catch(Exception e) {
	        //Default to return 1 core
	        return 1;
	    }
	}
}