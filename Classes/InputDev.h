#pragma once

namespace ClientCore
{
	// 操控传输
	struct XINPUT_GAMEPAD
	{
		XINPUT_GAMEPAD(void)
		{
			memset(this, 0, sizeof(XINPUT_GAMEPAD));
		}
		uint32_t sButtons;          // 枚举ClientCore::dev_xinput::xinput_button的组合
		uint8_t sLeftTrigger;       // 左二扳机键,0~255,如果没有梯度则就是0或255
		uint8_t sRightTrigger;      // 右二扳机键,0~255,如果没有梯度则就是0或255
		int16_t sThumbLX;           // 左轴X,从左到右是-32768~32767
		int16_t sThumbLY;           // 左轴Y,从下到上是-32768~32767
		int16_t sThumbRX;           // 右轴X,从左到右是-32768~32767
		int16_t sThumbRY;           // 左轴Y,从下到上是-32768~32767
	};

	namespace dev_type
	{
		enum dev_list_v1
		{
			keyboard = 1,
			mouse,
			xinput,
			gamepad,
			gamepad_first = gamepad,
			gamepad_last = gamepad_first + 8,
			customized_gamepad_begin = 63,
			customized_gamepad_end = 126,
			other_device_begin = 127,
			other_device_end = 254,
			string_input = 255,
		};
	}
	namespace dev_kb
	{
		enum key_value
		{
			// 键盘,按照虚拟键盘码定义
			KEY_BEGIN = 0,          // 键盘消息范围开始
			KEY_VK_LBUTTON = 1,     // 鼠标左键
			KEY_VK_RBUTTON = 2,     // 鼠标右键
			KEY_VK_CANCEL = 3,      // Cancel
			KEY_VK_MBUTTON = 4,     // 鼠标中键
			KEY_VK_XBUTTON1 = 5,
			KEY_VK_XBUTTON2 = 6,
			KEY_VK_BACK = 8,        // Backspace
			KEY_VK_TAB = 9,         // Tab
			KEY_VK_CLEAR = 12,      // Clear
			KEY_VK_RETURN = 13,     // Enter
			KEY_VK_SHIFT = 16,      // Shift
			KEY_VK_CONTROL = 17,    // Ctrl
			KEY_VK_MENU = 18,       // Alt
			KEY_VK_PAUSE = 19,      // Pause
			KEY_VK_CAPITAL = 20,    // Caps Lock
			KEY_VK_KANA = 21,
			KEY_VK_HANGUL = 21,
			KEY_VK_JUNJA = 23,
			KEY_VK_FINAL = 24,
			KEY_VK_HANJA = 25,
			KEY_VK_KANJI = 25,
			KEY_VK_ESCAPE = 27,     // Esc
			KEY_VK_CONVERT = 28,
			KEY_VK_NONCONVERT = 29,
			KEY_VK_ACCEPT = 30,
			KEY_VK_MODECHANGE = 31,
			KEY_VK_SPACE = 32,      // Space
			KEY_VK_PRIOR = 33,      // Page Up
			KEY_VK_NEXT = 34,       // Page Down
			KEY_VK_END = 35,        // End
			KEY_VK_HOME = 36,       // Home
			KEY_VK_LEFT = 37,       // Left Arrow
			KEY_VK_UP = 38,         // Up Arrow
			KEY_VK_RIGHT = 39,      // Right Arrow
			KEY_VK_DOWN = 40,       // Down Arrow
			KEY_VK_SELECT = 41,     // Select
			KEY_VK_PRINT = 42,      // Print
			KEY_VK_EXECUTE = 43,    // Execute
			KEY_VK_SNAPSHOT = 44,   // Snapshot
			KEY_VK_INSERT = 45,     // Insert
			KEY_VK_DELETE = 46,     // Delete
			KEY_VK_HELP = 47,       // Help
			KEY_KEY_0 = 48,   // 0
			KEY_KEY_1 = 49,   // 1
			KEY_KEY_2 = 50,   // 2
			KEY_KEY_3 = 51,   // 3
			KEY_KEY_4 = 52,   // 4
			KEY_KEY_5 = 53,   // 5
			KEY_KEY_6 = 54,   // 6
			KEY_KEY_7 = 55,   // 7
			KEY_KEY_8 = 56,   // 8
			KEY_KEY_9 = 57,   // 9
			KEY_KEY_A = 65,   // A
			KEY_KEY_B = 66,   // B
			KEY_KEY_C = 67,   // C
			KEY_KEY_D = 68,   // D
			KEY_KEY_E = 69,   // E
			KEY_KEY_F = 70,   // F
			KEY_KEY_G = 71,   // G
			KEY_KEY_H = 72,   // H
			KEY_KEY_I = 73,   // I
			KEY_KEY_J = 74,   // J
			KEY_KEY_K = 75,   // K
			KEY_KEY_L = 76,   // L
			KEY_KEY_M = 77,   // M
			KEY_KEY_N = 78,   // N
			KEY_KEY_O = 79,   // O
			KEY_KEY_P = 80,   // P
			KEY_KEY_Q = 81,   // Q
			KEY_KEY_R = 82,   // R
			KEY_KEY_S = 83,   // S
			KEY_KEY_T = 84,   // T
			KEY_KEY_U = 85,   // U
			KEY_KEY_V = 86,   // V
			KEY_KEY_W = 87,   // W
			KEY_KEY_X = 88,   // X
			KEY_KEY_Y = 89,   // Y
			KEY_KEY_Z = 90,   // Z
			KEY_VK_LWIN = 91,
			KEY_VK_RWIN = 92,
			KEY_VK_APPS = 93,
			KEY_VK_SLEEP = 95,
			KEY_VK_NUMPAD0 = 96,      // 小键盘 0
			KEY_VK_NUMPAD1 = 97,      // 小键盘 1
			KEY_VK_NUMPAD2 = 98,      // 小键盘 2
			KEY_VK_NUMPAD3 = 99,      // 小键盘 3
			KEY_VK_NUMPAD4 = 100,     // 小键盘 4
			KEY_VK_NUMPAD5 = 101,     // 小键盘 5
			KEY_VK_NUMPAD6 = 102,     // 小键盘 6
			KEY_VK_NUMPAD7 = 103,     // 小键盘 7
			KEY_VK_NUMPAD8 = 104,     // 小键盘 8
			KEY_VK_NUMPAD9 = 105,     // 小键盘 9
			KEY_VK_MULTIPLY = 106,    // 小键盘 *
			KEY_VK_ADD = 107,         // 小键盘 +
			KEY_VK_SEPARATOR = 108,   // 小键盘 Enter
			KEY_VK_SUBTRACT = 109,    // 小键盘 -
			KEY_VK_DECIMAL = 110,     // 小键盘 .
			KEY_VK_DIVIDE = 111,      // 小键盘 /
			KEY_VK_F1 = 112,          // F1
			KEY_VK_F2 = 113,          // F2
			KEY_VK_F3 = 114,          // F3
			KEY_VK_F4 = 115,          // F4
			KEY_VK_F5 = 116,          // F5
			KEY_VK_F6 = 117,          // F6
			KEY_VK_F7 = 118,          // F7
			KEY_VK_F8 = 119,          // F8
			KEY_VK_F9 = 120,          // F9
			KEY_VK_F10 = 121,         // F10
			KEY_VK_F11 = 122,         // F11
			KEY_VK_F12 = 123,         // F12
			KEY_VK_F13 = 124,
			KEY_VK_F14 = 125,
			KEY_VK_F15 = 126,
			KEY_VK_F16 = 127,
			KEY_VK_F17 = 128,
			KEY_VK_F18 = 129,
			KEY_VK_F19 = 130,
			KEY_VK_F20 = 131,
			KEY_VK_F21 = 132,
			KEY_VK_F22 = 133,
			KEY_VK_F23 = 134,
			KEY_VK_F24 = 135,
			KEY_VK_NUMLOCK = 144,   // Num Lock
			KEY_VK_SCROLL = 145,    // Scroll
			KEY_VK_LSHIFT = 160,
			KEY_VK_RSHIFT = 161,
			KEY_VK_LCONTROL = 162,
			KEY_VK_RCONTROL = 163,
			KEY_VK_LMENU = 164,
			KEY_VK_RMENU = 165,
			KEY_VK_BROWSER_BACK = 166,
			KEY_VK_BROWSER_FORWARD = 167,
			KEY_VK_BROWSER_REFRESH = 168,
			KEY_VK_BROWSER_STOP = 169,
			KEY_VK_BROWSER_SEARCH = 170,
			KEY_VK_BROWSER_FAVORITES = 171,
			KEY_VK_BROWSER_HOME = 172,
			KEY_VK_VOLUME_MUT = 173,            // VolumeMute
			KEY_VK_VOLUME_DOWN = 174,           // VolumeDown
			KEY_VK_VOLUME_UP = 175,             // VolumeUp
			KEY_VK_MEDIA_NEXT_TRACK = 176,
			KEY_VK_MEDIA_PREV_TRACK = 177,
			KEY_VK_MEDIA_STOP = 178,
			KEY_VK_MEDIA_PLAY_PAUSE = 179,
			KEY_VK_LAUNCH_MAIL = 180,
			KEY_VK_LAUNCH_MEDIA_SELECT = 181,
			KEY_VK_LAUNCH_APP1 = 182,
			KEY_VK_LAUNCH_APP2 = 183,
			KEY_VK_OEM_1 = 186,                 // ; :
			KEY_VK_OEM_PLUS = 187,              // = +
			KEY_VK_OEM_COMMA = 188,
			KEY_VK_OEM_MINUS = 189,             // - _
			KEY_VK_OEM_PERIOD = 190,
			KEY_VK_OEM_2 = 191,                 // / ?
			KEY_VK_OEM_3 = 192,                 // ` ~
			KEY_VK_OEM_4 = 219,                 // [ {
			KEY_VK_OEM_5 = 220,                 // \ |
			KEY_VK_OEM_6 = 221,                 // ] }
			KEY_VK_OEM_7 = 222,                 // ' "
			KEY_VK_OEM_8 = 223,
			KEY_VK_OEM_102 = 226,
			KEY_VK_PACKET = 231,
			KEY_VK_PROCESSKEY = 229,
			KEY_VK_ATTN = 246,
			KEY_VK_CRSEL = 247,
			KEY_VK_EXSEL = 248,
			KEY_VK_EREOF = 249,
			KEY_VK_PLAY = 250,
			KEY_VK_ZOOM = 251,
			KEY_VK_NONAME = 252,
			KEY_VK_PA1 = 253,
			KEY_VK_OEM_CLEAR = 254,
			KEY_END = 510,                      // 键盘消息范围结束
		};
		enum code_v1
		{
			// All of codes in this range following the WIN32 "virtual-key codes"
			VKEY_FIRST = 1,
			VKEY_LAST = 254,
		};
		enum action_v1
		{
			DOWN = 2,
			UP,
			UNTOGGLED,
			TOGGLED,
			HOLD,
		};
	}
	namespace dev_mouse
	{
		enum code_v1
		{
			// The BTN_xxx and POINT code has a hi/loword combination to the screen coordinate (x, y) in dev_param
			BTN_LEFT = 1,
			BTN_RIGHT = 2,
			BTN_MID = 4,
			BTN_X1 = 5,
			BTN_X2 = 6,
			POINT = 32,
			// WHEEL, AXIS_? has the dev_param for its own value
			WHEEL = 7,
			AXIS_Z = WHEEL,
			AXIS_X = 8,
			AXIS_Y = 16,
			//Unrecognized button, the id (instance in directinput) concept was saved in dev_param
			BTN_RAW = 33,
		};
		enum action_v1
		{
			// HOVER indicate the action gives an absolutive coordinates
			HOVER = 1,
			DOWN = 2,
			UP,
			// MOVE indicate the action gives an relative coordinates
			MOVE = 8,
		};
	}
	namespace dev_xinput
	{
		static const int XINPUT_USER_MAX = 4;         // 最多支持四个手柄
		enum xinput_button
		{
			XINPUT_GAMEPAD_ERROR = -1,				  //无效按键 -1
			XINPUT_GAMEPAD_DPAD_UP = 0x0001,          // 左边按键:上
			XINPUT_GAMEPAD_DPAD_DOWN = 0x0002,        // 左边按键:下
			XINPUT_GAMEPAD_DPAD_LEFT = 0x0004,        // 左边按键:左
			XINPUT_GAMEPAD_DPAD_RIGHT = 0x0008,       // 左边按键:右
			XINPUT_GAMEPAD_START = 0x0010,            // 对应北通战戟2的9
			XINPUT_GAMEPAD_BACK = 0x0020,             // 对应北通战戟2的10
			XINPUT_GAMEPAD_LEFT_THUMB = 0x0040,       // 左轴按键, 对应北通战戟2的11
			XINPUT_GAMEPAD_RIGHT_THUMB = 0x0080,      // 右轴按键, 对应北通战戟2的12
			XINPUT_GAMEPAD_LEFT_SHOULDER = 0x0100,    // 对应北通战戟2的5
			XINPUT_GAMEPAD_RIGHT_SHOULDER = 0x0200,   // 对应北通战戟2的6
			XINPUT_GAMEPAD_A = 0x1000,                // 右边按键:下   A, 对应北通战戟2的3
			XINPUT_GAMEPAD_B = 0x2000,                // 右边按键:右   B, 对应北通战戟2的2
			XINPUT_GAMEPAD_X = 0x4000,                // 右边按键:左   X, 对应北通战戟2的4
			XINPUT_GAMEPAD_Y = 0x8000,                // 右边按键:上   Y, 对应北通战戟2的1
		};
		enum code_v1
		{
			BTN_FIRST = 1,
			BTN_ALL,        // xinput_button中的所有键值都存在一个uint32里
			BTN_L2,         // 左二扳机键,0~255,如果没有梯度则就是0或255,对应北通战戟2的7
			BTN_R2,         // 右二扳机键,0~255,如果没有梯度则就是0或255,对应北通战戟2的8
			BTN_CLEAR = 15,
			// 方向轴
			AXIS_LX = 64,   // 左轴X,从左到右是-32768~32767
			AXIS_LY,        // 左轴Y,从下到上是-32768~32767
			AXIS_RX,        // 右轴X,从左到右是-32768~32767
			AXIS_RY,        // 左轴Y,从下到上是-32768~32767
		};
		enum action_v1
		{
			DEFAULT = 1,
		};
	}
	namespace dev_stringinput
	{
		//dev_param specified the size of string (**the size in char, though it was saved as wchar**) and extparam specified the content
		enum action_v1
		{
			SENDSTRING,
		};
	}
	
}
