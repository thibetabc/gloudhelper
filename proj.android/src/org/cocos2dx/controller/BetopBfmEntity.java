package org.cocos2dx.controller;

import java.util.ArrayList;
import java.util.Arrays;

import org.cocos2dx.cpp.AppActivity;
import org.cocos2dx.lib.GameControllerAdapter;
import org.cocos2dx.lib.GameControllerDelegate;

import android.app.Instrumentation;
import android.content.Context;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.os.SystemClock;
import android.view.InputDevice;
import android.view.KeyEvent;

// 针对北通安卓手柄
public final class BetopBfmEntity
{
	private UsbDevice _UsbDevice = null;
	private UsbDeviceConnection _UsbConnect = null;
	private int user_index = -1;
	private int _virtualDeviceID = -12345;
	private AppActivity _GameActivity = null;
	private byte VibrationBuffer[] = new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	private final BFMJoystick _BFMJoystick = new BFMJoystick();

	public BetopBfmEntity(UsbDevice usbDevice, int t_user_index)
	{
		_UsbDevice = usbDevice;
		user_index = t_user_index;
		_virtualDeviceID += user_index;
	}

	@Override
	public String toString()
	{
		return "user_index:" + user_index + ", Connected:" + Connected() + ", VirtualID:" + _virtualDeviceID + ", UsbDevice:" + _UsbDevice;
	}

	public final boolean Connected()
	{
		return _UsbConnect != null;
	}

	public final boolean isThisUsbDevice(UsbDevice usbDevice)
	{
		return _UsbDevice.getDeviceId() == usbDevice.getDeviceId();
	}

	// 接管北通安卓手柄
	public final boolean TakeOver(AppActivity tGameActivity)
	{
		_GameActivity = tGameActivity;
		if (_UsbDevice != null)
		{
			synchronized (this)
			{
				if (_UsbConnect == null)
				{
					UsbManager um = (UsbManager) _GameActivity.getSystemService(Context.USB_SERVICE);
					if (um.hasPermission(_UsbDevice))
					{
						_UsbConnect = um.openDevice(_UsbDevice);
						if (_UsbConnect != null)
						{
							final UsbInterface inf = _UsbDevice.getInterface(0);
							if (_UsbConnect.claimInterface(inf, true))
							{
//								Log.d("BetopBfmEntity", "Claim interface success");
								new Thread()
								{
									@Override
									public void run()
									{
										final byte zeroBuffer[] = new byte[21];
										Arrays.fill(zeroBuffer, (byte) 0);
										byte buffer[] = new byte[21];
										while (_UsbConnect != null)
										{
											Arrays.fill(buffer, (byte) 0);
											_UsbConnect.bulkTransfer(inf.getEndpoint(1), buffer, buffer.length, 1000);
											if (Arrays.equals(zeroBuffer, buffer))
											{
												// 如果为全0,则说明USB连接已经异常断开
//												Log.d("BetopBfmEntity", "Betop Bfm Device is disconnected(removed from system)");
												Utility.m_GamePads[user_index].clear();
												break;
											}
											_BFMJoystick.judge(buffer);
											try
											{
												Thread.sleep(50);
											} catch (InterruptedException e)
											{
												e.printStackTrace();
											}
										}
//										Log.d("BetopBfmEntity", "Betop Bfm read thead exited");
									}
								}.start();
							} else{
//								Log.e("BetopBfmEntity", "Claim interface failure");
							}
						} else{
//							Log.e("BetopBfmEntity", "Connect BetopBfmEntity failure");
						}
					}
					else
					{
//						Log.e("BetopBfmEntity", "Not has the permission");
					}
				}
			}
		}
		return false;
	}

	public final void Vibration(byte leftIntensity, byte rightIntensity)
	{
		synchronized (this)
		{
			if (_UsbConnect != null)
			{
//				Log.d("BetopBfmEntity", "Vibration " + getUByte(leftIntensity) + " : " + getUByte(rightIntensity));
				VibrationBuffer[2] = leftIntensity;
				VibrationBuffer[3] = rightIntensity;
				_UsbConnect.bulkTransfer(_UsbDevice.getInterface(0).getEndpoint(0), VibrationBuffer, VibrationBuffer.length, 1000);
			}
		}
	}

	// 释放手柄控制
	public final void Release()
	{
		synchronized (this)
		{
			if (_UsbConnect != null)
			{
				_UsbConnect.releaseInterface(_UsbDevice.getInterface(0));
				_UsbConnect.close();
				_UsbConnect = null;
//				Log.d("BetopBfmEntity", "Release interface and close the usb connection");
				Utility.m_GamePads[user_index].clear();
			}
		}
	}

	private final int getUByte(byte value)
	{
		int tmp = value;
		if (tmp < 0)
			tmp += 256;
		return tmp;
	}

	public static final String hex_print(byte[] val, String split)
	{
		if (val == null)
			return "-";
		final char[] sHEX = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
		String tResult = "";
		for (int index = 0; index < val.length; ++index)
		{
			tResult += sHEX[(val[index] >> 4) & 0xF];
			tResult += sHEX[val[index] & 0xF];
			if (index < val.length - 1)
				tResult += split;
		}
		return tResult;
	}

	private interface IBFMKeyMotion
	{
		void judge(byte[] buffer);
	}

	class BFMJoystick
	{
		private final class KeyMock
		{
			private int _KeyCode = 0x00;
			private boolean _Turbo = true;
			private boolean _Downed = false;
			Instrumentation _Inst = new Instrumentation();

			KeyMock(int keyCode, boolean turbo)
			{
				_KeyCode = keyCode;
				_Turbo = turbo;
			}

			public void MockDown()
			{
				if (_Turbo || !_Downed)
				{
					_Downed = true;
//					Log.d("BFMJoystick", "KeyMock Down Begin: " + KeyEvent.keyCodeToString(_KeyCode) + "(" + _KeyCode + ")");
					_Inst.sendKeySync(new KeyEvent(SystemClock.uptimeMillis(), SystemClock.uptimeMillis(), KeyEvent.ACTION_DOWN, _KeyCode, 0, 0,
							_virtualDeviceID, 0, 0, InputDevice.SOURCE_CLASS_JOYSTICK));
//					Log.d("BFMJoystick", "KeyMock Down End: " + KeyEvent.keyCodeToString(_KeyCode) + "(" + _KeyCode + ")");
				}
			}

			public void MockUp()
			{
				if (_Downed)
				{
					_Downed = false;
//					Log.d("BFMJoystick", "KeyMock Up Begin: " + KeyEvent.keyCodeToString(_KeyCode) + "(" + _KeyCode + ")");
					_Inst.sendKeySync(new KeyEvent(SystemClock.uptimeMillis(), SystemClock.uptimeMillis(), KeyEvent.ACTION_UP, _KeyCode, 0, 0,
							_virtualDeviceID, 0, 0, InputDevice.SOURCE_CLASS_JOYSTICK));
//					Log.d("BFMJoystick", "KeyMock Up End: " + KeyEvent.keyCodeToString(_KeyCode) + "(" + _KeyCode + ")");
				}
			}
		}

		private class BFM_KeyMap implements IBFMKeyMotion
		{
			private int _Index = 0x00;
			private int _Mask = 0x00;
			private KeyMock _KeyMock = null;

			BFM_KeyMap(int index, int mask, int keyCode, int dsyKeyCode)
			{
				_Index = index;
				_Mask = mask;
				_KeyMock = new KeyMock(keyCode, true);
			}

			@Override
			public void judge(byte[] buffer)
			{
				if ((buffer[_Index] & _Mask) != 0)
				{
					_KeyMock.MockDown();
				} else
					_KeyMock.MockUp();
			}
		}

		private class BFM_DPadMap implements IBFMKeyMotion
		{
			private int[] _Mask = null;
			private KeyMock _KeyMock = null;

			BFM_DPadMap(int[] mask, int keyCode, int dsyKeyCode)
			{
				_Mask = mask;
				_KeyMock = new KeyMock(keyCode, false);
			}

			@Override
			public void judge(byte[] buffer)
			{
				int tmp = buffer[2] & 0x0F;
				for (int mask : _Mask)
				{
					if (tmp == mask)
					{
						_KeyMock.MockDown();
						return;
					}
				}
				_KeyMock.MockUp();
			}
		}

		private class BFM_LeftStickKeyMap implements IBFMKeyMotion
		{
			private KeyMock _KeyMock_U = new KeyMock(KeyEvent.KEYCODE_DPAD_UP, false);
			private KeyMock _KeyMock_D = new KeyMock(KeyEvent.KEYCODE_DPAD_DOWN, false);
			private KeyMock _KeyMock_L = new KeyMock(KeyEvent.KEYCODE_DPAD_LEFT, false);
			private KeyMock _KeyMock_R = new KeyMock(KeyEvent.KEYCODE_DPAD_RIGHT, false);

			@Override
			public void judge(byte[] buffer)
			{
				judge_impl(getUByte(buffer[3]), _KeyMock_L, _KeyMock_R);
				judge_impl(getUByte(buffer[4]), _KeyMock_U, _KeyMock_D);
			}

			private void judge_impl(int value, KeyMock negKeyMock, KeyMock posKeyMock)
			{
				value -= 0x80;
				if (value < 0)
				{
					if (value < -0x40)
						negKeyMock.MockDown();
					else
						negKeyMock.MockUp();
				} else if (value > 0)
				{
					if (value > 0x40)
						posKeyMock.MockDown();
					else
						posKeyMock.MockUp();
				} else
				{
					negKeyMock.MockUp();
					posKeyMock.MockUp();
				}
			}
		}

		private ArrayList<IBFMKeyMotion> _KeyMaps = new ArrayList<IBFMKeyMotion>();
		{
			_KeyMaps.add(new BFM_KeyMap(0, 0x01, KeyEvent.KEYCODE_BUTTON_A, Utility.XINPUT_GAMEPAD_A));
			_KeyMaps.add(new BFM_KeyMap(0, 0x02, KeyEvent.KEYCODE_BUTTON_B, Utility.XINPUT_GAMEPAD_B));
			_KeyMaps.add(new BFM_KeyMap(0, 0x08, KeyEvent.KEYCODE_BUTTON_X, Utility.XINPUT_GAMEPAD_X));
			_KeyMaps.add(new BFM_KeyMap(0, 0x10, KeyEvent.KEYCODE_BUTTON_Y, Utility.XINPUT_GAMEPAD_Y));
			_KeyMaps.add(new BFM_KeyMap(0, 0x40, KeyEvent.KEYCODE_BUTTON_L1, Utility.XINPUT_GAMEPAD_LEFT_SHOULDER));
			_KeyMaps.add(new BFM_KeyMap(0, 0x80, KeyEvent.KEYCODE_BUTTON_R1, Utility.XINPUT_GAMEPAD_RIGHT_SHOULDER));
			_KeyMaps.add(new BFM_KeyMap(1, 0x01, KeyEvent.KEYCODE_BUTTON_L2, Utility.XINPUT_GAMEPAD_LEFT_TRIGGER));
			_KeyMaps.add(new BFM_KeyMap(1, 0x02, KeyEvent.KEYCODE_BUTTON_R2, Utility.XINPUT_GAMEPAD_RIGHT_TRIGGER));
			_KeyMaps.add(new BFM_KeyMap(1, 0x04, KeyEvent.KEYCODE_BUTTON_SELECT, Utility.XINPUT_GAMEPAD_BACK));
			_KeyMaps.add(new BFM_KeyMap(1, 0x08, KeyEvent.KEYCODE_BUTTON_START, Utility.XINPUT_GAMEPAD_START));
			_KeyMaps.add(new BFM_KeyMap(1, 0x20, KeyEvent.KEYCODE_BUTTON_THUMBL, Utility.XINPUT_GAMEPAD_LEFT_THUMB));
			_KeyMaps.add(new BFM_KeyMap(1, 0x40, KeyEvent.KEYCODE_BUTTON_THUMBR, Utility.XINPUT_GAMEPAD_RIGHT_THUMB));
			_KeyMaps.add(new BFM_DPadMap(new int[] { 0, 1, 7 }, KeyEvent.KEYCODE_DPAD_UP, Utility.XINPUT_GAMEPAD_DPAD_UP));
			_KeyMaps.add(new BFM_DPadMap(new int[] { 3, 4, 5 }, KeyEvent.KEYCODE_DPAD_DOWN, Utility.XINPUT_GAMEPAD_DPAD_DOWN));
			_KeyMaps.add(new BFM_DPadMap(new int[] { 5, 6, 7 }, KeyEvent.KEYCODE_DPAD_LEFT, Utility.XINPUT_GAMEPAD_DPAD_LEFT));
			_KeyMaps.add(new BFM_DPadMap(new int[] { 1, 2, 3 }, KeyEvent.KEYCODE_DPAD_RIGHT, Utility.XINPUT_GAMEPAD_DPAD_RIGHT));
			_KeyMaps.add(new BFM_LeftStickKeyMap());
		}

		public void judge(byte[] buffer)
		{
			// Log.d("BetopBfmEntity", hex_print(buffer, " "));
			for (IBFMKeyMotion km : _KeyMaps)
			{
				// 模拟按键
				km.judge(buffer);
			}
			// 左摇杆(模拟成-32767 - 32767)
			GamePadInfo gpi = Utility.m_GamePads[user_index];
			gpi.LX = (getUByte(buffer[3]) - 0x80) * 32767 / 0x80;
			gpi.LY = ((getUByte(buffer[4]) - 0x80) * 32767 / 0x80) * -1;
			// 右摇杆(模拟成-32767 - 32767)
			gpi.RX = (getUByte(buffer[5]) - 0x80) * 32767 / 0x80;
			gpi.RY = ((getUByte(buffer[6]) - 0x80) * 32767 / 0x80) * -1;
			// 左右食指键(0 - 255)
			gpi.L2 = getUByte(buffer[7]);
			gpi.R2 = getUByte(buffer[8]);
			
	        GameControllerAdapter.onAxisEvent("", user_index, GameControllerDelegate.THUMBSTICK_LEFT_X, gpi.LX, false);
	        GameControllerAdapter.onAxisEvent("", user_index, GameControllerDelegate.THUMBSTICK_LEFT_Y, gpi.LY, false);
	        GameControllerAdapter.onAxisEvent("", user_index, GameControllerDelegate.THUMBSTICK_RIGHT_X, gpi.RX, false);
	        GameControllerAdapter.onAxisEvent("", user_index, GameControllerDelegate.THUMBSTICK_RIGHT_Y, gpi.RY, false);
	        GameControllerAdapter.onAxisEvent("", user_index, GameControllerDelegate.BUTTON_LEFT_TRIGGER, gpi.L2, false);
	        GameControllerAdapter.onAxisEvent("", user_index, GameControllerDelegate.BUTTON_RIGHT_TRIGGER, gpi.R2, false);
		}
	}
}
