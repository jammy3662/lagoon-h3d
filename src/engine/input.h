#pragma once

#include "gpu.h"

#include <array>

enum InputDevice
{
	KEYBOARD,
	CLICK,
	GAMEPAD,
	MOUSE_MOVE, // cursor movement
	MOUSE_SCROLL, // scroll wheel
	JOYSTICK,
};

enum MouseCode
{
	MOUSE_LEFT = GLFW_MOUSE_BUTTON_LEFT,
	MOUSE_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
	MOUSE_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
};

enum GamepadCode
{
	A_BUT = GLFW_GAMEPAD_BUTTON_A,
	B_BUT = GLFW_GAMEPAD_BUTTON_B,
	X_BUT = GLFW_GAMEPAD_BUTTON_X,
	Y_BUT = GLFW_GAMEPAD_BUTTON_Y,
	
	L_BUT = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
	R_BUT = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
	ZL_BUT,
	ZR_BUT,
	
	UP_BUT = GLFW_GAMEPAD_BUTTON_DPAD_UP,
	DOWN_BUT = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
	LEFT_BUT = GLFW_GAMEPAD_BUTTON_DPAD_LEFT, 
	RIGHT_BUT = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
	
	START_BUT = GLFW_GAMEPAD_BUTTON_START,
};

enum AnalogCode
{
	AXIS_UP, AXIS_DOWN,
	AXIS_LEFT, AXIS_RIGHT,
	
	// left joystick only
	AXIS_NORTH, AXIS_SOUTH,
	AXIS_WEST, AXIS_EAST,
};

enum InputAction
{
	ACCEPT, CANCEL,
	INFO, SPECIAL,
	
	MENU,
	
	PICK, ALT, SHIFT,
	
	MORE, LESS,
	PREV, NEXT,
	
	NAV_UP, NAV_DOWN,
	NAV_LEFT, NAV_RIGHT,
	
	POINT_UP, POINT_DOWN,
	POINT_LEFT, POINT_RIGHT,
	
	INPUT_ACTION_CT
};

struct Binding
{
	int device;
	int code;
	double strength;
};

typedef std::array <Binding, INPUT_ACTION_CT> Mapping;

extern const Mapping mouseAndKeyboard, gamepadAndJoystick;

// current active bindings
extern Mapping mapping;

// TODO: enables console output when set
extern char debugInput;

void initInput ();
void refreshInput ();

void captureCursor ();
void releaseCursor ();

char buttonPressed (InputAction button);
char buttonPressedNow (InputAction button);

float2 axisL ();
float2 axisR ();
