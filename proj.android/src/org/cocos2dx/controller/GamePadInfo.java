package org.cocos2dx.controller;


import android.os.Vibrator;
import android.view.InputDevice;

import com.yunos.tv.exdeviceservice.client.EXDevice;

/****************************
 * 创建时间：2014年3月31日 下午1:53:26 项目名称：Gloud
 * 
 * @author 赵强
 * @version 1.0
 * @since JDK 1.8.0 文件名称：GamePadInfo.java 类说明： 手柄信息
 ****************************/
public class GamePadInfo
{
	public int wButtons; // 手柄上各个按键
	public int L2;
	public int R2;
	public int LX;
	public int LY;
	public int RX;
	public int RY;
	private InputDevice m_Device; // 操控设备
	private EXDevice m_EXDevice;  // 阿里云OS操控设备
	public static final int PC_MODE = 0;
	public static final int XBOX_MODE = 1;
	public static final int ANDROID_MODE = 2;
	public int m_mode;
	public boolean m_hasRXRYRZ; // XBOX_MODE时，部分盒子将右摇杆和R2识别为RX/RY/RZ，L2识别为Z
	public boolean m_hasLTRT; // XBOX_MODE时，部分盒子将L2和R2识别为LT/RT，但是右摇杆是Z/RZ
	public boolean m_brakeOnLeft; // ANDROID_MODE时，AXIS_BRAKE一般在左，但是部分手柄在右
	public boolean m_dropGasBrake; // ANDROID_MODE时，阿修罗TE、大麦手柄的Gas/Brake没有值，还是直接看L2/R2吧
	public BetopBfmEntity m_BetopBfmEntity; // 当手柄为北通Android模式时(BFM)有效
	public static final int XINPUT_USER_MAX = 4;

	public GamePadInfo()
	{
		clear();
	}
	
	public void setDevice(InputDevice dev)
	{
		m_Device = dev;
		m_EXDevice = null;
	}
	
	public void setEXDevice(EXDevice dev)
	{
		m_Device = null;
		m_EXDevice = dev;
	}
	
	public void Vibrate(int vibMilliSeconds, int leftMotorSpeed/*0~65535*/, int rightMotorSpeed/*0~65535*/)
	{
		if (m_Device != null) {
			
			Vibrator v = m_Device.getVibrator();

			if (v != null && v.hasVibrator()) {
				v.cancel();
				v.vibrate(vibMilliSeconds);
			}
		}
		else if (m_EXDevice != null) {
			// YunOS的震动比较弱，所以这里放大了几倍，时间也延长到200ms
			int l_speed = Math.min(255, (int)((float)leftMotorSpeed * 2.5  / 65535 * 255));
			int r_speed = Math.min(255, (int)((float)rightMotorSpeed * 2.5 / 65535 * 255));
			vibMilliSeconds = Math.min(vibMilliSeconds, 200);
			 // speed是振动强度0~255。
			if (l_speed < 10 && r_speed < 10)
				m_EXDevice.setVibrate(0, 0, 0);
			else if (l_speed > 10 && r_speed < 10)
				m_EXDevice.setVibrate(1, l_speed, vibMilliSeconds);
			else if (l_speed < 10 && r_speed > 10)
				m_EXDevice.setVibrate(2, r_speed, vibMilliSeconds);
			else 
				m_EXDevice.setVibrate(3, Math.max(l_speed, r_speed), vibMilliSeconds);
			
//			Log.i("Vibrate", "Vibrate: vibMilliSeconds:"+vibMilliSeconds+" left:"+l_speed+" right:"+r_speed);
		}
	}

	public final boolean isNull()
	{
		return (m_Device == null && m_EXDevice == null);
	}
	public final boolean isYunOSDevice()
	{
		return (m_EXDevice != null);
	}

	public final boolean isTheDeviceID(int tDeviceID)
	{
		if (getDeviceId() == tDeviceID && tDeviceID != -1)
			return true;
		return false;
	}
	
	public final int getDeviceId() {
		if (m_Device != null)
			return m_Device.getId();
		if (m_EXDevice != null)
			return m_EXDevice.getDeviceId();
		return -1;
	}
	public final String getDeviceName() {
		if (m_Device != null)
			return m_Device.getName();
		if (m_EXDevice != null)
			return m_EXDevice.getProductName();
		return "";
	}

	public void clear()
	{
		wButtons = L2 = R2 = LX = LY = RX = RY = 0;
		m_Device = null;
		m_EXDevice = null;
		m_mode = XBOX_MODE;
		m_hasRXRYRZ = false;
		m_hasLTRT = false;
		m_brakeOnLeft = true;
		m_dropGasBrake = false;
		m_BetopBfmEntity = null;
	}

	@Override
	public String toString()
	{
		return "GamePadInfo:wButtons(" + wButtons + "), Mode(" + m_mode + "), LX(" + LX + "), LY(" + LY + "), RX(" + RX + "), RY(" + RY + "), L2(" + L2
				+ "), R2(" + R2 + "), DeviceID(" + getDeviceId() + "), RXRYRZ(" + m_hasRXRYRZ + "), LTRT(" + m_hasLTRT
				+ "), BreakOnLeft(" + m_brakeOnLeft + "), DropGasBrake(" + m_dropGasBrake + "), BetopBfmEntity(" + m_BetopBfmEntity + ")";
	}
}
