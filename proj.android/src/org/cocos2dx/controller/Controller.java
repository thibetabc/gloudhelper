package org.cocos2dx.controller;

import org.cocos2dx.lib.GameControllerAdapter;
import org.cocos2dx.lib.GameControllerDelegate;

import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class Controller {

	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// game controller event redirect
		if (((keyCode >= KeyEvent.KEYCODE_DPAD_UP && keyCode <= KeyEvent.KEYCODE_DPAD_RIGHT) && (event
				.getSource() != InputDevice.SOURCE_KEYBOARD))
				|| (keyCode >= KeyEvent.KEYCODE_BUTTON_A && keyCode <= KeyEvent.KEYCODE_BUTTON_MODE)
				|| (keyCode >= KeyEvent.KEYCODE_BUTTON_1 && keyCode <= KeyEvent.KEYCODE_BUTTON_16)) {
			int index = Utility.getGamePadUserIndex(event.getDevice());
			if (index < 0)
				return true;
			int keycode = Utility.getCorrectKeyCode(event);
			int xinputcode = Utility.m_GamePadMap.get(keycode);
			int code = getKeyCodeForCocos2dx(xinputcode);
			if (handleGamePad(index, keycode, event.getAction())) {
				GameControllerAdapter.onButtonEvent("", index, code, true, 0,
						false);
			}
		}
		return true;
	}
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		if (((keyCode >= KeyEvent.KEYCODE_DPAD_UP && keyCode <= KeyEvent.KEYCODE_DPAD_RIGHT) && (event
				.getSource() != InputDevice.SOURCE_KEYBOARD))
				|| (keyCode >= KeyEvent.KEYCODE_BUTTON_A && keyCode <= KeyEvent.KEYCODE_BUTTON_MODE)
				|| (keyCode >= KeyEvent.KEYCODE_BUTTON_1 && keyCode <= KeyEvent.KEYCODE_BUTTON_16)) {
			int index = Utility.getGamePadUserIndex(event.getDevice());
			if (index < 0)
				return true;
			int keycode = Utility.getCorrectKeyCode(event);
			int xinputcode = Utility.m_GamePadMap.get(keycode);
			int code = getKeyCodeForCocos2dx(xinputcode);
			if (handleGamePad(index, keycode, event.getAction())) {
				GameControllerAdapter.onButtonEvent("", index, code, false,
						0, false);
			}
		}
		return true;
	}
	public boolean onGenericMotionEvent(MotionEvent event) {
		if ((event.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) != 0
				&& event.getAction() == MotionEvent.ACTION_MOVE) {
			int user_index = Utility.getGamePadUserIndex(event.getDevice());
			if (user_index == -1) {
				return true;
			}

			// Process all historical movement samples in the batch.
			final int historySize = event.getHistorySize();
			for (int i = 0; i < historySize; i++) {
				processJoystickInput(user_index, Utility.m_GamePads[user_index], event, i);
			}

			// Process the current movement sample in the batch.
			processJoystickInput(user_index, Utility.m_GamePads[user_index], event, -1);

			return true;
		}
		return false;
	}
    private int getKeyCodeForCocos2dx(int keycode) {
    	
    	int cocoskey = -1;
    	
    	switch(keycode) {
    	case Utility.XINPUT_GAMEPAD_DPAD_UP:
    		cocoskey = GameControllerDelegate.BUTTON_DPAD_UP;
    		break;
    	case Utility.XINPUT_GAMEPAD_DPAD_DOWN:
    		cocoskey = GameControllerDelegate.BUTTON_DPAD_DOWN;
    		break;
    	case Utility.XINPUT_GAMEPAD_DPAD_LEFT:
    		cocoskey = GameControllerDelegate.BUTTON_DPAD_LEFT;
    		break;
    	case Utility.XINPUT_GAMEPAD_DPAD_RIGHT:
    		cocoskey = GameControllerDelegate.BUTTON_DPAD_RIGHT;
    		break;
    	case Utility.XINPUT_GAMEPAD_START:
    		cocoskey = GameControllerDelegate.BUTTON_START;
    		break;
    	case Utility.XINPUT_GAMEPAD_BACK:
    		cocoskey = GameControllerDelegate.BUTTON_SELECT;
    		break;
    	case Utility.XINPUT_GAMEPAD_LEFT_THUMB:
    		cocoskey = GameControllerDelegate.BUTTON_LEFT_THUMBSTICK;
    		break;
    	case Utility.XINPUT_GAMEPAD_RIGHT_THUMB:
    		cocoskey = GameControllerDelegate.BUTTON_RIGHT_THUMBSTICK;
    		break;
    	case Utility.XINPUT_GAMEPAD_LEFT_SHOULDER:
    		cocoskey = GameControllerDelegate.BUTTON_LEFT_SHOULDER;
    		break;
    	case Utility.XINPUT_GAMEPAD_RIGHT_SHOULDER:
    		cocoskey = GameControllerDelegate.BUTTON_RIGHT_SHOULDER;
    		break;
    	case Utility.XINPUT_GAMEPAD_A:
    		cocoskey = GameControllerDelegate.BUTTON_A;
    		break;
    	case Utility.XINPUT_GAMEPAD_B:
    		cocoskey = GameControllerDelegate.BUTTON_B;
    		break;
    	case Utility.XINPUT_GAMEPAD_X:
    		cocoskey = GameControllerDelegate.BUTTON_X;
    		break;
    	case Utility.XINPUT_GAMEPAD_Y:
    		cocoskey = GameControllerDelegate.BUTTON_Y;
    		break;
    	default:
    		cocoskey = -1;
    		break;
    	}
    	
    	return cocoskey;
    }

	private boolean handleGamePad(int user_index, int keyCode, int action) {
		if (user_index < 0 || user_index > GamePadInfo.XINPUT_USER_MAX) {
//			Log.e("ZQ", "handleGamePad. wrong user_index: " + user_index);
			return false;
		}
		GamePadInfo gamepad = Utility.m_GamePads[user_index];
		int xinputCode = Utility.m_GamePadMap.get(keyCode);
		if (xinputCode == 0) { // unknown key
//			Log.e("ZQ", "handleGamePad. unknown keyCode: " + keyCode);
			return false;
		}
		int keyPressed = 0;
		if (action == KeyEvent.ACTION_DOWN)
			keyPressed = 1;

		switch (xinputCode) {
		case Utility.XINPUT_GAMEPAD_LEFT_TRIGGER:
			if (keyPressed == 1) {
				gamepad.L2 = (gamepad.L2 == 0 ? 0xFF : gamepad.L2); // L2,
																	// 如果尚未被赋值，则设为最大值0xff
			} else {
				gamepad.L2 = 0; // L2
			}
			GameControllerAdapter.onAxisEvent("", user_index,
					GameControllerDelegate.BUTTON_LEFT_TRIGGER, gamepad.L2,
					false);
			return false;
		case Utility.XINPUT_GAMEPAD_RIGHT_TRIGGER:
			if (keyPressed == 1) {
				gamepad.R2 = (gamepad.R2 == 0 ? 0xFF : gamepad.R2); // R2,
																	// 如果尚未被赋值，则设为最大值0xff
			} else {
				gamepad.R2 = 0; // R2
			}
			GameControllerAdapter.onAxisEvent("", user_index,
					GameControllerDelegate.BUTTON_RIGHT_TRIGGER, gamepad.R2,
					false);
			return false;
		default:
			break;
		}

		return true;
	}
	 private static float getCenteredAxis(MotionEvent event, InputDevice device, int axis, int historyPos) {
	        final InputDevice.MotionRange range = device.getMotionRange(axis, event.getSource());
	        if (range != null) {
	            final float flat = Math.max(0.02f, range.getFlat()); 
	            final float value = historyPos < 0 ? event.getAxisValue(axis)
	                    : event.getHistoricalAxisValue(axis, historyPos);
	            
	             if (value > 1.0f) {
	                // MyLog.w("getCenteredAxis", "value is "+value);
	            	return 1.0f;
	            }
	            else if (value < -1.0f) {
	                // MyLog.w("getCenteredAxis", "value is "+value);
	            	return -1.0f;
	            }
	            // Ignore axis values that are within the 'flat' region of the joystick axis center.
	            // A joystick at rest does not always report an absolute position of (0,0).
	            if (Math.abs(value) > flat) {
	                return value;
	            }
	        }
	        return 0;
	    }

    private void processJoystickInput(int index, GamePadInfo gamepad, MotionEvent event, int historyPos) {
    	
        float hat_x = getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_HAT_X, historyPos);
        if (hat_x > 0.9f) {
        	gamepad.wButtons |= Utility.XINPUT_GAMEPAD_DPAD_RIGHT;
        	int keycode = getKeyCodeForCocos2dx(Utility.XINPUT_GAMEPAD_DPAD_RIGHT);
        	GameControllerAdapter.onButtonEvent("", index, keycode, true, 0, false);
        }
        else if (Math.abs(hat_x) < 0.1f) {
        	gamepad.wButtons &= ~Utility.XINPUT_GAMEPAD_DPAD_RIGHT;
        	int keycode = getKeyCodeForCocos2dx(Utility.XINPUT_GAMEPAD_DPAD_RIGHT);
        	GameControllerAdapter.onButtonEvent("", index, keycode, false, 0, false);
        	gamepad.wButtons &= ~Utility.XINPUT_GAMEPAD_DPAD_LEFT;
        	keycode = getKeyCodeForCocos2dx(Utility.XINPUT_GAMEPAD_DPAD_LEFT);
        	GameControllerAdapter.onButtonEvent("", index, keycode, false, 0, false);
        }
        else if (hat_x < -0.9f) {
        	gamepad.wButtons |= Utility.XINPUT_GAMEPAD_DPAD_LEFT;
        	int keycode = getKeyCodeForCocos2dx(Utility.XINPUT_GAMEPAD_DPAD_LEFT);
        	GameControllerAdapter.onButtonEvent("", index, keycode, true, 0, false);
        }
        float hat_y = getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_HAT_Y, historyPos);
        if (hat_y > 0.9f) {
        	gamepad.wButtons |= Utility.XINPUT_GAMEPAD_DPAD_DOWN;
        	int keycode = getKeyCodeForCocos2dx(Utility.XINPUT_GAMEPAD_DPAD_DOWN);
        	GameControllerAdapter.onButtonEvent("", index, keycode, true, 0, false);
        }
        else if (Math.abs(hat_y) < 0.1f) {
        	gamepad.wButtons &= ~Utility.XINPUT_GAMEPAD_DPAD_DOWN;
        	int keycode = getKeyCodeForCocos2dx(Utility.XINPUT_GAMEPAD_DPAD_DOWN);
        	GameControllerAdapter.onButtonEvent("", index, keycode, false, 0, false);
        	gamepad.wButtons &= ~Utility.XINPUT_GAMEPAD_DPAD_UP;
        	keycode = getKeyCodeForCocos2dx(Utility.XINPUT_GAMEPAD_DPAD_UP);
        	GameControllerAdapter.onButtonEvent("", index, keycode, false, 0, false);
        }
        else if (hat_y < -0.9f) {
        	gamepad.wButtons |= Utility.XINPUT_GAMEPAD_DPAD_UP;
        	int keycode = getKeyCodeForCocos2dx(Utility.XINPUT_GAMEPAD_DPAD_UP);
        	GameControllerAdapter.onButtonEvent("", index, keycode, true, 0, false);
        }

        // 左摇杆永远都是AXIS_X和AXIS_Y。从-1~1放大到0~32767。
        int lx = (int)(getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_X, historyPos) * 32767); // LX
        int ly = 0 - (int)(getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_Y, historyPos) * 32767); // LY
        int rx = gamepad.RX;
        int ry = gamepad.RY;
        int l2 = gamepad.L2;
        int r2 = gamepad.R2;
        
        if (gamepad.m_mode == GamePadInfo.ANDROID_MODE) {
	        // 右摇杆是AXIS_Z和AXIS_RZ。 从-1~1放大到0~32767
        	rx = (int)(getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_Z, historyPos) * 32767);
	        ry = 0 - (int)(getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_RZ, historyPos) * 32767); // RY
	        
	        // L2 R2的值就比较怪了，大部分BREAK GAS，取值是0~1，也有其他情况
			// NVSHIELD，AXIS_BRAKE(左) AXIS_GAS(右)  AXIS_LTRIGGER(左) AXIS_RTRIGGER(右)
			// 小米蓝牙手柄，AXIS_BRAKE(左) AXIS_THROTTLE(右)  RX(左,-1~1) RY(右, -1~1)
			// 北通智游者卡洛 ，AXIS_BRAKE(左) AXIS_GAS(右)
			// 北通阿修罗手柄 ，AXIS_BRAKE(左) AXIS_GAS(右)，但是无线版的GAS/Brake永远是零，而且HAT_X/Y还会影响GAS/Brake。。。
			// WAMO手柄的A模式有AXIS_BRAKE(右) AXIS_GAS(左)，居然是反的！！

	        if (!gamepad.m_dropGasBrake) {
		        // 小米蓝牙手柄，AXIS_BRAKE AXIS_THROTTLE是L2 R2，范围0~1
		        // 北通卡洛手柄，AXIS_BRAKE AXIS_GAS是L2 R2，范围0~1
		        float brake = getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_BRAKE, historyPos); // AXIS_BRAKE
		        float gas = getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_GAS, historyPos); // AXIS_GAS
		        float throttle = getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_THROTTLE, historyPos); // AXIS_THROTTLE
		        float gas_or_throttle = Math.max(gas, throttle); // 取两者之中较大的值
	            
	            // 从0~1放大到0~255
		        if (gamepad.m_brakeOnLeft) {
		            l2 = (int)(brake * 0xff);
		            r2 = (int)(gas_or_throttle * 0xff);
		        }
		        else {
		            l2 = (int)(gas_or_throttle * 0xff);
		            r2 = (int)(brake * 0xff);
		        }
	        }

        }
        else if (gamepad.m_mode == GamePadInfo.XBOX_MODE) {
        	// 大麦盒子1001、大麦盒子2003、天猫魔盒、乐视盒子(letv 2.3)上，右摇杆是AXIS_Z和AXIS_RZ，L2是AXIS_LTRIGGER，R2是AXIS_RTRIGGER
        	
        	// 大麦盒子1002、爱奇艺i71盒子上，右摇杆是AXIS_RX和AXIS_RY，L2是AXIS_Z，R2是AXIS_RZ
        	
        	// 小米盒子1S无法识别XBox模式的手柄
        	
        	if (gamepad.m_hasLTRT) {
	        	// Xbox模式的右摇杆是AXIS_Z和AXIS_RZ，L2是AXIS_LTRIGGER，R2是AXIS_RTRIGGER
	            // 从-1~1放大到0~32767
	            rx = (int)(getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_Z, historyPos) * 32767); // Z
	            ry = 0 - (int)(getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_RZ, historyPos) * 32767); // RZ
	            
	        	// LT RT是L2 R2，从0~1放大到0~255
	            float lt = getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_LTRIGGER, historyPos);
	            float rt = getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_RTRIGGER, historyPos);
	            l2 = (int)(lt * 0xff); // LTRIGGER
	            r2 = (int)(rt * 0xff); // RTRIGGER
        	}
        	if(gamepad.m_hasRXRYRZ) {
	        	// Xbox模式的右摇杆是AXIS_RX和AXIS_RY，从-1~1放大到0~32767
	            rx = (int)(getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_RX, historyPos) * 32767); // RX
	            ry = 0 - (int)(getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_RY, historyPos) * 32767); // RY

	        	// Xbox模式的左右扳机键是AXIS_Z和AXIS_RZ，从-1~1放大到0~255
	            l2 = (int)((getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_Z, historyPos) + 1.0f) * 0x7f); // Z
	            r2 = (int)((getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_RZ, historyPos) + 1.0f) * 0x7f); // RZ
        	}
        }
        else { // PC模式
        	// L2是KEYCODE_BUTTON_7，R2是KEYCODE_BUTTON_8。已经在handleGamePad里处理过了。
	        // 北通阿修罗、战戟2、莱仕达龙之血刃的右摇杆是AXIS_Z和AXIS_RZ，北通蝙蝠、莱仕达剑神右摇杆是AXIS_RX和AXIS_RZ。
            // 从-1~1放大到0~32767
	        float axis_z  = getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_Z, historyPos);
	        float axis_rx = getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_RX, historyPos);
	        if (Math.abs(axis_z) > Math.abs(axis_rx))
	        	rx = (int)(axis_z * 32767);
	        else
	        	rx = (int)(axis_rx * 32767);
            // 从-1~1放大到0~32767
	        ry = 0 - (int)(getCenteredAxis(event, event.getDevice(), MotionEvent.AXIS_RZ, historyPos) * 32767); // RY
        }
        
        GameControllerAdapter.onAxisEvent(event.getDevice().getName(), index, GameControllerDelegate.THUMBSTICK_LEFT_X, lx, false);
        GameControllerAdapter.onAxisEvent(event.getDevice().getName(), index, GameControllerDelegate.THUMBSTICK_LEFT_Y, ly, false);
        GameControllerAdapter.onAxisEvent(event.getDevice().getName(), index, GameControllerDelegate.THUMBSTICK_RIGHT_X, rx, false);
        GameControllerAdapter.onAxisEvent(event.getDevice().getName(), index, GameControllerDelegate.THUMBSTICK_RIGHT_Y, ry, false);
        GameControllerAdapter.onAxisEvent(event.getDevice().getName(), index, GameControllerDelegate.BUTTON_LEFT_TRIGGER, l2, false);
        GameControllerAdapter.onAxisEvent(event.getDevice().getName(), index, GameControllerDelegate.BUTTON_RIGHT_TRIGGER, r2, false);
    }
}
