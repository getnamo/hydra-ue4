/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */

#ifndef SIXENSE_UTILS_KEYBOARD_AND_MOUSE_WIN32_HPP
#define SIXENSE_UTILS_KEYBOARD_AND_MOUSE_WIN32_HPP

#include "export.hpp"

#include "sixense_utils/interfaces.hpp"

namespace sixenseUtils {

	const int key_esc = 0x01;

#define KEY_ESCAPE          0x01
#define KEY_1               0x02
#define KEY_2               0x03
#define KEY_3               0x04
#define KEY_4               0x05
#define KEY_5               0x06
#define KEY_6               0x07
#define KEY_7               0x08
#define KEY_8               0x09
#define KEY_9               0x0A
#define KEY_0               0x0B
#define KEY_MINUS           0x0C    /* - on main keyboard */
#define KEY_EQUALS          0x0D
#define KEY_BACK            0x0E    /* backspace */
#define KEY_TAB             0x0F
#define KEY_Q               0x10
#define KEY_W               0x11
#define KEY_E               0x12
#define KEY_R               0x13
#define KEY_T               0x14
#define KEY_Y               0x15
#define KEY_U               0x16
#define KEY_I               0x17
#define KEY_O               0x18
#define KEY_P               0x19
#define KEY_LBRACKET        0x1A
#define KEY_RBRACKET        0x1B
#define KEY_RETURN          0x1C    /* Enter on main keyboard */
#define KEY_LCONTROL        0x1D
#define KEY_A               0x1E
#define KEY_S               0x1F
#define KEY_D               0x20
#define KEY_F               0x21
#define KEY_G               0x22
#define KEY_H               0x23
#define KEY_J               0x24
#define KEY_K               0x25
#define KEY_L               0x26
#define KEY_SEMICOLON       0x27
#define KEY_APOSTROPHE      0x28
#define KEY_GRAVE           0x29    /* accent grave */
#define KEY_LSHIFT          0x2A
#define KEY_BACKSLASH       0x2B
#define KEY_Z               0x2C
#define KEY_X               0x2D
#define KEY_C               0x2E
#define KEY_V               0x2F
#define KEY_B               0x30
#define KEY_N               0x31
#define KEY_M               0x32
#define KEY_COMMA           0x33
#define KEY_PERIOD          0x34    /* . on main keyboard */
#define KEY_SLASH           0x35    /* / on main keyboard */
#define KEY_RSHIFT          0x36
#define KEY_MULTIPLY        0x37    /* * on numeric keypad */
#define KEY_LMENU           0x38    /* left Alt */
#define KEY_SPACE           0x39
#define KEY_CAPITAL         0x3A
#define KEY_F1              0x3B
#define KEY_F2              0x3C
#define KEY_F3              0x3D
#define KEY_F4              0x3E
#define KEY_F5              0x3F
#define KEY_F6              0x40
#define KEY_F7              0x41
#define KEY_F8              0x42
#define KEY_F9              0x43
#define KEY_F10             0x44
#define KEY_NUMLOCK         0x45
#define KEY_SCROLL          0x46    /* Scroll Lock */
#define KEY_NUMPAD7         0x47
#define KEY_NUMPAD8         0x48
#define KEY_NUMPAD9         0x49
#define KEY_SUBTRACT        0x4A    /* - on numeric keypad */
#define KEY_NUMPAD4         0x4B
#define KEY_NUMPAD5         0x4C
#define KEY_NUMPAD6         0x4D
#define KEY_ADD             0x4E    /* + on numeric keypad */
#define KEY_NUMPAD1         0x4F
#define KEY_NUMPAD2         0x50
#define KEY_NUMPAD3         0x51
#define KEY_NUMPAD0         0x52
#define KEY_DECIMAL         0x53    /* . on numeric keypad */
#define KEY_F11             0x57
#define KEY_F12             0x58
#define KEY_F13             0x64    /*                     (NEC PC98) */
#define KEY_F14             0x65    /*                     (NEC PC98) */
#define KEY_F15             0x66    /*                     (NEC PC98) */

#define KEY_KANA            0x70    /* (Japanese keyboard)            */
#define KEY_CONVERT         0x79    /* (Japanese keyboard)            */
#define KEY_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define KEY_YEN             0x7D    /* (Japanese keyboard)            */
#define KEY_BREAK			0x7E
#define KEY_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define KEY_CIRCUMFLEX      0x90    /* (Japanese keyboard)            */
#define KEY_AT              0x91    /*                     (NEC PC98) */
#define KEY_COLON           0x92    /*                     (NEC PC98) */
#define KEY_UNDERLINE       0x93    /*                     (NEC PC98) */
#define KEY_KANJI           0x94    /* (Japanese keyboard)            */
#define KEY_STOP            0x95    /*                     (NEC PC98) */
#define KEY_AX              0x96    /*                     (Japan AX) */
#define KEY_UNLABELED       0x97    /*                        (J3100) */
#define KEY_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define KEY_RCONTROL        0x9D
#define KEY_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define KEY_DIVIDE          0xB5    /* / on numeric keypad */
#define KEY_SYSRQ           0xB7
#define KEY_RMENU           0xB8    /* right Alt */
#define KEY_HOME            0xC7    /* Home on arrow keypad */
#define KEY_UP              0xC8    /* UpArrow on arrow keypad */
#define KEY_PRIOR           0xC9    /* PgUp on arrow keypad */
#define KEY_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define KEY_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define KEY_END             0xCF    /* End on arrow keypad */
#define KEY_DOWN            0xD0    /* DownArrow on arrow keypad */
#define KEY_NEXT            0xD1    /* PgDn on arrow keypad */
#define KEY_INSERT          0xD2    /* Insert on arrow keypad */
#define KEY_DELETE          0xD3    /* Delete on arrow keypad */
#define KEY_LWIN            0xDB    /* Left Windows key */
#define KEY_RWIN            0xDC    /* Right Windows key */
#define KEY_APPS            0xDD    /* AppMenu key */

	class SIXENSE_UTILS_EXPORT mouseAndKeyboardWin32 {
	public:
		static void sendMouseClick( int click, int release ); // 0 = none, 1 = left, 2 = right
		static void sendMouseClick( int click, int release, int delay); // 0 = none, 1 = left, 2 = right
		static void sendMouseWheelMove( int dir ); // in 'clicks', + = forwards - = backwards
		static void sendKeyState( char key, int press, int release );
		static void sendKeyState( char key, int press, int release, int delay );
		static void releaseKey( char key );
		static void releaseMouseButton( int release );
		static void sendAbsoluteMouseMove( float x, float y );
		static void sendRelativeMouseMove( float x, float y );
		static void sendAbsoluteMouseMoveInPixels( int x, int y );

		static void getMousePos( int *x, int *y );
		static void getPrimaryScreenResolution( int *x, int *y );

		static void sendMouseClick( float x, float y, int click, int release );

		// This should be called once per 10ms or so...
		static void processQueue();

	private:
		static double _last_click_time;
		static float _last_absolute_mouse_pos_x;
		static float _last_absolute_mouse_pos_y;
		static float _last_absolute_mouse_pos_x_when_clicked;
		static float _last_absolute_mouse_pos_y_when_clicked;
	};

	// Define a non-static class for use by the factory constructor. These calls just
	// pass through to the static calls above.
	class MouseAndKeyboardWin32 : public IMouseAndKeyboard {
	public:
		virtual void sendMouseClick( int click, int release ) {
			mouseAndKeyboardWin32::sendMouseClick( click, release );
		}
		virtual void sendMouseClick( int click, int release, int delay) {
			mouseAndKeyboardWin32::sendMouseClick( click, release, delay );
		}
		virtual void sendMouseWheelMove( int dir ) {
			mouseAndKeyboardWin32::sendMouseWheelMove( dir );
		}
		virtual void sendKeyState( char key, int press, int release ) {
			mouseAndKeyboardWin32::sendKeyState( key, press, release );
		}
		virtual void sendKeyState( char key, int press, int release, int delay ) {
			mouseAndKeyboardWin32::sendKeyState( key, press, release, delay );
		}
		virtual void releaseKey( char key ) {
			mouseAndKeyboardWin32::releaseKey( key );
		}
		virtual void releaseMouseButton( int release ) {
			mouseAndKeyboardWin32::releaseMouseButton( release );
		}
		virtual void sendAbsoluteMouseMove( float x, float y ) {
			mouseAndKeyboardWin32::sendAbsoluteMouseMove( x, y );
		}
		virtual void sendRelativeMouseMove( float x, float y ) {
			mouseAndKeyboardWin32::sendRelativeMouseMove( x, y );
		}
		virtual void sendAbsoluteMouseMoveInPixels( int x, int y ) {
			mouseAndKeyboardWin32::sendAbsoluteMouseMoveInPixels( x, y );
		}
		virtual void getMousePos( int *x, int *y ) {
			mouseAndKeyboardWin32::getMousePos( x, y );
		}
		virtual void getPrimaryScreenResolution( int *x, int *y ) {
			mouseAndKeyboardWin32::getPrimaryScreenResolution( x, y );
		}

		virtual void sendMouseClick( float x, float y, int click, int release ) {
			mouseAndKeyboardWin32::sendMouseClick( x, y, click, release );
		}
		// This should be called once per 10ms or so...
		virtual void processQueue() {
			mouseAndKeyboardWin32::processQueue();
		}

	};

}

#endif

